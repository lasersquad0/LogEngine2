/*
 * Sink.h
 *
 * Copyright 2024, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#pragma once

#include <string>
#include "LogEvent.h"
#include "Layout.h"

LOGENGINE_NS_BEGIN

class Sink
{
protected:
	Levels::LogLevel FLogLevel;
	std::string FName;
	Layout* FLayout;
	ullong FMessageCounts[(int)Levels::n_LogLevels]{0,0,0,0,0,0};
	ullong FBytesWritten = 0;

	bool shouldLog(Levels::LogLevel ll) { return FLogLevel >= ll; }

	virtual void SendMsg(LogEvent& e) = 0;
	virtual void Flush() { /* does nothing here*/ }

public:
	Sink(const std::string& name, Levels::LogLevel ll = Levels::llInfo) : FLogLevel{ ll }, FName{ name }, FLayout{ new FixedLayout() }
	{
		//for (uint i = 0; i < n_LogLevels; i++)
		//	FMessageCounts[i] = 0;
	}

	virtual ~Sink() { delete FLayout; };

	void PubSendMsg(LogEvent& e)
	{
		if (!shouldLog(e.msgLevel)) return;

		SendMsg(e);
	};

	virtual void SetLayout(Layout* layout)
	{
		if (layout == nullptr) return; // layout cannot be null
		if (FLayout != nullptr) delete FLayout;
		FLayout = layout;
	}

	virtual std::string FormatString(LogEvent& e)
	{
		std::string str = FLayout->Format(e);
		FMessageCounts[e.msgLevel]++;
	
		return str;
	}

	/**
	* Returns bytes written since creating the sink.
	* @returns bytes written as value of size_t type.
	**/
	ullong GetBytesWritten()
	{
		return FBytesWritten;
	}
	/**
	* Returns message statistic. Message statistic it is number of written messages per log level.
	* @returns array of size_t valuesm one value for each log level.
	**/
	ullong* GetMessageCounts()
	{
		return FMessageCounts;
	}
	void SetLogLevel(Levels::LogLevel ll) { FLogLevel = ll; }

	/**
	 * Get the name of this sink. The name identifies the sink.
	 * @returns the name of the sink.
	 **/
	const std::string& GetName() const { return FName; };
};

LOGENGINE_NS_END