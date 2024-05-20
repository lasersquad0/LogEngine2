
/*
 * Common.h
 *
 * Copyright 2024, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef COMMON_H
#define COMMON_H

#include <time.h>
#include <string>
#include <chrono>
#include <mutex>

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;

#define LOGENGINE_NS_BEGIN namespace LogEngine {
#define LOGENGINE_NS_END }
#define LOGENGINE_NS LogEngine

#define CRChar '\r'
#define LFChar '\n'

#ifdef WIN32 
#define EndLine "\r\n"
#define EndLineChar '\n'
#define BUILD_ENDL(_) (_) += CRChar; (_) += LFChar;
#else
#define EndLine "\n"
#define EndLineChar '\n'
#define BUILD_ENDL(_) (_) += LFChar;
#endif

typedef std::lock_guard<std::recursive_mutex> mutexguard;
typedef std::chrono::time_point<std::chrono::system_clock> tm_point;

// truncates Value to Precision digits after point
double round(const double Value, const int Precision);

// functions to convert varous ints and longs to string
std::string IntToStr(int Value, int FieldSize);
std::string IntToStr(uint Value, int FieldSize);
std::string IntToStr(int Value);
std::string IntToStr(uint Value);
std::string IntToStr(ulong Value);

// function to convert double to string
std::string FloatToStr(double Value);

// function to convert bool to string
std::string BoolToStr(bool Value);

// function to convert string value (1,0,yes,no,true,false) to the bool
bool StrToBool(const std::string& Value);

// extracts filename from path with filename
std::string ExtractFileName(const std::string& FileName);

// extracts directory name from path
std::string ExtractFileDir(const std::string& FileName);

// excludes file extention from FileName
std::string StripFileExt(const std::string& FileName);

// replaces in string S all occurrences of OldPattern by NewPattern
std::string StringReplace(const std::string& S, const std::string& OldPattern, const std::string& NewPattern);

tm_point GetCurrTimePoint();

struct tm GetCurrDateTime();

// converts date to string representation
//std::string DateToString(int Date);

// retrieves current date as std::string
std::string GetCurrDateAsString(void);

// retrieves current time as std::string
std::string GetCurrTimeAsString(void);

// retrieves current datetime as std::string
std::string GetCurrDateTimeAsString(void);

// gets formatted current datetime
std::string FormatCurrDateTime(const std::string& FormatStr);

// converts native datetime value into std::string
std::string DateTimeToStr(time_t t);
std::string DateTimeToStr(struct tm const& t);

// deletes all leading and trailing \n \r symbols from string
std::string DelCRLF(const std::string s);

// removes all leading and trailing \n \r and space symbols from string
std::string trimSPCRLF(std::string S);

// removes all leading and trailing space and tab symbols from string
std::string trim(std::string s);

// removes any leading space and tab symbols from string
std::string trimLeft(std::string str);

// removes any trailing space and tab symbols from string
std::string trimRight(std::string str);

// compares two strings case insensitive
bool EqualNCase(const std::string& str1,const std::string& str2);

// compares two strings case insensitive
// return 0 if strings are equal, -1 if str1 is "less" then str2, 1 if str1 is "larger" than str2 
int CompareNCase(const std::string& str1, const std::string& str2);

// checks if string contains unsigned integer or not
bool isUInt(std::string& value);

uint GetThreadID();

std::string StrToLower(std::string str);

std::string DisplaySystemVersion();

#endif //COMMON_H
