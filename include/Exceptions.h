/*
 * Exceptions.h
 *
 * Copyright 2024, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#pragma once

#include <string>

// 2K should be enough for one format line
#define VARIABLE_LIST_BUFFER_SIZE (2*1024)

class LogException : public std::exception
{
private:
	std::string Text;
public:
	LogException(const char* formatstr, ...)
	{
		char res[VARIABLE_LIST_BUFFER_SIZE];
		va_list list;
		va_start(list, formatstr);
#ifdef WIN32 // __STDC_SECURE_LIB__ //_MSC_VER < 1400
		vsprintf_s(res, VARIABLE_LIST_BUFFER_SIZE, formatstr, list);
#else
		vsprintf(res, formatstr, list);
#endif
		va_end(list);
		Text = res;
	}

	LogException(const std::string& Message) { Text = Message; }
	LogException(const LogException& ex) { Text = ex.Text; }
	virtual ~LogException() noexcept { /*nothing to do */ }

	LogException& operator=(const LogException& ex)
	{
		Text = ex.Text;
		return *this;
	}

	virtual const char* what() const throw()
	{
		static std::string s = "[LogException] " + Text;
		return s.c_str();
	}
};
