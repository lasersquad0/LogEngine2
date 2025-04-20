/*
 * Logger.h
 *
 * Copyright 2024, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#pragma once

#include <string>
#include <sstream>
#include "LogEvent.h"
#include "Common.h"

LOGENGINE_NS_BEGIN

/*
class Layout
{
public:
	virtual ~Layout() { }
	virtual std::string Format(const LogEvent& event) = 0;

	virtual void SetPattern(const std::string& , Levels::LogLevel ) { }
	virtual void SetCritPattern (const std::string& ) { }
	virtual void SetErrorPattern(const std::string& ) { }
	virtual void SetWarnPattern (const std::string& ) { }
	virtual void SetInfoPattern (const std::string& ) { }
	virtual void SetDebugPattern(const std::string& ) { }
	virtual void SetTracePattern(const std::string& ) { }

};


class FixedLayout : public Layout
{
public:
	std::string Format(const LogEvent& lv) override
	{
		std::string msgTypeName = LLtoString(lv.msgLevel);

		std::ostringstream o;
		o << DateTimeToStr(lv.tmtime) << " [" << msgTypeName << "] " << lv.message;

		return o.str();
	}
};
*/

LOGENGINE_NS_END
