/*
 * FileStream.h
 *
 * Copyright 2024, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef FILESTREAM_H
#define FILESTREAM_H

#include <string.h> // for strlen() under Linux
#include <string>
#include <exception>
#include <locale>

#include "Common.h"

LOGENGINE_NS_BEGIN

enum TFileMode { fmRead, fmWrite, fmReadWrite, fmWriteTrunc };
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
protected:
	bool FEOF = false;
public:
	virtual ~TStream() {}
	virtual int Read(void* Buffer, size_t Size) = 0;
	virtual int Write(const void* Buffer, const size_t Size) = 0;
	virtual size_t Length() = 0;
	virtual off_t Seek(const off_t Offset, const TSeekMode sMode) = 0;
	//virtual int ReadChar();
	//virtual int ReadWChar();
	virtual std::string ReadPString();
	virtual void WritePString(std::string& str);
	virtual bool Eof() { return FEOF; }

	template<class R>
	void operator >>(R& Value)
	{
		//static_assert(std::is_integral<R>::value || std::is_floating_point<R>::value); // template works only for integral types + float types
		if constexpr (std::is_integral<R>::value || std::is_floating_point<R>::value)
		{
			if (Read(&Value, sizeof(Value)) != sizeof(Value)) // integral type needs to be read in full, that is why we throw an exception if not.
				throw IOException("End of stream reached!");
		}
		else if constexpr (std::is_base_of<std::basic_string<typename R::value_type, typename R::traits_type>, R>::value)
		{
			Value = ReadString<R>();
		}

	}

	template<typename W>
	TStream& operator <<(const W& Value)
	{
		//static_assert(std::is_integral<W>::value || std::is_floating_point<W>::value); // template works only for integral types + float types
		
		if constexpr (std::is_integral<W>::value || std::is_floating_point<W>::value)
		{
			Write(&Value, sizeof(Value));
		}
		/*else if constexpr (std::is_same_v<W, wchar_t*>)
		{
			auto len = wcslen(Value) * sizeof(wchar_t);
			if (Write(Value, len) != len)
				throw IOException("Cannot write to the stream.");
		}
		else if (std::is_integral<std::remove_pointer<W>>::value) // W is a pointer to integral type
		{
		}*/
		else if constexpr (std::is_base_of<std::basic_string<typename W::value_type, typename W::traits_type>, W>::value)
		{			
			Write(Value.data(), Value.size() * sizeof(W::value_type));
			
			W endL;
			BUILD_ENDL(endL);
			Write(endL.data(), endL.size() * sizeof(W::value_type));
		}

		return *this;
	}

	template<typename R>
	int ReadChar()
	{
		typename R::value_type c;
		if (Read(&c, sizeof(c)) == sizeof(R::value_type))
			return c;
		else
			return -1;
	}

	int ReadChar()
	{
		char c;
		if (Read(&c, sizeof(char)) == sizeof(char))
			return c;
		else
			return -1;
	}

	// reading string char-by-char until EndLineChar ('\n') reached.
	template<typename R>
	R ReadString()
	{
		// check if class R is instantiation or descendant of std::basic_string
		static_assert(std::is_base_of<std::basic_string<typename R::value_type, typename R::traits_type>, R>::value);
	
		R value;
		do
		{
			int c = ReadChar<R>();
			if (c == EndLineChar)
			{
				if (value[value.size() - 1] == CRChar) value.resize(value.size() - 1); // if '\r' has been read right BEFORE '\n' delete it
				return value;
			};
			if (c == -1) return value; // end of file reached
			value += static_cast<typename R::value_type>(c);
		} while (true);

		return value;
	}


	TStream& operator <<(const char* Value)    { Write(Value, strlen(Value)); return *this; }
	TStream& operator <<(char* Value)          { Write(Value, strlen(Value)); return *this; }
	TStream& operator <<(const wchar_t* Value) { Write(Value, wcslen(Value) * sizeof(wchar_t)); return *this; }
	TStream& operator <<(wchar_t* Value)       { Write(Value, wcslen(Value) * sizeof(wchar_t)); return *this; }

	//void operator <<(const std::string& Value) { Write(Value.data(), Value.length()); Write(EndLine, strlen(EndLine)); } // need Endline to determine where string ends
	//void operator <<(const std::wstring& Value) { Write(Value.data(), Value.length()*sizeof(std::wstring::value_type)); Write(EndLineU, wcslen(EndLineU)*sizeof(std::wstring::value_type)); } // need Endline to determine where string ends
	//void operator >>(std::string& Value);
	//void operator >>(std::wstring& Value);
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
