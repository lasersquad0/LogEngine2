/*
 * LogEngine-hdr.h
 *
 * Copyright 2025, LogEngine2 Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#include "RotatingFileSink.h"
#include "IniReader.h"
//#include "LogEngine.h"

LOGENGINE_NS_BEGIN

/*class Destructor
{
public:
	~Destructor() { ShutdownLoggers(); }
};*/

class Registry
{
private:
	// logger names are case INsensitive
	THash<std::string, Logger*, CompareStringNCase> FLoggers;

	Registry() 
	{
		// adding default logger
		Logger* logger = new Logger("");
		FLoggers.SetValue("", logger);
		std::shared_ptr<StdoutSink> sink(new StdoutSink("_stdout_"));
		logger->AddSink(sink);
	}

	~Registry() { Shutdown(); }
public:
	Registry(const Registry&) = delete;
	Registry& operator=(const Registry&) = delete;

	void Shutdown()
	{
		for (uint i = 0; i < FLoggers.Count(); i++)
			delete FLoggers.GetValues()[i];

		FLoggers.ClearMem();
	}

	uint Count() { return FLoggers.Count(); }
	bool IfExists(const std::string& loggerName) { return FLoggers.IfExists(loggerName); }

	Logger& GetDefaultLogger()
	{
		return *FLoggers.GetValue("");
	}

	void SetDefaultLogger(Logger& logger)
	{
		FLoggers.SetValue("", &logger);
	}

	Logger& RegisterLogger(const std::string& loggerName)
	{
		// Logger name can be empty for default logger only
		// other loggers must have non-empty name
		if (loggerName.size() == 0) throw LogException("Logger name cannot be empty.");

		Logger** loggerPtr = FLoggers.GetValuePointer(loggerName);
		if (loggerPtr == nullptr)
		{
			Logger* logger = new Logger(loggerName);
			FLoggers.SetValue(loggerName, logger);
			return *logger;
		}
		else
		{
			return **loggerPtr;
		}
	}

	//void InitializeLogger(Logger& newLogger);

	static Registry& Instance()
	{
		static Registry FInstance;
		return FInstance;
	}
};

static Properties properties;
//static Destructor destructor; // variable destructor must be located BELOW variable loggers because loggers should exist when destructor is being destroyed.

LOGENGINE_INLINE void SetProperty(const std::string& name, const std::string& value)
{
	properties.SetValue(name, value);
}

LOGENGINE_INLINE std::string GetProperty(const std::string& name, const std::string& defaultValue)
{
	return properties.getString(name, defaultValue);
}

LOGENGINE_INLINE bool PropertyExist(const std::string& name)
{
	return properties.IfExists(name);
}

LOGENGINE_INLINE void ShutdownLoggers()
{
	Registry::Instance().Shutdown();
}

LOGENGINE_INLINE uint LoggersCount()
{
	return Registry::Instance().Count();
}

LOGENGINE_INLINE bool LoggerExist(const std::string& loggerName)
{
	return Registry::Instance().IfExists(loggerName);
}

LOGENGINE_INLINE Logger& GetDefaultLogger()
{
	return Registry::Instance().GetDefaultLogger();
}

// returns reference to the logger with name specified in loggerName parameter
// creates new "empty" logger in case if logger with specified name does not yet exists
// "empty" means that the logger does not contain any Sinks.
LOGENGINE_INLINE Logger& GetLogger(const std::string& loggerName)
{
	return Registry::Instance().RegisterLogger(loggerName);
}

// returns reference to the file logger with name specified in loggerName parameter
// if logger with specified name does not exist new logger is created and one FileSink is added to thie logger
LOGENGINE_INLINE Logger& GetFileLogger(const std::string& loggerName, const std::string& fileName)
{
	Logger& logger = GetLogger(loggerName); // TODO what if second logger has the same logger name, but different file name???
	if (logger.SinkCount() > 0) return logger; // this is pre-existed logger

	auto sink = std::make_shared<FileSink>(loggerName, fileName); //TODO may be use file name as Sink name instead of logger name?
	logger.AddSink(sink);

	return logger;
}

LOGENGINE_INLINE Logger& GetRotatingFileLogger(const std::string& loggerName, const std::string& fileName, ullong maxLogSize, LogRotatingStrategy strategy, uint maxBackupIndex)
{
	Logger& logger = GetLogger(loggerName); // TODO what if second logger has the same logger name, but different file name???
	if (logger.SinkCount() > 0) return logger; // this is pre-existed logger

	auto sink = std::make_shared<RotatingFileSink>(loggerName, fileName, maxLogSize, strategy, maxBackupIndex); //TODO may be use file name as Sink name instead of logger name?
	logger.AddSink(sink);

	return logger;
}

LOGENGINE_INLINE Logger& GetStdoutLogger(const std::string& loggerName)
{
	Logger& logger = GetLogger(loggerName); //TODO what to do when logger with the same name but another type (e.g. FileLogger) already exists. 
	if (logger.SinkCount() > 0) return logger; // this is existed logger

	auto sink = std::make_shared<StdoutSink>(loggerName);
	logger.AddSink(sink);
	return logger;
}

LOGENGINE_INLINE Logger& GetStderrLogger(const std::string& loggerName)
{
	Logger& logger = GetLogger(loggerName);
	if (logger.SinkCount() > 0) return logger; // this is pre-existed logger

	auto sink = std::make_shared<StderrSink>(loggerName);
	logger.AddSink(sink);
	return logger;
}

LOGENGINE_INLINE Logger& GetMultiLogger(const std::string& loggerName, SinkList& sinks)
{
	Logger& logger = GetLogger(loggerName);
	logger.ClearSinks();
	for (uint i = 0; i < sinks.Count(); i++)
		logger.AddSink(sinks[i]);

	return logger;
}

LOGENGINE_INLINE Logger& GetMultiLogger(const std::string& loggerName, std::initializer_list<std::shared_ptr<Sink>> list)
{
	Logger& logger = GetLogger(loggerName); // TODO what is pre-existed logger with sinks has returned???
	logger.ClearSinks();
	for (auto sink: list)
		logger.AddSink(sink);

	return logger;
}

LOGENGINE_INLINE Logger& GetCallbackLogger(const std::string& loggerName, const CustomLogCallback& callback)
{
	Logger& logger = GetLogger(loggerName);
	if (logger.SinkCount() > 0) return logger; // this is pre-existed logger

	auto sink = std::make_shared<CallbackSink>(loggerName, callback);
	logger.AddSink(sink);
	return logger;
}

void Log(const std::string& msg, const Levels::LogLevel ll)
{
	Registry::Instance().GetDefaultLogger().Log(msg, ll);
}

void Crit(const std::string& msg) { Log(msg, Levels::llCritical); }
void Error(const std::string& msg) { Log(msg, Levels::llError); }
void Warn(const std::string& msg) { Log(msg, Levels::llWarning); }
void Info(const std::string& msg) { Log(msg, Levels::llInfo); }
void Debug(const std::string& msg) { Log(msg, Levels::llDebug); }
void Trace(const std::string& msg) { Log(msg, Levels::llTrace); }


static uint ParseInt(std::string s, uint defaultValue = 0)
{
	if (s.empty()) return defaultValue;

	int factor = 1;
	std::string sl = StrToLower(s);

	if (sl.find_last_of('k') != std::string::npos)
	{
		sl = sl.substr(0, sl.length() - 1);
		factor = 1024;
	}
	else if (sl.find_last_of('m') != std::string::npos)
	{
		sl = sl.substr(0, sl.length() - 1);
		factor = 1024 * 1024;
	}
	else if (sl.find_last_of('g') != std::string::npos)
	{
		sl = sl.substr(0, sl.length() - 1);
		factor = 1024 * 1024 * 1024;
	}

	try
	{
		uint result = static_cast<uint>(stoi(sl) * factor);
		return result;
	}
	catch (...)
	{
		return defaultValue;
	}
}

LOGENGINE_INLINE void InitFromFile(const std::string& fileName)
{
	IniReader reader;
	reader.LoadIniFile(fileName);

	for (auto it = reader.begin(); it != reader.end(); ++it)
	{
		std::string sectName = *it;
		IniReader::StorageType::ValuesHash& section = reader.GetSection(sectName);

		size_t n = sectName.find('.');
		if ((n != std::string::npos) && EqualNCase<std::string>(sectName.substr(0, n + 1), LOGGER_PREFIX) )
		{
			std::string loggerName = sectName.substr(n + 1);
			if (loggerName.empty()) continue;


			Logger& logger = GetLogger(loggerName);
			logger.SetLogLevel(LLfromString(reader.GetValue(sectName, LOGLEVEL_PARAM, LL_DEFAULT_NAME, 0)));
			logger.SetAsyncMode(StrToBool(reader.GetValue(sectName, ASYNMODE_PARAM, "false", 0)));

			if (!section.IfExists(SINK_PARAM)) continue;

			IniReader::ValueType& sinks = section.GetValue(SINK_PARAM);
			for (auto sn : sinks)
			{
				if (sn.empty()) continue;

				std::string sinkSectName = SINK_PREFIX;
				sinkSectName.append(sn);

				if (!reader.HasSection(sinkSectName))
					throw FileException("Sink section '" + sinkSectName + "' refered by logger '" + loggerName + "' not found in file '" + fileName + "'.");

				std::string value = reader.GetValue(sinkSectName, TYPE_PARAM, ST_DEFAULT_NAME, 0);
				LogSinkType stype = STfromString(value);
				Sink* sink = nullptr;

				switch (stype)
				{
				case stStdout: sink = new StdoutSink(sn); break;
				case stStderr: sink = new StderrSink(sn); break;
				case stString: sink = new StringSink(sn); break;
				case stFile:
				{
					std::string sinkFileName = reader.GetValue(sinkSectName, FILENAME_PARAM, "");
					if (sinkFileName.empty())
						throw FileException("File sink '" + sn + "' missing FileName parameter.");

					sink = new FileSink(sn, sinkFileName);
					
					break;
				}
				case stRotatingFile:
				{
					std::string sinkFileName = reader.GetValue(sinkSectName, FILENAME_PARAM, "");
					if (sinkFileName.empty())
						throw FileException("File sink '" + sn + "' missing FileName parameter.");
					
					LogRotatingStrategy strategy = RSfromString(reader.GetValue(sinkSectName, STRATEGY_PARAM, RS_DEFAULT_NAME));
					uint maxlogsize = ParseInt(reader.GetValue(sinkSectName, MAXLOGSIZE_PARAM, DefaultMaxLogSizeStr), DefaultMaxLogSize);
					uint maxIndex = ParseInt(reader.GetValue(sinkSectName, MAXBACKUPINDEX_PARAM, DefaultMaxBackupIndexStr), DefaultMaxBackupIndex);

					sink = new RotatingFileSink(sn, sinkFileName, maxlogsize, strategy, maxIndex);

					break;
				}
				default:
					break;
				}

				sink->SetLogLevel(LLfromString(reader.GetValue(sinkSectName, LOGLEVEL_PARAM, LL_DEFAULT_NAME, 0)));
				PatternLayout* lay = new PatternLayout();
				lay->SetPattern(reader.GetValue(sinkSectName, PATTERNALL_PARAM, DefaultLinePattern, 0));  
				if (reader.HasValue(sinkSectName, CRITPATTERN_PARAM))  lay->SetCritPattern(reader.GetValue(sinkSectName,  CRITPATTERN_PARAM));
				if (reader.HasValue(sinkSectName, ERRORPATTERN_PARAM)) lay->SetErrorPattern(reader.GetValue(sinkSectName, ERRORPATTERN_PARAM));
				if (reader.HasValue(sinkSectName, WARNPATTERN_PARAM))  lay->SetWarnPattern(reader.GetValue(sinkSectName,  WARNPATTERN_PARAM));
				if (reader.HasValue(sinkSectName, INFOPATTERN_PARAM))  lay->SetInfoPattern(reader.GetValue(sinkSectName,  INFOPATTERN_PARAM));
				if (reader.HasValue(sinkSectName, DEBUGPATTERN_PARAM)) lay->SetDebugPattern(reader.GetValue(sinkSectName, DEBUGPATTERN_PARAM));
				if (reader.HasValue(sinkSectName, TRACEPATTERN_PARAM)) lay->SetTracePattern(reader.GetValue(sinkSectName, TRACEPATTERN_PARAM));

				sink->SetLayout(lay);
				logger.AddSink(std::shared_ptr<Sink>(sink));
			}
		}
	}
}

/*
static Sink* SinkByName(THArray<Sink*>& sinks, std::string& sinkName)
{
	for (Sink* si: sinks)
	{
		if (EqualNCase(si->GetName(), sinkName)) return si;
	}
	return nullptr;
}

static Sink* SinkByName(THArray<std::pair<Sink*, bool>>& sinks, std::string& sinkName)
{
	for (auto si : sinks)
	{
		if (EqualNCase(si.first->GetName(), sinkName))
		{
			si.second = true; // mark sink as "visited"
			return si.first;
		}
	}
	return nullptr;
}

void InitFromFile(std::string fileName)
{
	IniReader reader;
	reader.LoadIniFile(fileName);
	
	THArray<std::pair<Sink*, bool>> sinks;

	// look for all sink sections, create and initialise appropriate Sinks. Put them in sinks array for further processing.
	for (auto it = reader.begin(); it != reader.end(); ++it)
	{
		std::string sectName = *it;
		//IniReader::StorageType::ValuesHash& section = reader.GetSection(sectName);

		size_t n = sectName.find('.');
		if ((n != std::string::npos) && EqualNCase(sectName.substr(0, n + 1), SINK_PREFIX) )
		{
			std::string sinkName = sectName.substr(n + 1);

			if (sinkName.size() == 0) continue;

			std::string value = reader.GetValue(sectName, TYPE_PARAM, ST_DEFAULT_NAME, 0);
			LogSinkType stype = STfromString(value);
			Sink* sink = nullptr;

			switch (stype)
			{
			case stStdout: sink = new StdoutSink(sinkName); break;
			case stStderr: sink = new StderrSink(sinkName); break;
			case stString: sink = new StringSink(sinkName); break;
			case stFile:
			case stRotatingFile:
			{
				std::string sinkFileName = reader.GetValue(sectName, FILENAME_PARAM, "");
				if (fileName.empty())
					throw file_exception("File sink '" + sinkName + "' missing FileName parameter.");

				if (stype == stFile)
					sink = new FileSink(sinkName, sinkFileName);
				else
					sink = new RotatingFileSink(sinkName, sinkFileName);

				break;
			}
			default:
				throw file_exception("Unknown sink type for sink '" + sinkName);
			}

			sink->SetLogLevel(LLfromString(reader.GetValue(sectName, LOGLEVEL_PARAM, LL_DEFAULT_NAME, 0)));

			PatternLayout* lay = new PatternLayout();
			lay->SetAllPatterns(reader.GetValue(sectName, PATTERNALL_PARAM, DefaultLinePattern, 0));
			lay->SetCritPattern(reader.GetValue(sectName, CRITPATTERN_PARAM, DefaultCritPattern, 0));
			lay->SetErrorPattern(reader.GetValue(sectName, ERRORPATTERN_PARAM, DefaultErrorPattern, 0));
			lay->SetWarnPattern(reader.GetValue(sectName, WARNPATTERN_PARAM, DefaultWarningPattern, 0));

			sink->SetLayout(lay);
			sinks.AddValue(std::pair(sink, false));

		}
	}

	// look init file again for logger sections. create a initialize apprioriate loggers, assign previously created sinks to loggers
	for (auto it = reader.begin(); it != reader.end(); ++it)
	{
		std::string sectName = *it;
		IniReader::StorageType::ValuesHash& section = reader.GetSection(sectName);

		size_t n = sectName.find('.');
		if ((n != std::string::npos) && (CompareNCase(sectName.substr(0, n + 1), LOGGER_PREFIX) == 0))
		{
			std::string loggerName = sectName.substr(n + 1);
			
			if (loggerName.size() == 0) continue;
			
			Logger& logger = GetLogger(loggerName);
			logger.SetLogLevel(LLfromString(reader.GetValue(sectName, LOGLEVEL_PARAM, LL_DEFAULT_NAME, 0)));
			logger.SetAsyncMode(StrToBool(reader.GetValue(sectName, ASYNMODE_PARAM, "false", 0)));

			if (!section.IfExists("Sink")) continue;

			IniReader::ValueType& loggerSinks = section.GetValue("Sink");
			for (auto sn : loggerSinks)
			{
				if (sn.empty()) continue;

				Sink* sink = SinkByName(sinks, sn);
				if(sink == nullptr)
					throw file_exception("Sink '" + sn + "' refered by logger '" + loggerName + "' is not found in file '" + fileName + "'.");
				logger.AddSink(sink);
			}
		}
	}

	// delete 'unused' sinks
	//for (auto si : sinks)
	//{
	//	if (!si.second)	delete si.first;
	//}

}
*/

LOGENGINE_NS_END
