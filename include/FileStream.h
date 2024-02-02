/*
 * FileStream.h
 *
 * Copyright 2024, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef FILESTREAM_H
#define FILESTREAM_H

#include <string>
#include <exception>
#include "Common.h"

LOGENGINE_NS_BEGIN

enum TFileMode { fmRead, fmWrite, fmReadWrite };
enum TSeekMode { smFromBegin, smFromEnd, smFromCurrent };

#define IO_EXCEPTION_PREFIX "LogException : "

class IOException : public std::exception
{
public:
	IOException(const char* Message) { Text = Message; whatText = IO_EXCEPTION_PREFIX + std::string(Message); }
	IOException(const std::string& Message) { Text = Message; whatText = IO_EXCEPTION_PREFIX + Message; }
	IOException(const IOException& ex) { Text = ex.Text; whatText = ex.whatText; }
	~IOException() noexcept override {}
	IOException& operator=(const IOException& ex) { Text = ex.Text; whatText = ex.whatText;	return *this; }
	const char* what() const noexcept override { return whatText.c_str(); }
	//std::string GetError(void);
private:
	std::string Text;
	std::string whatText;
};


class TStream
{
public:
	virtual ~TStream() {}
	virtual int Read(void* Buffer, size_t Size) = 0;
	virtual int Write(const void* Buffer, const size_t Size) = 0;
	virtual size_t Length() = 0;
	virtual off_t Seek(const off_t Offset, const TSeekMode sMode) = 0;
	virtual int ReadChar();
	virtual std::string ReadPString();
	virtual void WritePString(std::string& str);

	template<class R>
	void operator >>(R& Value)
	{
		static_assert(std::is_integral<R>::value || std::is_floating_point<R>::value); // template works only for integral types + float types
		if (Read(&Value, sizeof(Value)) != sizeof(Value))
			throw IOException("End of stream reached!");
	}

	template<class W>
	void operator <<(const W& Value)
	{
		static_assert(std::is_integral<W>::value || std::is_floating_point<W>::value); // template works only for integral types + float types
		if (Write(&Value, sizeof(Value)) != sizeof(Value))
			throw IOException("Cannot write to the stream.");
	}

	void operator <<(const char* Value)  { Write(Value, strlen(Value)); }
	void operator <<(char* Value)        { Write(Value, strlen(Value)); }
	void operator <<(std::string& Value) { Write(Value.data(), Value.length()); Write(EndLine, strlen(EndLine)); } // need Endline to determine where string ends
	void operator >>(std::string& Value);
};

#define DEFAULT_BUF_SIZE 1024

class TMemoryStream : public TStream
{
private:
	uint8_t* FMemory = nullptr;
	size_t FSize = 0;
	size_t FRPos = 0;
	size_t FWPos = 0;
	bool FNeedFree = true;

	void ResetPos();
	off_t Seek(const off_t, const TSeekMode) override { return -1; } // hide Seek, use SeekR and SeekW instead
public:
	TMemoryStream(uint BuffSize = DEFAULT_BUF_SIZE) { FMemory = new uint8_t[BuffSize]; }
	~TMemoryStream() override { if (FNeedFree) free(FMemory); }
	int Read(void* Buffer, size_t Size) override;
	int Write(const void* Buffer, const size_t Size) override;
	size_t Length() override { return FSize; }
	off_t SeekR(const off_t Offset, const TSeekMode sMode);
	off_t SeekW(const off_t Offset, const TSeekMode sMode);

	void SetBuffer(uint8_t* Buffer, size_t Size);
	void UnsetBuffer();
};


class TFileStream :public TStream
{
private:
	std::string FFileName;
	TFileMode FFileMode;
	int hf = 0;  // file handle
public:
	TFileStream(const std::string& FileName, const TFileMode fMode = fmWrite);
	~TFileStream() override;
	int Read(void* Buffer, size_t Size) override;
	int Write(const void* Buffer, const size_t Size) override;
	int Write(const std::string& str);
	int WriteLn(const std::string& str);
	int WriteLn(const void* Buffer, const size_t Size);
	int WriteCRLF(void);
	size_t Length() override;
	void Flush();

	/* Moves the current position in the file.
	 * When sMode=smFromEnd the current position moves _back_ (to the beginning).
	 * Negative values of Offset parameter are allowed only when sMode=smFromCurrent.
	 */
	off_t Seek(const off_t Offset, const TSeekMode sMode) override;

	std::string GetFileName() const { return FFileName; }

};


LOGENGINE_NS_END

#endif //FILESTREAM_H
