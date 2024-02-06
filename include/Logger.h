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
#include "SynchronizedQueue.h"

LOGENGINE_NS_BEGIN

class Logger;

typedef SafeQueue<LogEvent*> LoggerQueue;
struct LoggerThreadInfo
{
	LoggerQueue* queue;
	Logger* logger;
};

class Logger
{
private:
	Levels::LogLevel FLogLevel;
	std::string FName;
	LoggerQueue FQueue{1000};
	std::thread FThread;
	bool FAsync = false;
	THArray<Sink*> sinks;
	bool shouldLog(const Levels::LogLevel ll) const { return FLogLevel >= ll; }
	void InternalLog(const LogEvent& le) { SendToAllSinks(le); }

public:
	Logger(const std::string& name, Levels::LogLevel ll = Levels::llInfo) : FLogLevel(ll), FName(name) { }

	virtual ~Logger() 
	{ 
		SetAsyncMode(false); // send stop to async thread and wait till it finishes.  
		for (uint i = 0; i < sinks.Count(); i++) delete sinks[i]; 
		sinks.Clear(); 
	}

	void SetLogLevel(const Levels::LogLevel ll) { FLogLevel = ll; }

	void SetAsyncMode(bool amode)
	{
		if (FAsync == amode) return; //mode hasn't changed

		if (amode == true)
		{
			LoggerThreadInfo* info = new LoggerThreadInfo;
			info->queue = &FQueue;
			info->logger = this;

			std::thread thr(ThreadProc, info);
			FThread.swap(thr);
			FAsync = amode;
		}
		else
		{
			FAsync = amode; // stop adding new log messages
			FQueue.PushElement(nullptr);
			FThread.join(); // waiting till thread finishes
		}

	}

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
		InternalLog(ev);
	}
#endif

	void Crit(const std::string& msg)  { Log(msg, Levels::llCritical); }
	void Error(const std::string& msg) { Log(msg, Levels::llError); }
	void Warn(const std::string& msg)  { Log(msg, Levels::llWarning);}
	void Info(const std::string& msg)  { Log(msg, Levels::llInfo); }
	void Debug(const std::string& msg) { Log(msg, Levels::llDebug); }
	void Trace(const std::string& msg) { Log(msg, Levels::llTrace); }

	void Log(const std::string& msg, const Levels::LogLevel lv)
	{
		if (!shouldLog(lv)) return;

		if (FAsync)
		{
			LogEvent* event = new LogEvent(msg, lv, GetThreadID(), GetCurrDateTime());
			FQueue.PushElement(event);
		}
		else
		{
			LogEvent ev(msg, lv, GetThreadID(), GetCurrDateTime());
			InternalLog(ev);
		}
	}

	void SendToAllSinks(const LogEvent& le)
	{
		for (auto si : sinks)
		{
			si->PubSendMsg(le);
		}

		/*for (uint i = 0; i < sinks.Count(); i++)
		{
			sinks[i]->PubSendMsg(le);
		}*/
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

	Sink* GetSink(std::string& sinkName)
	{
		for(Sink* si : sinks)
		{
			if (si->GetName() == sinkName) return si;
		}

		return nullptr;
	}
	
	void WaitQueue()
	{
		FQueue.WaitEmptyQueue();
	}

	static int ThreadProc(void* parameter)
	{
		LoggerThreadInfo* info = (LoggerThreadInfo*)parameter;

		LogEvent* current_msg;
		do
		{
			current_msg = info->queue->WaitForElement();
			if (current_msg)
			{
				LogEvent* event = current_msg;
				info->logger->InternalLog(*event);
				delete event;
			}

		} while (current_msg); // null as msg means that we need to stop this thread

		delete info;

		return 0;
	}

};

LOGENGINE_NS_END


