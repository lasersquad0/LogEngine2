/*
 * RotatingFileSink.h
 *
 * Copyright 2024, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#pragma once

#include <string>
#include <iostream>
#include <filesystem>
#include "FileSink.h"
#include "LogEvent.h"
#include "FileStream.h"
#include "Exceptions.h"

LOGENGINE_NS_BEGIN

enum TLogRotatingStrategy { rsNone, rsSingle, rsTimeStamp, rsNumbers };
#define DefaultMaxLogSize  1024*1024 // 1 kbyte 
#define DefaultMaxBackupIndex  10  // max number of backup files for strategy rsNumbers
#define DefaultRotatingStrategy rsNone
#define LogExt    ".log"
#define BackupExt ".bak"

class RotatingFileSink : public FileSink
{
protected:
	ulong FInitialFileSize; // log file size on the moment of creating this class. need for proper work of file rotating by size
	ulong FMaxLogSize;
	TLogRotatingStrategy FStrategy;
	uint FMaxBackupIndex;
public:
	RotatingFileSink(const std::string& name, const std::string& fileName, ulong MaxLogSize = DefaultMaxLogSize,
		TLogRotatingStrategy strategy = DefaultRotatingStrategy, uint MaxBackupIndex = DefaultMaxBackupIndex) : FileSink(name, fileName)
	{
		FMaxLogSize = MaxLogSize;
		FStrategy = strategy;
		FMaxBackupIndex = MaxBackupIndex;
		FInitialFileSize = FStream->Length();
	}

	void SendMsg(LogEvent& e) override
	{
		if (FInitialFileSize + FBytesWritten > FMaxLogSize) truncLogFile();

		FileSink::SendMsg(e);
	}

	void Flush() override { FStream->Flush(); }

	uint FindFreeBackupIndex(std::string& existingFileName)
	{
		std::string s = StripFileExt(existingFileName);
		int ind = 1;
		while (true)
		{
			std::string ss = s + '.' + IntToStr(ind) + BackupExt;
			if (!std::filesystem::exists(ss)) break;
			ind++;
		}

		return ind;
	}

	void ShiftBackupFiles(std::string& existingFileName, uint freeIndex)
	{
		std::string s = StripFileExt(existingFileName);
		for (uint i = freeIndex - 1; i > 0; i--)
		{
			std::string oldName = s + '.' + IntToStr(i) + BackupExt;
			std::string newName = s + '.' + IntToStr(i + 1) + BackupExt;
			rename(oldName.c_str(), newName.c_str());
		}
	}

	std::string GenerateBackupName(std::string& existingFileName)
	{
		std::string newFileName;
		switch (FStrategy)
		{
		case rsSingle:
			newFileName = existingFileName + BackupExt;
			break;
		case rsTimeStamp:
			newFileName = StripFileExt(existingFileName) + "(" + FormatCurrDateTime("%d-%m-%Y %H.%M.%S") + ")" + LogExt;
			break;
		case rsNumbers:
		{
			uint ind = FindFreeBackupIndex(existingFileName);
			ShiftBackupFiles(existingFileName, ind);
			newFileName = StripFileExt(existingFileName) + ".1" + BackupExt;
			break;
		}
		default:
			throw LogException("Wrong rotating strategy.");
		}

		return newFileName;
	}

	void truncLogFile(void)
	{
		//mutexguard lock(mtx);

		std::string filename = FStream->GetFileName();

		delete FStream;
		FStream = nullptr;

		std::string newName;
		switch (FStrategy)
		{
		case rsNone:
			remove(filename.c_str()); // for rsNone we remove existing log file and start it from beginning
			break;
		case rsSingle:
			newName = GenerateBackupName(filename);
			remove(newName.c_str());
			rename(filename.c_str(), newName.c_str());
			break;
		case rsTimeStamp:
			newName = GenerateBackupName(filename);
			remove(newName.c_str());
			rename(filename.c_str(), newName.c_str());
			break;
		case rsNumbers:
			newName = GenerateBackupName(filename);
			rename(filename.c_str(), newName.c_str());
			break;
		}

		FStream = new TFileStream(filename);
		FInitialFileSize = FStream->Length();
		FBytesWritten = 0;
	}

	ulong GetMaxLogSize() { return FMaxLogSize; }
	TLogRotatingStrategy GetStrategy() { return FStrategy; }

};

LOGENGINE_NS_END