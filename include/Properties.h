/*
 * Properties.h
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <string>
#include "DynamicArrays.h"

class Properties : public THash<std::string, std::string, CompareStringNCase> 
{
public:
	Properties(): THash() {}
	virtual ~Properties() {}

	virtual void load(std::istream& in);
	virtual void save(std::ostream& out);

	virtual int   getInt (const std::string& property, int defaultValue   = 0) const;
	virtual ulong getUInt(const std::string& property, ulong defaultValue = 0) const;
	virtual bool  getBool(const std::string& property, bool defaultValue = false) const;
	virtual std::string getString(const std::string& property, const char* defaultValue = "") const;

	static std::string trim(std::string str);
protected:
	virtual void _substituteVariables(std::string& value);
};

#endif // PROPERTIES_H

