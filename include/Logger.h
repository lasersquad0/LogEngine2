/*
 * Logger.h
 *
 * Copyright 2024, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#pragma once

#if defined(WIN32) && _HAS_CXX20==1 && !defined(__BORLANDC__)
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
	std::string FName;
	LoggerQueue FQueue;
	std::thread FThread;
	Levels::LogLevel FLogLevel;
	THArray<Sink*> sinks;
	bool FAsync = false;
	void InternalLog(const LogEvent& le) { SendToAllSinks(le); }

public:
	Logger(const std::string& name, Levels::LogLevel ll = LL_DEFAULT) : FName(name), FQueue(10), FLogLevel(ll) {}

	virtual ~Logger() 
	{ 
		SetAsyncMode(false); // send stop to async thread and wait till it finishes.  
		for (uint i = 0; i < sinks.Count(); i++) delete sinks[i]; 
		sinks.Clear(); 
	}

	bool ShouldLog(const Levels::LogLevel ll) const { return FLogLevel >= ll; }

	//TODO add parameter "propagate" with this parameter =true SetLogLevel will also set specified log level to all sinks
	void SetLogLevel(const Levels::LogLevel ll) { FLogLevel = ll; }
	Levels::LogLevel GetLogLevel() { return FLogLevel; }

	bool GetAsyncMode() { return FAsync; }
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

#if defined(WIN32) && _HAS_CXX20==1 && !defined(__BORLANDC__)
	template<class ... Args>
	void CritFmt(const std::format_string<Args...> fmt, Args&& ...args)
	{
		LogFmt(Levels::llCritical, fmt, std::forward<Args>(args)...);
	}

	template<class ... Args>
	void ErrorFmt(const std::format_string<Args...> fmt, Args&& ...args)
	{
		LogFmt(Levels::llError, fmt, std::forward<Args>(args)...);
	}

	template<class ... Args>
	void WarnFmt(const std::format_string<Args...> fmt, Args&& ...args)
	{
		LogFmt(Levels::llWarning, fmt, std::forward<Args>(args)...);
	}

	template<class ... Args>
	void InfoFmt(const std::format_string<Args...> fmt, Args&& ...args)
	{
		LogFmt(Levels::llInfo, fmt, std::forward<Args>(args)...);
	}

	template<class ... Args>
	void DebugFmt(const std::format_string<Args...> fmt, Args&& ...args)
	{
		LogFmt(Levels::llDebug, fmt, std::forward<Args>(args)...);
	}

	template<class ... Args>
	void TraceFmt(const std::format_string<Args...> fmt, Args&& ...args)
	{
		LogFmt(Levels::llTrace, fmt, std::forward<Args>(args)...);
	}

	template<class... Args>
	void LogFmt(Levels::LogLevel ll, const std::format_string<Args...> fmt, Args&&... args)
	{
		if (!ShouldLog(ll)) return;

		// TODO think how to pass all args into SendToAllSinks and create final string there
		if (FAsync)
		{
			LogEvent* event = new LogEvent(std::vformat(fmt.get(), std::make_format_args(args...)), ll, GetThreadID(), GetCurrDateTime());
			FQueue.PushElement(event);
		}
		else
		{
			LogEvent ev(std::vformat(fmt.get(), std::make_format_args(args...)), ll, GetThreadID(), GetCurrDateTime());
			InternalLog(ev);
		}
	}
#else
	std::string vformat(const char* format, va_list args)
	{
		size_t size = 1024; // should be enough for format string
		char* buffer = new char[size];

		while (1)
		{
			va_list args_copy;

#if defined(_MSC_VER) || defined(__BORLANDC__)
			args_copy = args;
#else
			va_copy(args_copy, args);
#endif

			int n = vsnprintf(buffer, size, format, args_copy);

			va_end(args_copy);

			// If that worked, return a string.
			if ((n > -1) && (static_cast<size_t>(n) < size))
			{
				std::string s(buffer);
				delete[] buffer;
				return s;
			}

			// Else try again with more space.
			size = (n > -1) ?
				(size_t)(n + 1) :   // ISO/IEC 9899:1999
				size * 2; // twice the old size

			delete[] buffer;
			buffer = new char[size];
		}
	}
	
	void CritFmt(const char* fmt, ...)
	{
		if (!ShouldLog(Levels::llCritical)) return;

		va_list va;
		va_start(va, fmt);
		LogFmt(Levels::llCritical, fmt, va);
		va_end(va);
	}

	void ErrorFmt(const char* fmt, ...)
	{
		if (!ShouldLog(Levels::llError)) return;

		va_list va;
		va_start(va, fmt);
		LogFmt(Levels::llError, fmt, va);
		va_end(va);
	}

	void WarnFmt(const char* fmt, ...)
	{
		if (!ShouldLog(Levels::llWarning)) return;

		va_list va;
		va_start(va, fmt);
		LogFmt(Levels::llWarning, fmt, va);
		va_end(va);
	}

	void InfoFmt(const char* fmt, ...)
	{
		if (!ShouldLog(Levels::llInfo)) return;

		va_list va;
		va_start(va, fmt);
		LogFmt(Levels::llInfo, fmt, va);
		va_end(va);
	}

	void DebugFmt(const char* fmt, ...)
	{ 
		if (!ShouldLog(Levels::llDebug)) return;

		va_list va;
		va_start(va, fmt);
		LogFmt(Levels::llDebug, fmt, va);
		va_end(va);
	}

	void TraceFmt(const char* fmt, ...)
	{
		if (!ShouldLog(Levels::llTrace)) return;

		va_list va;
		va_start(va, fmt);
		LogFmt(Levels::llTrace, fmt, va);
		va_end(va);
	}

	void LogFmt(Levels::LogLevel ll, const char* fmt,  ...)
	{
		if (!ShouldLog(ll)) return;
		
		va_list va;
		va_start(va, fmt);

		// TODO think how to pass all args into SendToAllSinks and create final string there
		if (FAsync)
		{
			LogEvent* event = new LogEvent(vformat(fmt, va), ll, GetThreadID(), GetCurrDateTime());
			FQueue.PushElement(event);
		}
		else
		{
			LogEvent ev(vformat(fmt, va), ll, GetThreadID(), GetCurrDateTime());
			InternalLog(ev);
		}
		//LogEvent ev(vformat(fmt, va), ll, GetThreadID(), GetCurrDateTime());
		va_end(va);
	}
#endif

	void Crit (const std::string& msg) { Log(msg, Levels::llCritical); }
	void Error(const std::string& msg) { Log(msg, Levels::llError);    }
	void Warn (const std::string& msg) { Log(msg, Levels::llWarning);  }
	void Info (const std::string& msg) { Log(msg, Levels::llInfo);     }
	void Debug(const std::string& msg) { Log(msg, Levels::llDebug);    }
	void Trace(const std::string& msg) { Log(msg, Levels::llTrace);    }

	void Log(const std::string& msg, const Levels::LogLevel ll)
	{
		if (!ShouldLog(ll)) return;

		if (FAsync)
		{
			LogEvent* event = new LogEvent(msg, ll, GetThreadID(), GetCurrDateTime());
			FQueue.PushElement(event);
		}
		else
		{
			LogEvent ev(msg, ll, GetThreadID(), GetCurrDateTime());
			InternalLog(ev);
		}
	}

	void SendToAllSinks(const LogEvent& le)
	{
		for (Sink* si : sinks)
		{
			si->PubSendMsg(le);
		}

		//for (uint i = 0; i < sinks.Count(); i++)
		//{
		//	sinks[i]->PubSendMsg(le);
		//}
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
			if (EqualNCase(sinks[i]->GetName(), sinkName)) sinks.DeleteValue(i);
		}
	}

	uint SinkCount()
	{
		return sinks.Count();
	}

	Sink* GetSink(const std::string& sinkName)
	{
		for(Sink* si : sinks)
		{
			if (EqualNCase(si->GetName(), sinkName)) return si;
		}

		//for (uint i = 0; i < sinks.Count(); i++)
		//{
		//	if (sinks[i]->GetName() == sinkName) return sinks[i];
		//}

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


