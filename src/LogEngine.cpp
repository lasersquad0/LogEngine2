
#include "LogEngine.h"

LOGENGINE_NS_BEGIN

static THash<std::string, Logger*> loggers;

void ClearLoggers()
{
	for (uint i = 0; i < loggers.Count(); i++)
	{
		delete loggers.GetValues()[i];
	}

	loggers.Clear();
}

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
	Logger& logger = GetLogger(loggerName); // TODO what if second logger has the same logger name, but different file name???
	if (logger.SinkCount() > 0) return logger; // this is pre-existed logger

	Sink* sink = new FileSink(loggerName, fileName);
	logger.AddSink(sink);

	return logger;
}

Logger& GetStdoutLogger(const std::string& loggerName)
{
	Logger& logger = GetLogger(loggerName);
	if (logger.SinkCount() > 0) return logger; // this is existed logger

	Sink* sink = new StdoutSink(loggerName);
	logger.AddSink(sink);
	return logger;
}

Logger& GetStderrLogger(const std::string& loggerName)
{
	Logger& logger = GetLogger(loggerName);
	if (logger.SinkCount() > 0) return logger; // this is pre-existed logger

	Sink* sink = new StderrSink(loggerName);
	logger.AddSink(sink);
	return logger;
}

Logger& GetMultiLogger(const std::string& loggerName, THArray<Sink*> sinks)
{
	Logger& logger = GetLogger(loggerName); // TODO what is pre-existed logger with sinks has returned???

	for (uint i = 0; i < sinks.Count(); i++)
		logger.AddSink(sinks[i]);

	return logger;
}

LOGENGINE_NS_END
