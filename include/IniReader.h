#pragma once

#include <string>
#include <fstream>
#include "Common.h"
#include <DynamicArrays.h>

std::string trimLeft(std::string str)
{
	// remove any leading spaces and tabs
	size_t strBegin = str.find_first_not_of(" \t");
	str.erase(0, strBegin);

	return str;
}

std::string trimRight(std::string str)
{
	// remove any trailing spaces and tabs
	size_t strEnd = str.find_last_not_of(" \t");
	str.erase(strEnd + 1, str.size() - strEnd);

	return str;
}

#define IO_EXCEPTION_PREFIX "Ini file exception : "

class file_exception : public std::exception
{
public:
	file_exception(const char* Message) { Text = Message; whatText = IO_EXCEPTION_PREFIX + std::string(Message); }
	file_exception(const std::string& Message) { Text = Message; whatText = IO_EXCEPTION_PREFIX + Message; }
	file_exception(const file_exception& ex) { Text = ex.Text; whatText = ex.whatText; }
	~file_exception() noexcept override {};
	file_exception& operator=(const file_exception& ex) { Text = ex.Text; whatText = ex.whatText;	return *this; }
	const char* what() const noexcept override { return whatText.c_str(); }
private:
	std::string Text;
	std::string whatText;
};

#define MISSING_SECTION "__MISSING_SECTION__"

/// /////////////////////////////////////////////
/// TODO
/// несколько параметров с одинаковыми именами внутри одной секции
/// удалять ли проблемы по краям имени секции
/// ограничения на символы в названии секции? в имени параметра? может ли быть пробел в имени параметра
/// многострочные значения?????
/// репортинг ошибок? (нету первой секции, нету = в строчке)

class IniReader
{
private:
	using ValueType = THArray<std::string>;
	THash2<std::string, std::string, ValueType, CompareStringNCase> inidata;

	void SetValue(const std::string& Key1, const std::string& Key2, const std::string& Value)
	{
		auto p = inidata.GetValuePointer(Key1, Key2);
		if (p == nullptr)
		{
			ValueType v;
			v.AddValue(Value);
			inidata.SetValue(Key1, Key2, v);
		}
		else
		{
			p->AddValue(Value);
			//auto v = inidata.GetValue(Key1, Key2);
		}
	}

public:
//	using SectionType = decltype(inidata)::ValuesHash;
	
	IniReader() { }

	IniReader(std::string& fileName) { LoadIniFile(fileName); }

	void LoadIniFile(const std::string& fileName)
	{
		inidata.Clear();

		std::ifstream fin(fileName, std::ios::in /* | std::ios::binary*/);

		if (!fin)
			throw file_exception("Cannot open file '" + fileName + "' for reading.");


		std::string line;
		std::string leftSide, rightSide;
		std::string::size_type length;
		std::string section = MISSING_SECTION;
		uint paramCount = 0;

		while (std::getline(fin, line))
		{
			/* if the beginning of line contains a # or ; then it is a comment
			if we don't find = int the line we assume that entire line is a parameter name with empty value
			lines containing spaces and tabs only are bypassed (empty lines)
			*/

			// checking if line is a comment
			line = trim(line);
			if (line.size() == 0) continue; // empty line, go to next line
			if (line[0] == '#' || line[0] == ';') continue; // bypass lines starting from # and ; sine they are comments

			if (line[0] == '[')
			{
				if (paramCount == 0 && section != MISSING_SECTION) // sections without parameters may exist 
					inidata.SetValue(section);

				length = line.find(']');
				section = line.substr(1, length - 1); // name of the section
				paramCount = 0; // reset parameters counter for the section
				continue;
			}


			// check the command and handle it
			length = line.find('=');
			if (length != std::string::npos)
			{
				leftSide = line.substr(0, length);
				rightSide = line.substr(length + 1, line.size() - length);
			}
			else
			{
				leftSide = line; // if line does not contain '=', we consider that entire line is parameter name 
			}

			SetValue(trimSPCRLF(section), trimSPCRLF(leftSide), trimSPCRLF(rightSide)); // trimSPCRLF is required here for Linux since its \n and \r differ from Windows.
			paramCount++;
		}

		// case when empty section is the last section in a file
		if (paramCount == 0 && section != MISSING_SECTION) // sections without parameters may exist 
			inidata.SetValue(section);
	}

	//SectionType& GetSection(const std::string& section)
	//{
	//	return inidata.GetValue(section);
	//}

	std::string GetValue(const std::string& section, const std::string& key, const std::string& defaultValue = "no_value", uint index = 0u)
	{
		ValueType* p = inidata.GetValuePointer(section, key);
		if (p == nullptr)
			return defaultValue;
		else
			return p->GetValue(index);
	}

	uint SectionsCount()
	{
		return inidata.GetAIndexes().Count();
	}

	bool HasSection(const std::string& section)
	{
		return inidata.GetValuePointer(section) != nullptr;
	}

	uint ValuesCount(const std::string& section)
	{
		auto p = inidata.GetValuePointer(section);
		if (p == nullptr)
			return 0;  // just return zero if section is not found 
		//else
		//	return p->Count();

		uint result = 0;

		auto keys = p->GetKeys();
		for (uint i = 0; i < keys.Count(); i++)
		{
			auto val = p->GetValue(keys[i]);
			result += val.Count();
		}

		return result;
	}

	bool HasValue(const std::string& section, const std::string& key)
	{
		auto p = inidata.GetValuePointer(section, key);
		return (p != nullptr) && (p->Count() > 0);
	}

};