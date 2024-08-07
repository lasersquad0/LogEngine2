/*
 * LogEngine.h
 *
 * Copyright 2023, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef LOG_ENGINE_H
#define LOG_ENGINE_H

#include <string>
#include "Logger.h"

LOGENGINE_NS_BEGIN

#define APPNAME_PROPERTY  "AppName"
#define APPVERSION_PROPERTY "AppVersion"

void SetProperty(const std::string& name, const std::string& value);
std::string GetProperty(const std::string& name, const std::string& defaultValue = "");
bool PropertyExist(const std::string& name);

void ClearLoggers();
uint GetLoggersCount();

Logger& GetLogger(const std::string& loggerName);

Logger& GetFileLogger(const std::string& loggerName, const std::string fileName);

Logger& GetStdoutLogger(const std::string& loggerName);

Logger& GetStderrLogger(const std::string& loggerName);

Logger& GetMultiLogger(const std::string& loggerName, THArray<Sink*> sinks);

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

#endif //LOG_ENGINE_H
