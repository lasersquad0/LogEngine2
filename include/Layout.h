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

class Layout
{
public:
	virtual ~Layout() {}
	virtual std::string Format(LogEvent& event) = 0;
};


class FixedLayout : public Layout
{
public:
	virtual std::string Format(LogEvent& lv)
	{
		std::string msgTypeName = LLtoString(lv.msgLevel);

		std::ostringstream o;
		o << DateTimeToStr(lv.tmtime) << " [" << msgTypeName << "] " << lv.message;

		return o.str();
	}
};

LOGENGINE_NS_END
