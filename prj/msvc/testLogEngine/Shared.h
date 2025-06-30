#pragma once

//#include <string.h>
#include <string>
#include <format>
//#include <iostream>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <cppunit/extensions/HelperMacros.h>

#ifdef WIN32
#define TEST_FILES_FOLDER "..\\..\\..\\test\\testFiles\\"
#else
#define TEST_FILES_FOLDER "testFiles/"
#endif

#define LOG_FILES_FOLDER "./logs/"

CPPUNIT_NS_BEGIN
template<>
struct assertion_traits<std::string>   // specialization for the std::string type
{
	static bool equal(const std::string& x, const std::string& y)
	{
		return x.compare(y) == 0;
	}

	static std::string toString(const std::string& x)
	{
		return x;
	}
};

template<>
struct assertion_traits<const char*>   // specialization for the char* type
{
	static bool equal(const char* x, const char* y)
	{
		return strcmp(x, y) == 0;
	}

	static std::string toString(const char* x)
	{
		std::string text;
		text += '"';
		text.append(x);
		text += '"';    // add quote around the string to see whitespaces
		
		CppUnit::OStringStream ost;
		ost << text;
		return ost.str();
	}
};

template<>
struct assertion_traits<char*>   // specialization for the char* type
{
	static bool equal(char* x, char* y)
	{
		return strcmp(x, y) == 0;
	}

	static std::string toString(char* x)
	{
		std::string text;
		text += '"';
		text.append(x);
		text += '"';    // add quote around the string to see whitespaces
		
		CppUnit::OStringStream ost;
		ost << text;
		return ost.str();
	}
};

CPPUNIT_NS_END


template<class STRING>
STRING MillisecToStr(uint64_t ms)
{
	// make sure that STRING is one of instantiations of std::string
	static_assert(std::is_base_of<std::basic_string<typename STRING::value_type, typename STRING::traits_type>, STRING>::value);

	uint32_t milliseconds = ms % 1000;
	uint32_t seconds = (ms / 1000) % 60;
	uint32_t minutes = (ms / 60000) % 60;
	uint32_t hours = (ms / 3600000) % 24;

	STRING result;

	if constexpr (std::is_same_v<typename STRING::value_type, char>)
	{
		if (hours > 0)
			result = std::format("{} hours {} minutes {} seconds {} ms", hours, minutes, seconds, milliseconds);
		else if (minutes > 0)
			result = std::format("{} minutes {} seconds {} ms", minutes, seconds, milliseconds);
		else
			result = std::format("{} seconds {} ms", seconds, milliseconds);
	}
	else if constexpr (std::is_same_v<typename STRING::value_type, wchar_t>)
	{
		if (hours > 0)
			result = std::format(L"{} hours {} minutes {} seconds {} ms", hours, minutes, seconds, milliseconds);
		else if (minutes > 0)
			result = std::format(L"{} minutes {} seconds {} ms", minutes, seconds, milliseconds);
		else
			result = std::format(L"{} seconds {} ms", seconds, milliseconds);
	}
	else
		result = "UNKNOWN STRING TYPE";

	return result;
}

//std::string millisecToStr(long long ms);
