/*
 * Sink.h
 *
 * Copyright 2025, LogEngine2 Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#pragma once

#include <string>
#include "Common.h"
#include "LogEvent.h"
#include "PatternLayout.h"

LOGENGINE_NS_BEGIN

//special struct that emulates mutex but does noting, for single threaded Sinks
struct NullMutex
{
	void lock() {}
	void unlock() {}
};

class Sink
{
protected:
	std::string FName;
	Levels::LogLevel FLogLevel{ LL_DEFAULT };
	
	virtual bool shouldLog(const Levels::LogLevel ll) { return FLogLevel >= ll; }
	virtual void sendMsg(const LogEvent& e) = 0;

public:
	Sink(const std::string& name, const Levels::LogLevel ll = Levels::llInfo) : FName{ name }, FLogLevel{ ll }
	{
		//for (uint i = 0; i < n_LogLevels; i++)
		//	FMessageCounts[i] = 0;
	}

	virtual ~Sink() = default;

	virtual void Flush() { /* does nothing here*/ }
	
	virtual void PubSendMsg(const LogEvent& e) = 0;

	virtual PatternLayout* GetLayout() const = 0;
	virtual void SetLayout(PatternLayout* layout) = 0;

	// sets log line pattern for the specified by parameter 'll' log line
	virtual void SetPattern(const std::string& pattern, Levels::LogLevel ll) = 0;

	// sets log line pattern for all log lines
	virtual void SetPattern(const std::string& pattern) = 0;

	virtual std::string FormatString(const LogEvent& e) = 0;

	// sets log line pattern for the specified by parameter 'll' log line
	virtual void SetDefaultPattern(Levels::LogLevel ll) = 0;

	// sets log line pattern for all log lines
	virtual void SetDefaultPattern() = 0;

	/**
	* Returns bytes written since creating the sink.
	* @returns bytes written as value of size_t type.
	**/
	virtual ullong GetBytesWritten() const = 0;

	/**
	* Returns message statistic. Message statistic it is number of written messages per log level.
	* @returns array of size_t valuesm one value for each log level.
	**/
	virtual ullong* GetMessageCounts() = 0;
	
	Levels::LogLevel GetLogLevel() const 
	{ 
		return FLogLevel; //TODO implement it using atomics for thread safety
	}
	
	void SetLogLevel(Levels::LogLevel ll) 
	{
		FLogLevel = ll; //TODO implement it using atomics for thread safety 
	}

	/**
	 * Get the name of this sink. The name identifies the sink.
	 * @returns the name of the sink.
	 **/
	const std::string& GetName() const { return FName; }
};

template<class Mutex>
class BaseSink : public Sink
{
protected:
	//std::string FName;
	//Levels::LogLevel FLogLevel{ LL_DEFAULT };
	Mutex FMutex;
	PatternLayout* FLayout{ nullptr };
	ullong FMessageCounts[static_cast<int>(Levels::n_LogLevels)]{ 0,0,0,0,0,0 };
	ullong FBytesWritten = 0; //TODO may be move to Sink class?

	//bool shouldLog(const Levels::LogLevel ll) const { return FLogLevel >= ll; }
	//virtual void sendMsg(const LogEvent& e) = 0;

public:
	BaseSink(const std::string& name, const Levels::LogLevel ll = Levels::llInfo) : Sink(name, ll), FLayout{ new PatternLayout() }
	{
		//for (uint i = 0; i < n_LogLevels; i++)
		//	FMessageCounts[i] = 0;
	}

	//TODO add here constructor with PatternLayout parameter. Decide who should delete PatternLayout in this case.

	~BaseSink() override
	{
		delete FLayout;
		FLayout = nullptr;
	}

	virtual void Flush() { /* does nothing here*/ }

	void PubSendMsg(const LogEvent& e) override
	{
		std::lock_guard<Mutex> lock(FMutex);

		if (!shouldLog(e.msgLevel)) return;

		sendMsg(e);
	}

	PatternLayout* GetLayout() const override { return FLayout; }
	void SetLayout(PatternLayout* layout) override
	{
		std::lock_guard<Mutex> lock(FMutex);

		if (layout == nullptr) return; // if layout is null do nothing
		if (FLayout != nullptr) delete FLayout;
		FLayout = layout;
	}

	// sets log line pattern for the specified by parameter 'll' log line
	void SetPattern(const std::string& pattern, Levels::LogLevel ll) override
	{
		std::lock_guard<Mutex> lock(FMutex);
		FLayout->SetPattern(pattern, ll);
	}

	// sets log line pattern for all log lines
	void SetPattern(const std::string& pattern) override
	{
		std::lock_guard<Mutex> lock(FMutex);
		FLayout->SetPattern(pattern);
	}

	virtual std::string FormatString(const LogEvent& e) override
	{
		std::string str = FLayout->Format(e);
		FMessageCounts[e.msgLevel]++;

		return str;
	}

	// sets log line pattern for the specified by parameter 'll' log line
	void SetDefaultPattern(Levels::LogLevel ll) override
	{
		std::lock_guard<Mutex> lock(FMutex);
		FLayout->SetDefaultPattern(ll);
	}

	// sets log line pattern for all log lines
	void SetDefaultPattern() override
	{
		std::lock_guard<Mutex> lock(FMutex);
		FLayout->SetDefaultPattern();
	}

	/**
	* Returns bytes written since creating the sink.
	* @returns bytes written as value of size_t type.
	**/
	ullong GetBytesWritten() const override
	{
		return FBytesWritten;
	}
	/**
	* Returns message statistic. Message statistic it is number of written messages per log level.
	* @returns array of size_t valuesm one value for each log level.
	**/
	ullong* GetMessageCounts() override
	{
		return FMessageCounts;
	}

	/*Levels::LogLevel GetLogLevel() const { return FLogLevel; }
	void SetLogLevel(Levels::LogLevel ll)
	{
		std::lock_guard<Mutex> lock(FMutex);
		FLogLevel = ll;
	}

	/**
	 * Get the name of this sink. The name identifies the sink.
	 * @returns the name of the sink.
	 **/
	//const std::string& GetName() const { return FName; }
	
};

LOGENGINE_NS_END
