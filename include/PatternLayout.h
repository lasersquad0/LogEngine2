/*
 * PatternLayout.h
 *
 * Copyright 2025, LogEngine2 Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#if !defined(PATTERNLAYOUT_H)
#define PATTERNLAYOUT_H

//#include <vector>
#include "Common.h"
#include "Pattern.h"

LOGENGINE_NS_BEGIN

class PatternLayout// : public Layout
{
protected:
	// default pattern values
	const std::string DefPatterns[Levels::n_LogLevels]{ DefaultLinePattern, DefaultCritPattern, DefaultErrorPattern,
				             DefaultWarningPattern, DefaultInfoPattern, DefaultDebugPattern, DefaultTracePattern };
	// actual pattern values
	Pattern MessagePatterns[Levels::n_LogLevels]{ Pattern{DefaultLinePattern}, Pattern{DefaultCritPattern}, Pattern{DefaultErrorPattern},
		          Pattern{DefaultWarningPattern}, Pattern{DefaultInfoPattern}, Pattern{DefaultDebugPattern}, Pattern{DefaultTracePattern} };
	Pattern AppName{ DefaultAppName };
	Pattern AppVersion{ DefaultAppVersion };
	Pattern StartAppLine{ DefaultStartAppLine };
	Pattern StopAppLine{ DefaultStopAppLine };
	//Pattern SeparatorLine{ DefaultSeparatorLine };

public:
	PatternLayout() {}
	virtual ~PatternLayout() {}
	virtual std::string Format(const LogEvent& lv) { return MessagePatterns[lv.msgLevel].Format(lv); }
	virtual std::string GetPattern(Levels::LogLevel level) { return MessagePatterns[level].GetPattern(); }
	virtual std::string GetPattern() { return MessagePatterns[Levels::llOff].GetPattern(); }

	virtual void SetDefaultPattern(Levels::LogLevel level) { MessagePatterns[level].SetPattern(DefPatterns[level]); }

	// set pattern for specified LogLevel only
	virtual void SetPattern(const std::string& pattern, Levels::LogLevel level) { MessagePatterns[level].SetPattern(pattern); }
	
	virtual void SetCritPattern (const std::string& pattern) { MessagePatterns[Levels::llCritical].SetPattern(pattern);}
	virtual void SetErrorPattern(const std::string& pattern) { MessagePatterns[Levels::llError].SetPattern(pattern);   }
	virtual void SetWarnPattern (const std::string& pattern) { MessagePatterns[Levels::llWarning].SetPattern(pattern); }
	virtual void SetInfoPattern (const std::string& pattern) { MessagePatterns[Levels::llInfo].SetPattern(pattern);    }
	virtual void SetDebugPattern(const std::string& pattern) { MessagePatterns[Levels::llDebug].SetPattern(pattern);   }
	virtual void SetTracePattern(const std::string& pattern) { MessagePatterns[Levels::llTrace].SetPattern(pattern);   }
	
	virtual void SetStartAppLinePattern(const std::string& pattern) { StartAppLine.SetPattern(pattern); }
	virtual void SetStopAppLinePattern(const std::string& pattern) { StopAppLine.SetPattern(pattern); }
	virtual void SetAppName(const std::string& aname) { AppName.SetPattern(aname); }

	// set the same pattern for all lines: Crit, Error, Warn, Info, Debug, Trace
	virtual void SetPattern(const std::string& pattern)
	{
		for (uint i = 0; i < Levels::n_LogLevels; i++)
			MessagePatterns[i].SetPattern(pattern);
	}

	virtual void SetDefaultPattern()
	{
		for (uint i = 0; i < Levels::n_LogLevels; i++)
			MessagePatterns[i].SetPattern(DefPatterns[i]);
	}

};

LOGENGINE_NS_END

#endif // PATTERNLAYOUT_H
