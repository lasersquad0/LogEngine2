/*
 * LogEngine.h
 *
 * Copyright 2023, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef _LOG_ENGINE_H_
#define _LOG_ENGINE_H_

#include <string>
//#include "SynchronizedQueue.h"
#include "Logger.h"
#include "FileSink.h"


LOGENGINE_NS_BEGIN
	

//typedef SynchronizedQueue<LogEvent*> LogEventQueue;
//	typedef SafeQueue<LogEvent*> LogEventQueue;

//struct LogEngineThreadInfo
//{
//	LogEventQueue* LogQueue;
//	TLogEngine* LogEngine;
//};


////////////////////////////////////////////////
static THash<std::string, Logger*> loggers;

Logger& GetLogger(const std::string& loggerName)
{
	Logger** loggerPtr = loggers.GetValuePointer(loggerName);
	if (loggerPtr == nullptr)
	{
		Logger* logger = new Logger(loggerName);
		loggers.SetValue(loggerName, logger);
		return *logger;
	}
	else
	{
		return **loggerPtr;
	}
}

Logger& GetFileLogger(const std::string& loggerName, const std::string fileName)
{
	Logger& logger = GetLogger(loggerName);
	Sink* sink = new FileSink(loggerName, fileName);
	logger.AddSink(sink);
	return logger;
}

Logger& GetStdoutLogger(const std::string& loggerName)
{
	Logger& logger = GetLogger(loggerName);
	Sink* sink = new StdoutSink(loggerName);
	logger.AddSink(sink);
	return logger;
}

Logger& GetStderrLogger(const std::string& loggerName)
{
	Logger& logger = GetLogger(loggerName);
	Sink* sink = new StderrSink(loggerName);
	logger.AddSink(sink);
	return logger;
}

Logger& GetMultiLogger(const std::string& loggerName, THArray<Sink*> sinks)
{
	Logger& logger = GetLogger(loggerName);

	for (uint i = 0; i < sinks.Count(); i++)
		logger.AddSink(sinks[i]);

	return logger;
}


LOGENGINE_NS_END

#endif //_LOG_ENGINE_H_
