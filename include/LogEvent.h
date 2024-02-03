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

#define LM_NAMES       { "off", "critical", "error", "warning", "info", "debug", "trace" }
#define LM_CAPS_NAMES  { "OFF", "CRITICAL", "ERROR", "WARNING", "INFO", "DEBUG", "TRACE" }
#define LM_SHORT_NAMES { "O", "C", "E", "W", "I", "D", "T" }

static const std::string MessageTypeNames[] LM_NAMES;
static const std::string MessageTypeCapsNames[] LM_CAPS_NAMES;
static const char* MessageTypeShortNames[] LM_SHORT_NAMES;

inline const std::string& LLtoString(Levels::LogLevel ll) { return MessageTypeNames[ll]; }
inline const std::string& LLtoCapsString(Levels::LogLevel ll) { return MessageTypeCapsNames[ll]; }

inline const char* LLtoShortString(Levels::LogLevel ll) { return MessageTypeShortNames[ll]; }

inline Levels::LogLevel LLfromString(const std::string& name)
{
	auto it = std::find(std::begin(MessageTypeNames), std::end(MessageTypeNames), name);
	if (it != std::end(MessageTypeNames))
		return static_cast<Levels::LogLevel>(std::distance(std::begin(MessageTypeNames), it));

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
