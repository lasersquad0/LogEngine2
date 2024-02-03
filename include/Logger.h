/*
 * Logger.h
 *
 * Copyright 2024, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#pragma once

#if defined(WIN32) && !defined(__BORLANDC__)
#include <format>
#endif

#include <string>
#include "Common.h"
#include "FileSink.h"
#include "DynamicArrays.h"

LOGENGINE_NS_BEGIN

class Logger
{
private:
	Levels::LogLevel FLogLevel;
	std::string FName;
	//THash<std::string, Sink*> sinks;
	THArray<Sink*> sinks;
	bool shouldLog(Levels::LogLevel ll) { return FLogLevel >= ll; }

public:
	Logger(const std::string& name, Levels::LogLevel ll = Levels::llInfo) : FLogLevel(ll), FName(name) { }

	virtual ~Logger() { for (uint i = 0; i < sinks.Count(); i++) delete sinks[i]; sinks.Clear(); }

	void SetLogLevel(Levels::LogLevel ll) { FLogLevel = ll; }

#if defined(WIN32) && !defined(__BORLANDC__)
	template<class ... Args>
	void CritFmt(const std::format_string<Args...> fmt, Args&& ...args)
	{
		LogFmt(fmt, Levels::llCritical, std::forward<Args>(args)...);
	}

	template<class... Args>
	void LogFmt(const std::format_string<Args...> fmt, Levels::LogLevel ll, Args&&... args)
	{
		if (!shouldLog(ll)) return;

		// TODO think how to pass all args into SendToAllSinks and create final string there 
		LogEvent ev(std::vformat(fmt.get(), std::make_format_args(args...)), ll, GetThreadID(), GetCurrDateTime());
		SendToAllSinks(ev);
	}
#endif

	void Crit(const std::string& msg)
	{
		Log(msg, Levels::llCritical);
	}

	void Error(const std::string& msg)
	{
		Log(msg, Levels::llError);
	}

	void Warn(const std::string& msg)
	{
		Log(msg, Levels::llWarning);
	}

	void Info(const std::string& msg)
	{
		Log(msg, Levels::llInfo);
	}

	void Debug(const std::string& msg)
	{
		Log(msg, Levels::llDebug);
	}

	void Trace(const std::string& msg)
	{
		Log(msg, Levels::llTrace);
	}

	void Log(const std::string& msg, Levels::LogLevel lv)
	{
		if (!shouldLog(lv)) return;

		LogEvent ev(msg, lv, GetThreadID(), GetCurrDateTime());
		SendToAllSinks(ev);
	}

	void SendToAllSinks(LogEvent& le)
	{
		for (uint i = 0; i < sinks.Count(); i++)
		{
			sinks[i]->PubSendMsg(le);
		}
	}

	void AddSink(Sink* sink)
	{
		if (sinks.IndexOf(sink) >= 0) return; // already added
		sinks.AddValue(sink);
	}

	void RemoveSink(std::string& sinkName)
	{
		for (uint i = 0; i < sinks.Count(); i++)
		{
			if (sinks[i]->GetName() == sinkName) sinks.DeleteValue(i);
		}
	}

	uint SinkCount()
	{
		return sinks.Count();
	}

};

LOGENGINE_NS_END


