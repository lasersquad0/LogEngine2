/*
 * LogEngine.h
 *
 * Copyright 2025, LogEngine2 Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef LOG_ENGINE_H
#define LOG_ENGINE_H

#include <string>
#include <functional>
#include "DynamicArrays.h"
#include "Common.h"
#include "Compare.h"
#include "LogEvent.h"
#include "Properties.h"
#include "Logger.h"
#include "RotatingFileSink.h"
#include "version.h"

LOGENGINE_NS_BEGIN

void SetProperty(const std::string& name, const std::string& value);
std::string GetProperty(const std::string& name, const std::string& defaultValue /*= ""*/);
bool PropertyExist(const std::string& name);

void ShutdownLoggers();
uint LoggersCount();
bool LoggerExist(const std::string& loggerName);

Logger& GetDefaultLogger();

Logger& GetLogger(const std::string& loggerName);

Logger& GetFileLogger(const std::string& loggerName, const std::string& fileName);

Logger& GetRotatingFileLogger(const std::string& loggerName, const std::string& fileName, ullong maxLogSize = DefaultMaxLogSize,
                        	LogRotatingStrategy strategy = DefaultRotatingStrategy, uint maxBackupIndex = DefaultMaxBackupIndex);

Logger& GetStdoutLogger(const std::string& loggerName);

Logger& GetStderrLogger(const std::string& loggerName);

Logger& GetMultiLogger(const std::string& loggerName, SinkList& sinks);
Logger& GetMultiLogger(const std::string& loggerName, std::initializer_list<std::shared_ptr<Sink>> list);

Logger& GetCallbackLogger(const std::string& loggerName, const CustomLogCallback& callback);

void Log(const std::string& msg, const Levels::LogLevel ll);

void Crit(const std::string& msg);
void Error(const std::string& msg);
void Warn(const std::string& msg);
void Info(const std::string& msg);
void Debug(const std::string& msg);
void Trace(const std::string& msg);

template<class... Args>
void LogFmt(Levels::LogLevel ll, const std::format_string<Args...> fmt, Args&&... args)
{
	GetDefaultLogger().LogFmt(ll, fmt, args...);
}

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


// parameters for loggers
#define LOGGER_PREFIX  "logger."
#define SINK_PREFIX	   "sink."
#define LOGLEVEL_PARAM "loglevel"
#define ASYNMODE_PARAM "asyncmode"
#define SINK_PARAM "sink"

//parameters for Sinks (FileSink, RotatingFileSink, StdoutSink, StderrSink, StringSink)
#define TYPE_PARAM     "type"
#define FILENAME_PARAM "filename"
#define MAXLOGSIZE_PARAM "maxlogfilesize"
#define MAXBACKUPINDEX_PARAM "maxbackupindex"
#define STRATEGY_PARAM "strategy"
#define PATTERNALL_PARAM   "patternall"
#define CRITPATTERN_PARAM  "critpattern"
#define ERRORPATTERN_PARAM "errorpattern"
#define WARNPATTERN_PARAM  "warnpattern"
#define INFOPATTERN_PARAM  "infopattern"
#define DEBUGPATTERN_PARAM "debugpattern"
#define TRACEPATTERN_PARAM "tracepattern"

void InitFromFile(const std::string& fileName);

LOGENGINE_NS_END

#ifdef LOGENGINE_HEADER_ONLY
#include "LogEngine-hdr.h"
#endif

#endif //LOG_ENGINE_H

