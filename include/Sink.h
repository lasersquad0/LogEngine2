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
	std::string FName;
    Layout* FLayout{nullptr};
	ullong FMessageCounts[(int)Levels::n_LogLevels]{0,0,0,0,0,0};
	ullong FBytesWritten = 0;
    Levels::LogLevel FLogLevel{LL_DEFAULT};

	bool shouldLog(const Levels::LogLevel ll) { return FLogLevel >= ll; }

	virtual void SendMsg(const LogEvent& e) = 0;

public:
	Sink(const std::string& name, const Levels::LogLevel ll = Levels::llInfo) : FName{ name }, FLogLevel{ ll } /*, FLayout{new FixedLayout()}*/
	{
		//for (uint i = 0; i < n_LogLevels; i++)
		//	FMessageCounts[i] = 0;
	}

	virtual ~Sink() { delete FLayout; }

	virtual void Flush() { /* does nothing here*/ }
	
	void PubSendMsg(const LogEvent& e)
	{
		if (!shouldLog(e.msgLevel)) return;

		SendMsg(e);
	}

	Layout* GetLayout() { return FLayout; }
	virtual void SetLayout(Layout* layout)
	{
		if (layout == nullptr) return; // layout cannot be null
		if (FLayout != nullptr) delete FLayout;
		FLayout = layout;
	}

	virtual std::string FormatString(const LogEvent& e)
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

	Levels::LogLevel GetLogLevel() { return FLogLevel; }
	void SetLogLevel(Levels::LogLevel ll) { FLogLevel = ll; }

	/**
	 * Get the name of this sink. The name identifies the sink.
	 * @returns the name of the sink.
	 **/
	const std::string& GetName() const { return FName; }
};

LOGENGINE_NS_END
