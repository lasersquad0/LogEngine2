/*
 * LogEvent.h
 *
 * Copyright 2024, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#if !defined(LOGEVENT_H)
#define LOGEVENT_H

//#include <string_view>
#include <sys/timeb.h>
#include "Common.h"

LOGENGINE_NS_BEGIN

namespace Levels
{
	enum LogLevel:int { llOff, llCritical, llError, llWarning, llInfo, llDebug, llTrace, n_LogLevels };
}

#define LL_NAMES       { "off", "critical", "error", "warning", "info", "debug", "trace" }
#define LL_CAPS_NAMES  { "OFF", "CRITICAL", "ERROR", "WARNING", "INFO", "DEBUG", "TRACE" }
#define LL_SHORT_NAMES { "O", "C", "E", "W", "I", "D", "T" }

static const std::string LogLevelNames[] LL_NAMES;
static const std::string LogLevelCapsNames[] LL_CAPS_NAMES;
static const char* LogLevelShortNames[] LL_SHORT_NAMES;

inline const std::string& LLtoString(const Levels::LogLevel ll) { return LogLevelNames[ll]; }
inline const std::string& LLtoCapsString(const Levels::LogLevel ll) { return LogLevelCapsNames[ll]; }

inline const char* LLtoShortString(const Levels::LogLevel ll) { return LogLevelShortNames[ll]; }

inline Levels::LogLevel LLfromString(const std::string& name)
{
	auto it = std::find(std::begin(LogLevelNames), std::end(LogLevelNames), name);
	if (it != std::end(LogLevelNames))
		return static_cast<Levels::LogLevel>(std::distance(std::begin(LogLevelNames), it));

	// check also for "warn" and "err" before giving up..
	if (name == "warn") return Levels::llWarning;
	if (name == "err") return Levels::llError;

	return Levels::llOff;
}

class LogEvent
{
public:
	Levels::LogLevel msgLevel;
	unsigned int threadID; // this is a thread that generated a log message. it may differ from thread that makes actual writing to the file (in case LogEngone.Threaded property is set to true)
	struct tm tmtime;
	std::string message;

	LogEvent(const std::string& msg, Levels::LogLevel msgType, unsigned int thrID, struct tm time)
		: msgLevel(msgType), threadID(thrID), tmtime(time), message(msg) {}

};

LOGENGINE_NS_END

#endif //LOGEVENT_H
