/*
 * FileSink.h
 *
 * Copyright 2025, LogEngine2 Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#pragma once

#include <string>
#include <iostream>
#include <functional>
#include "Common.h"
#include "BaseSink.h"
#include "LogEvent.h"
#include "FileStream.h"
//#include "PatternLayout.h"
//#include "Exceptions.h"

LOGENGINE_NS_BEGIN

template<class Mutex>
class FileSink : public BaseSink<Mutex>
{
protected:
	TFileStream* FStream;

	void sendMsg(const LogEvent& e) override
	{
		std::string str = this->FormatString(e); // FormatString adds '\n' to the end of string
		//FStream.Seek(0, smFromEnd); // TODO this is done for the case when two filesinks write into the same file. may be not good solution to position for every log line.
		this->FBytesWritten += static_cast<ullong>(FStream->WriteLn(str));
	}

public:
	FileSink(const std::string& name, const std::string& fileName) : BaseSink<Mutex>(name)
	{
		FStream = new TFileStream(fileName);
		FStream->Seek(0, smFromEnd); // move to the end of file 
		//SetLayout(new PatternLayout());
	}

	~FileSink() override 
	{ 
		FStream->Flush(); 
		delete FStream; 
	}

	/**
	* Get the FileName of this file sink. The name of the file where sink writes its logs.
	* @returns the FileName of the sink.
	**/
	std::string GetFileName() const { return FStream->GetFileName(); }

	void Flush() override {	FStream->Flush(); }
};

template<class Mutex>
class StdoutSink : public BaseSink<Mutex>
{
public:
	StdoutSink(const std::string& name) : BaseSink<Mutex>(name) { /*SetLayout(new PatternLayout());*/ }

	void sendMsg(const LogEvent& e) override
	{
		std::string str = this->FormatString(e);
		std::cout << str << EndLine;
	}
};

template<class Mutex>
class StderrSink : public BaseSink<Mutex>
{
public:
	StderrSink(const std::string& name) : BaseSink<Mutex>(name) { /*SetLayout(new PatternLayout());*/ }

	void sendMsg(const LogEvent& e) override
	{
		std::string str = this->FormatString(e);
		std::cerr << str << std::endl;
	}
};

template<class Mutex>
class StringSink : public BaseSink<Mutex>
{
private:
	std::ostringstream output;
public:
	StringSink(const std::string& name) : BaseSink<Mutex>(name) { /*SetLayout(new PatternLayout());*/ }

	void sendMsg(const LogEvent& e) override
	{
		std::string str = this->FormatString(e);
		output << str << std::endl;
	}

	std::string GetOutput() { return output.str(); }
	void Clear() { output.str(""); }
};

// callbacks type
typedef std::function<void(const LogEvent& lv)> CustomLogCallback;

template<class Mutex>
class CallbackSink : public BaseSink<Mutex>
{
private:
	CustomLogCallback FCallback;
public:
	CallbackSink(const std::string& name, const CustomLogCallback& callback) : BaseSink<Mutex>(name), FCallback(callback) { }

	void sendMsg(const LogEvent& e) override
	{
		FCallback(e);
	}
};

LOGENGINE_NS_END

