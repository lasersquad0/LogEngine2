/*
 * Logger.h
 *
 * Copyright 2024, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#pragma once

#include <string>
#include <format>
#include "Common.h"
#include "FileSink.h"
#include "DynamicArrays.h"

LOGENGINE_NS_BEGIN

class Logger
{
private:
	Levels::LogLevel FLogLevel;
	std::string FName;
	THash<std::string, Sink*> sinks;
	bool shouldLog(Levels::LogLevel ll) { return FLogLevel >= ll; }

public:
	Logger(const std::string& name, Levels::LogLevel ll = Levels::llInfo) : FName(name), FLogLevel(ll) { }

	virtual ~Logger() { for (uint i = 0; i < sinks.Count(); i++) delete sinks.GetValues()[i]; sinks.Clear(); }

	void SetLogLevel(Levels::LogLevel ll) { FLogLevel = ll; }

	template<class ... Args>
	void CritFmt(const std::format_string<Args...> fmt, Args&& ...args)
	{
		LogFmt(fmt, Levels::llCritical, std::forward<Args>(args)...);
	}

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

	template<class... Args>
	void LogFmt(const std::format_string<Args...> fmt, Levels::LogLevel ll, Args&&... args)
	{
		if (!shouldLog(ll)) return;

		// TODO think how to pass all args into SendToAllSinks and create final string there 
		LogEvent ev(std::vformat(fmt.get(), std::make_format_args(args...)), ll, GetThreadID(), GetCurrDateTime());
		SendToAllSinks(ev);
	}

	void SendToAllSinks(LogEvent& le)
	{
		for (uint i = 0; i < sinks.Count(); i++)
		{
			sinks.GetValues()[i]->PubSendMsg(le);
		}
	}

	void AddSink(Sink* sink)
	{
		sinks.SetValue(sink->GetName(), sink); // if sink with the same name already exist - overwrite it
	}

	void RemoveSink(std::string& sinkName)
	{
		if (sinks.IfExists(sinkName))
			sinks.Delete(sinkName);
		// if sinkName is not found in sinks - do nothing
	}

};

LOGENGINE_NS_END

