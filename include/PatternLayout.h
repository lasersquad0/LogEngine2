/*
 * PatternLayout.h
 *
 * Copyright 2023, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#if !defined(_PATTERNLAYOUT_H_)
#define _PATTERNLAYOUT_H_

#include <vector>
//#include <chrono>
//#include <iomanip>
//#include "Common.h"
//#include "DynamicArrays.h"
#include "Layout.h"
#include "Pattern.h"

LOGENGINE_NS_BEGIN

class PatternLayout : public Layout
{
protected:
	Pattern MessagePatterns[Levels::n_LogLevels]{ Pattern{DefaultLinePattern}, Pattern{DefaultCritPattern}, Pattern{DefaultErrorPattern},
		Pattern{DefaultWarningPattern}, Pattern{DefaultInfoPattern}, Pattern{DefaultDebugPattern}, Pattern{DefaultTracePattern} };
	Pattern AppName{ DefaultAppName };
	Pattern AppVersion{ DefaultAppVersion };
	Pattern DefaultPattern{ DefaultLinePattern };
	Pattern StartAppLine{ DefaultStartAppLine };
	Pattern StopAppLine{ DefaultStopAppLine };
	//Pattern SeparatorLine{ DefaultSeparatorLine };

public:
	PatternLayout() {}
	virtual ~PatternLayout() {};
	std::string Format(LogEvent& lv) override { return MessagePatterns[lv.msgLevel].Format(lv); }

	void SetErrorPattern(const std::string& pattern, Levels::LogLevel msgType) { MessagePatterns[msgType].SetPattern(pattern); }
	void SetErrorPattern(const std::string& pattern) { MessagePatterns[Levels::llError].SetPattern(pattern); }
	void SetWarnPattern(const std::string& pattern) { MessagePatterns[Levels::llWarning].SetPattern(pattern); }
	void SetInfoPattern(const std::string& pattern) { MessagePatterns[Levels::llInfo].SetPattern(pattern); }
	void SetStartAppLinePattern(const std::string& pattern) { StartAppLine.SetPattern(pattern); }
	void SetStopAppLinePattern(const std::string& pattern) { StopAppLine.SetPattern(pattern); }
	void SetAppName(const std::string& aname) { AppName.SetPattern(aname); }

	// set the same pattern for all lines: Crit, Error, Warn, Info, Debug, Trace
	void SetAllPatterns(const std::string& pattern)
	{
		for (uint i = 0; i < Levels::n_LogLevels; i++)
		{
			MessagePatterns[i].SetPattern(pattern);
		}

		DefaultPattern.SetPattern(pattern);
	}
};

LOGENGINE_NS_END

#endif // _PATTERNLAYOUT_H_