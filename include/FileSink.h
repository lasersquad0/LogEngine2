/*
 * FileSink.h
 *
 * Copyright 2024, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#pragma once

#include <string>
#include <iostream>
#include "Sink.h"
#include "LogEvent.h"
#include "FileStream.h"
#include "PatternLayout.h"
//#include "Exceptions.h"

LOGENGINE_NS_BEGIN

class FileSink : public Sink
{
public:
	FileSink(const std::string& name, const std::string& fileName) : Sink(name)
	{
		FStream = new TFileStream(fileName);
		FStream->Seek(0, smFromEnd); // move to the end of file 
		SetLayout(new PatternLayout());
	}

	~FileSink() override { FStream->Flush(); delete FStream; }

	void SendMsg(LogEvent& e) override
	{
		std::string str = FormatString(e); // FormatString adds '\n' to the end of string
		//FStream.Seek(0, smFromEnd); // TODO this is done for the case when two filesinks write into the same file. may be not good solution to position for every log line.
		FBytesWritten += static_cast<ullong>(FStream->WriteLn(str));
	}

	/**
	* Get the FileName of this file sink. The name of the file where sink writes its logs.
	* @returns the FileName of the sink.
	**/
	std::string getFileName() const { return FStream->GetFileName(); }

protected:
	TFileStream* FStream;
};


class StdoutSink : public Sink
{
public:
	StdoutSink(const std::string& name) : Sink(name) { SetLayout(new PatternLayout()); }

	void SendMsg(LogEvent& e) override
	{
		std::string str = FormatString(e);
		std::cout << str << EndLine;
	}
};


class StderrSink : public Sink
{
public:
	StderrSink(const std::string& name) : Sink(name) { SetLayout(new PatternLayout()); }

	void SendMsg(LogEvent& e) override
	{
		std::string str = FormatString(e);
		std::cerr << str << std::endl;
	}
};

class StringSink : public Sink
{
private:
	std::ostringstream output;
public:
	StringSink(const std::string& name) : Sink(name) { SetLayout(new PatternLayout()); }

	void SendMsg(LogEvent& e) override
	{
		std::string str = FormatString(e);
		output << str << std::endl;
	}

	std::string GetOutput() { return output.str(); }
	void Clear() { output.str(""); }
};


LOGENGINE_NS_END

