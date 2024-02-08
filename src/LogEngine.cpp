
#include "Compare.h"
#include "LogEngine.h"

LOGENGINE_NS_BEGIN

static THash<std::string, Logger*, CompareStringNCase> loggers;

void ClearLoggers()
{
	for (uint i = 0; i < loggers.Count(); i++)
	{
		delete loggers.GetValues()[i];
	}

	loggers.Clear();
}

uint GetLoggersCount()
{
	return loggers.Count();
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


void InitFromFile(std::string fileName)
{
	IniReader reader;
	reader.LoadIniFile(fileName);

	for (auto it = reader.begin(); it != reader.end(); ++it)
	{
		std::string sectName = *it;
		IniReader::StorageType::ValuesHash& section = reader.GetSection(sectName);

		size_t n = sectName.find('.');
		if ((n != std::string::npos) && EqualNCase(sectName.substr(0, n + 1), LOGGER_PREFIX) )
		{
			std::string loggerName = sectName.substr(n + 1);
			if (loggerName.empty()) continue;


			Logger& logger = GetLogger(loggerName);
			logger.SetLogLevel(LLfromString(reader.GetValue(sectName, LOGLEVEL_PARAM, LL_DEFAULT_NAME, 0)));
			logger.SetAsyncMode(StrToBool(reader.GetValue(sectName, ASYNMODE_PARAM, "false", 0)));

			if (!section.IfExists("Sink")) continue;

			IniReader::ValueType& sinks = section.GetValue("Sink");
			for (auto sn : sinks)
			{
				if (sn.empty()) continue;

				std::string sinkSectName = SINK_PREFIX;
				sinkSectName.append(sn);

				if (!reader.HasSection(sinkSectName))
					throw file_exception("Sink section '" + sinkSectName + "' refered by logger '" + loggerName + "' not found in file '" + fileName + "'.");

				std::string value = reader.GetValue(sinkSectName, TYPE_PARAM, ST_DEFAULT_NAME, 0);
				LogSinkType stype = STfromString(value);
				Sink* sink = nullptr;

				switch (stype)
				{
				case stStdout: sink = new StdoutSink(sn); break;
				case stStderr: sink = new StderrSink(sn); break;
				case stString: sink = new StringSink(sn); break;
				case stFile:
				case stRotatingFile:
				{
					std::string sinkFileName = reader.GetValue(sinkSectName, FILENAME_PARAM, "");
					if (sinkFileName.empty())
						throw file_exception("File sink '" + sn + "' missing FileName parameter.");

					if (stype == stFile)
						sink = new FileSink(sn, sinkFileName);
					else
						sink = new RotatingFileSink(sn, sinkFileName);


					break;
				}
				default:
					break;
				}

				sink->SetLogLevel(LLfromString(reader.GetValue(sinkSectName, LOGLEVEL_PARAM, LL_DEFAULT_NAME, 0)));
				PatternLayout* lay = new PatternLayout();
				lay->SetAllPatterns(reader.GetValue(sinkSectName, PATTERNALL_PARAM, DefaultLinePattern, 0));
				lay->SetCritPattern(reader.GetValue(sinkSectName, CRITPATTERN_PARAM, DefaultCritPattern, 0));
				lay->SetErrorPattern(reader.GetValue(sinkSectName, ERRORPATTERN_PARAM, DefaultErrorPattern, 0));
				lay->SetWarnPattern(reader.GetValue(sinkSectName, WARNPATTERN_PARAM, DefaultWarningPattern, 0));

				sink->SetLayout(lay);
				logger.AddSink(sink);
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
