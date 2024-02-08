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
#include "RotatingFileSink.h"
#include "IniReader.h"

LOGENGINE_NS_BEGIN

void ClearLoggers();
uint GetLoggersCount();

Logger& GetLogger(const std::string& loggerName);

Logger& GetFileLogger(const std::string& loggerName, const std::string fileName);

Logger& GetStdoutLogger(const std::string& loggerName);

Logger& GetStderrLogger(const std::string& loggerName);

Logger& GetMultiLogger(const std::string& loggerName, THArray<Sink*> sinks);

#define LOGGER_PREFIX  "logger."
#define SINK_PREFIX	   "sink."
#define LOGLEVEL_PARAM "loglevel"
#define ASYNMODE_PARAM "asyncmode"
#define TYPE_PARAM     "type"
#define FILENAME_PARAM "filename"
#define PATTERNALL_PARAM   "patternall"
#define CRITPATTERN_PARAM  "critpattern"
#define ERRORPATTERN_PARAM "errorpattern"
#define WARNPATTERN_PARAM  "warnpattern"
#define INFOPATTERN_PARAM  "infopattern"

void InitFromFile(std::string fileName);


LOGENGINE_NS_END

#endif //LOG_ENGINE_H
