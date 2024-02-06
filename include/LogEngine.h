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
#include "FileSink.h"


LOGENGINE_NS_BEGIN

void ClearLoggers();

Logger& GetLogger(const std::string& loggerName);

Logger& GetFileLogger(const std::string& loggerName, const std::string fileName);

Logger& GetStdoutLogger(const std::string& loggerName);

Logger& GetStderrLogger(const std::string& loggerName);

Logger& GetMultiLogger(const std::string& loggerName, THArray<Sink*> sinks);

LOGENGINE_NS_END

#endif //LOG_ENGINE_H
