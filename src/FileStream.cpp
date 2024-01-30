/*
 * FileStream.cpp
 *
 * Copyright 2024, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#ifdef WIN32
#include <io.h>
#else
#include <sys/io.h>
#include <unistd.h>
#endif

#include "FileStream.h"
#include <cassert>

LOGENGINE_NS_BEGIN

using namespace std;

//////////////////////////////////////////////////////////////////////
//  TStream Class
//////////////////////////////////////////////////////////////////////

int TStream::ReadChar()
{
	char c;
	if (Read(&c, sizeof(c)) == sizeof(char))
		return c;
	else 
		return -1;
}

void TStream::operator >>(string& Value)
{
	Value.clear();
	do 
	{
		int c = ReadChar();
		if (c == -1) return; // end of file reached
		if (c == EndLineChar) return;
		Value += (char)c;
	} while (true);
}

string TStream::ReadPString()
{
	string res;
	int i;
	*this >> i;
	res.resize(i);
	Read((void*)res.data(), res.length());
	return res;
}

void TStream::WritePString(std::string& str)
{
	*this << str.size();
	*this << str.data();
}

//////////////////////////////////////////////////////////////////////
//  TMemoryStream Class
//////////////////////////////////////////////////////////////////////

void TMemoryStream::ResetPos()
{
	FSize = 0;
	FRPos = 0;
	FWPos = 0;
}

int TMemoryStream::Read(void* Buffer, size_t Size)
{
	assert(FRPos <= FSize);
	assert(FWPos <= FSize);

	if (Buffer == nullptr) return -1;
	if (Size == 0) return -1; 

	if (FRPos + Size > FSize) Size = FSize - FRPos;
	if (Size == 0) return 0; // we've have read everything (EOF reached)

	memcpy(Buffer, FMemory + FRPos, Size);

	FRPos += Size;
	return (int)Size;
}

int TMemoryStream::Write(const void* Buffer, const size_t Size)
{
	_set_errno(EINVAL);
	if (Buffer == nullptr) return -1;
	if (Size == 0) return -1;
	_set_errno(0);

	if (FWPos + Size > FSize) // not enough space in FMemory buffer 
		if(FNeedFree) // we control the buffer, so we can reallocate it 
			FMemory = static_cast<uint8_t*>(realloc(FMemory, FWPos + Size)), FSize = FWPos + Size;
		else   // we do not control the buffer, so we cannot reallocate it
		{
			_set_errno(ENOSPC);
			return -1;
		}

	memcpy(FMemory + FWPos, Buffer, Size);
	FWPos += Size;
	return (int)Size;
}

template<typename T>
T check_range(const T& start, const T& end, const T& offset)
{
	return (start + offset < 0) ? 0 : (start + offset > end) ? end : (start + offset);
}

#define CHECK_RANGE(start, end, offset) check_range((start), (end), (offset)) 
#define CHECK_RANGE_OFF_T(start, end, offset) check_range(static_cast<off_t>(start), static_cast<off_t>(end), (offset)) 


off_t TMemoryStream::SeekR(const off_t Offset, const TSeekMode sMode)
{
	switch (sMode)
	{
	case smFromBegin:  FRPos = CHECK_RANGE_OFF_T(0,     FSize,  Offset); return static_cast<off_t>(FRPos);  //(Offset < 0) ? 0 : (Offset > FSize) ? FSize : Offset; return static_cast<off_t>(FRPos);
	case smFromEnd:    FRPos = CHECK_RANGE_OFF_T(FSize, FSize, -Offset); return static_cast<off_t>(FRPos); //(Offset > FSize) ? 0 : FSize - Offset; return static_cast<off_t>(FRPos);
	case smFromCurrent:FRPos = CHECK_RANGE_OFF_T(FRPos, FSize,  Offset); return static_cast<off_t>(FRPos);   //(static_cast<off_t>(FRPos) + Offset < 0) ? 0 : (FRPos + Offset > FSize) ? FSize : FRPos + Offset; return static_cast<off_t>(FRPos);
	default:
		return -1l;
	}
}

off_t TMemoryStream::SeekW(const off_t Offset, const TSeekMode sMode)
{
	switch (sMode)
	{
	case smFromBegin:  FWPos = CHECK_RANGE_OFF_T(0,     FSize,  Offset); return static_cast<off_t>(FWPos);
	case smFromEnd:    FWPos = CHECK_RANGE_OFF_T(FSize, FSize, -Offset); return static_cast<off_t>(FWPos);
	case smFromCurrent:FWPos = CHECK_RANGE_OFF_T(FWPos, FSize,  Offset); return static_cast<off_t>(FWPos);
	default:
		return -1l;
	}
}

void TMemoryStream::SetBuffer(uint8_t* Buffer, size_t Size)
{
	ResetPos();
	if (FNeedFree) free(FMemory);
	FNeedFree = false;
	FMemory = Buffer;
	FSize = Size;
}

// call this method when you want to return back to internal buffer
void TMemoryStream::UnsetBuffer()
{
	if (FNeedFree) return; // if we have internal buffer - do nothing
	
	// otherwise reset all positions and allocate buffer of default size
	ResetPos();
	FNeedFree = true;
	FMemory = new uint8_t[DEFAULT_BUF_SIZE];
	FSize = DEFAULT_BUF_SIZE;
}


//////////////////////////////////////////////////////////////////////
//  TFileStream Class
//////////////////////////////////////////////////////////////////////

#ifdef WIN32
#define mylseek _lseek
#define myread _read
#define mywrite _write
#define myclose _close
#else
#define mylseek lseek
#define myread read
#define mywrite write
#define myclose close
#endif


TFileStream::TFileStream(const string& FileName, const TFileMode fMode)
{
	FFileName = FileName;
	FFileMode = fMode;
	hf = 0;

#if defined(WIN32) && !defined(__BORLANDC__)
	errno_t res = 0;
	switch (fMode)
	{
	case fmRead:     res = _sopen_s(&hf, FFileName.c_str(), O_RDONLY | O_BINARY, _SH_DENYNO, _S_IREAD | _S_IWRITE); break;
	case fmWrite:    res = _sopen_s(&hf, FFileName.c_str(), O_WRONLY | O_CREAT | O_BINARY, _SH_DENYWR, _S_IREAD | _S_IWRITE); break;
	case fmReadWrite:res = _sopen_s(&hf, FFileName.c_str(), O_RDWR | O_CREAT | O_BINARY, _SH_DENYWR, _S_IREAD | _S_IWRITE); break;
	}
#else
	switch (fMode)
	{
	case fmRead:     hf = open(FFileName.c_str(), O_RDONLY); break;
	case fmWrite:    hf = open(FFileName.c_str(), O_WRONLY | O_CREAT, S_IREAD | S_IWRITE); break;
	case fmReadWrite:hf = open(FFileName.c_str(), O_RDWR | O_CREAT, S_IREAD | S_IWRITE); break;
	}

	int res = errno;
#endif

	if (hf == -1)
	{
		string s;
		if (res == EINVAL)
			s = "Wrong file name '" + FileName + "'!";
		else if (res == EACCES)
			s = "Can't get access to file '" + FileName + "'!";
		else
			s = "Can't open file '" + FileName + "'!";

		throw IOException(s.c_str());
	}
}

TFileStream::~TFileStream()
{
	myclose(hf);
	hf = 0;
}

int TFileStream::Read(void* Buffer, size_t Size)
{
	if (FFileMode == fmWrite)
		throw IOException("File opened in write-only mode. Can't read!");

	int c = myread(hf, Buffer, (uint)Size);

	if (c == -1)
	{
		string s = "Cannot read from file '" + FFileName + "'! May be file closed?";
		throw IOException(s.c_str());
	}

	return c;
}

int TFileStream::WriteCRLF(void)
{
	const char* temp = EndLine;
	return Write(temp, strlen(temp));
}

int TFileStream::Write(const string& str)
{
	return Write(str.data(), str.length());
}

int TFileStream::Write(const void* Buffer, const size_t Size)
{
	if (FFileMode == fmRead)
		throw IOException("File opened in read-only mode. Can't write!");

	int c = mywrite(hf, Buffer, (uint)Size);

	if (c == -1)
	{
		string s = "Cannot write to file '" + FFileName + "'! May be disk full?";
		throw IOException(s.c_str());
	}

	return c;
}

int TFileStream::WriteLn(const string& str)
{
	int c = Write(str);
	return WriteCRLF() + c; 
}

int TFileStream::WriteLn(const void* Buffer, const size_t Size)
{
	int c = Write(Buffer, Size);
	return WriteCRLF() + c;
}

off_t TFileStream::Seek(off_t Offset, TSeekMode sMode)
{
	switch (sMode)
	{
	case smFromBegin:   return mylseek(hf, Offset, SEEK_SET);
	case smFromEnd:     return mylseek(hf, Offset, SEEK_END);
	case smFromCurrent: return mylseek(hf, Offset, SEEK_CUR);
	}

	throw IOException("Invalid Seek() mode.");
}

size_t TFileStream::Length()
{
	struct stat st;

	fstat(hf, &st);
	return st.st_size;
};

void TFileStream::Flush()
{
#ifdef WIN32
	_commit(hf);
#else
	fsync(hf);
#endif
}

LOGENGINE_NS_END 
