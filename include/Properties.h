/*
 * Properties.h
 *
 * Copyright 2025, LogEngine2 Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <string>
#include "Common.h"
#include "DynamicArrays.h"

LOGENGINE_NS_BEGIN

class Properties : public TStringHashNCase //THash<std::string, std::string, CompareStringNCase> 
{
public:
	//Properties(): THash() {}
	~Properties() override {}

	//virtual void load(std::istream& in);
	//virtual void save(std::ostream& out);

	virtual int   getInt (const std::string& property, int defaultValue   = 0) const;
	virtual ulong getUInt(const std::string& property, ulong defaultValue = 0) const;
	virtual bool  getBool(const std::string& property, bool defaultValue = false) const;
	virtual std::string getString(const std::string& property, const std::string& defaultValue = "") const;

};

LOGENGINE_NS_END

#ifdef LOGENGINE_HEADER_ONLY
#include "Properties-hdr.h"
#endif


#endif // PROPERTIES_H

