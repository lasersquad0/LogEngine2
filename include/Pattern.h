/*
 * Pattern.h
 *
 * Copyright 2025, LogEngine2 Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#pragma once

#include <cassert>
#include <vector>
#include <sstream>
#include "Common.h"
#include "DynamicArrays.h"
#include "LogEvent.h"

#ifdef WIN32
#include <windows.h>
#endif


LOGENGINE_NS_BEGIN

#define APPNAME_PROPERTY  "AppName"
#define APPVERSION_PROPERTY "AppVersion"

class Holder
{
public:
	virtual std::string Format(const LogEvent& event) = 0;
	virtual ~Holder() {}
};

#define STRFTIME_SIZE 100

class DateHolder : public Holder
{
public:
	std::string Format(const LogEvent& event) override
	{
		char ss[STRFTIME_SIZE];
		std::strftime(ss, STRFTIME_SIZE, "%d-%b-%Y", &event.tmtime);
		return ss;
	}
};

class TimeHolder : public Holder
{
public:
	std::string Format(const LogEvent& event) override
	{
		char ss[STRFTIME_SIZE];
		std::strftime(ss, STRFTIME_SIZE, "%X", &event.tmtime);
		return ss;
	}
};

class DateTimeHolder : public Holder
{
public:
	std::string Format(const LogEvent& event) override
	{
		char ss[STRFTIME_SIZE];
		std::strftime(ss, STRFTIME_SIZE, "%d-%b-%Y %X", &event.tmtime);
		return ss;
	}
};

class MessageHolder : public Holder
{
public:
	std::string Format(const LogEvent& event) override { return event.message; }
};

class ThreadHolder : public Holder
{
public:
	std::string Format(const LogEvent& event) override { return IntToStr(event.threadID/*, 4*/); }
};

class AppNameHolder : public Holder
{
private:
	std::string appName;
public:
	AppNameHolder(const std::string& aname) { this->appName = aname; }
	std::string Format(const LogEvent&) override { return appName; }
};

class AppVersionHolder : public Holder
{
private:
	std::string version;
public:
	AppVersionHolder(const std::string& ver) { this->version = ver; }

	std::string Format(const LogEvent&) override { return version; }
};

class LiteralHolder : public Holder
{
private:
	std::string value;
public:
	LiteralHolder(std::string& val) { this->value = val; }
	std::string Format(const LogEvent&) override { return value; }
};

class LogLevelHolder : public Holder
{
public:
	std::string Format(const LogEvent& event) override { return LLtoCapsString(event.msgLevel); }
};

class OSHolder : public Holder
{
public:
	std::string Format(const LogEvent&) override
	{
#ifdef WIN32
		DWORD OSMajorVer = 6, OSMinorVer = 3, SPMajorVer = 0, SPMinorVer = 0;
		DWORD result;

		if (0 == GetProductInfo(OSMajorVer, OSMinorVer, SPMajorVer, SPMinorVer, &result))
			return "Error getting OS info";
		else
			return IntToStr(OSMajorVer) + "." + IntToStr(OSMinorVer) + " SP " + IntToStr(SPMajorVer) + "." + IntToStr(SPMinorVer) + " " + IntToStr(result);
#else
		return "OSHolder";
#endif	
	}
};

class OSVersionHolder : public Holder
{
private:
	std::string FVersionCache;
public:
	std::string Format(const LogEvent&) override 
	{
		//return cached value if it is exists.
		if (FVersionCache.size() > 0) return FVersionCache;

#ifdef WIN32
		const auto system = L"kernel32.dll";
		DWORD dummy = 0;
		const auto cbInfo = ::GetFileVersionInfoSizeExW(FILE_VER_GET_NEUTRAL, system, &dummy);
		std::vector<char> buffer(cbInfo);
		GetFileVersionInfoExW(FILE_VER_GET_NEUTRAL, system, dummy, static_cast<DWORD>(buffer.size()), &buffer[0]);
		void* p = nullptr;
		UINT size = 0;
		::VerQueryValueW(buffer.data(), L"\\", &p, &size);
		assert(size >= sizeof(VS_FIXEDFILEINFO));
		assert(p != nullptr);
		auto pFixed = static_cast<const VS_FIXEDFILEINFO*>(p);

		std::ostringstream o;
		o << HIWORD(pFixed->dwFileVersionMS) << '.'
			<< LOWORD(pFixed->dwFileVersionMS) << '.'
			<< HIWORD(pFixed->dwFileVersionLS) << '.'
			<< LOWORD(pFixed->dwFileVersionLS);

		FVersionCache = o.str();
		return FVersionCache;
#else
		FVersionCache = "<OSVERSION>";
		return FVersionCache;
#endif
	}
};

#define OSMacro		      "%OS%"
#define OSVersionMacro	  "%OSVERSION%"
#define ThreadMacro		  "%THREAD%"
#define MessageMacro      "%MSG%"
#define DateTimeMacro     "%DATETIME%"
#define DateMacro         "%DATE%"
#define TimeMacro         "%TIME%"
#define AppNameMacro      "%APPNAME%"
#define AppVersionMacro   "%APPVERSION%"
#define LogLevelMacro	  "%LOGLEVEL%"

#define SecondMacro       "%SEC%"
#define MinuteMacro       "%MIN%"
#define HourMacro         "%HOUR%"
#define DayMacro          "%DAY%"
#define MonthMacro        "%MONTH%"
#define YearMacro         "%YEAR%"

#define ExceptionMacro    "%E%"
#define ClassNameMacro	  "%NAME%"
#define ClassClassMacro	  "%CLASS%"

#define DefaultAppName        "nonameapp"
#define DefaultAppVersion     "0.0.0.0"
#define DefaultLinePattern    " " TimeMacro " #" ThreadMacro ": " MessageMacro
#define DefaultCritPattern    "*!*" DefaultLinePattern
#define DefaultErrorPattern   "E!" DefaultLinePattern
#define DefaultWarningPattern "W#" DefaultLinePattern
#define DefaultInfoPattern    "I" DefaultLinePattern
#define DefaultDebugPattern   "D" DefaultLinePattern
#define DefaultTracePattern   "T" DefaultLinePattern
#define DefaultStartAppLine   "\n%APPNAME% %APPVERSION% startup\nLog is started at %DATETIME%."
#define DefaultStopAppLine    "%APPNAME% %APPVERSION% normal shutdown \nLog stopped at %DATETIME%.\n"
#define DefaultSeparatorLine  "----------------------------------------------------------------"


class Pattern
{
protected:
	THArray<Holder*> FHolders; // container of pointers is required here to support proper virtual "->Format()" calls 
	std::string FPattern;
	void parsePattern(const std::string& pattern); // compiles string pattern into list of appropriate Holder classes
	void clearHolders();
public:
	Pattern(const std::string& pattern) { parsePattern(pattern); }
	virtual ~Pattern() { clearHolders(); }
	virtual std::string Format(const LogEvent& event);
	void SetPattern(const std::string& pattern) { parsePattern(pattern); }
	std::string GetPattern() { return FPattern; }
};

LOGENGINE_NS_END

#ifdef LOGENGINE_HEADER_ONLY
#include "Pattern-hdr.h"
#endif
