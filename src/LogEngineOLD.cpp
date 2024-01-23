/*
 * LogEngine.cpp
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 * 
 * 
 */

#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <LogEngine.h>
#include <Common.h>
#include <LogEvent.h>

#ifndef WIN32 //HAVE_STDARG_H assume we have Linux here
#include <stdarg.h>
#include <pthread.h>
#endif

#include <iostream>
 
LOGENGINE_NS_BEGIN


	//////////////////////////////////////////////////////////////////////
	// TLogEngine
	//////////////////////////////////////////////////////////////////////
	/*
	void TLogEngine::initialInit()
	{
		//	FLogStream = nullptr;
		FStarted = false;
		FFileBytesWritten = 0;
		FTotalBytesWritten = 0;
		FInitialFileSize = 0;
		FMessageCount[llOff] = 0; // TODO how to initialie better and shorter here?
		FMessageCount[llCritical] = 0;
		FMessageCount[llError] = 0;
		FMessageCount[llWarning] = 0;
		FMessageCount[llInfo] = 0;
		FMessageCount[llDebug] = 0;
		FMessageCount[llTrace] = 0;
	}


	TLogEngine::TLogEngine(const std::string& ConfigFileName)//: TLogEngine() // : LogQueue()
	{
		initialInit();

		Properties Props;
		std::ifstream fin(ConfigFileName.c_str());

		if (fin.fail())
		{
			throw LogException("Cannot open config file: %s", ConfigFileName.c_str());
		}

		Props.load(fin);
		//FProperties.Fill(Props);
	}

	TLogEngine::~TLogEngine()
	{
		if (FStarted)
			Stop();

		//	if(FProperties.Threaded)
		//	{
		//#ifdef WIN32
		//		DWORD exitCode;
		//		if(GetExitCodeThread(hThread, &exitCode) == STILL_ACTIVE) // wait only when thread is running
		//			WaitForSingleObject(hThread, INFINITE);
		//#else
		////#ifdef HAVE_PTHREAD_H
		//		pthread_join(hThread, NULL);
		//#endif
		//	}

			//	LEAVE_CRITICAL_SECTION(CriticalSection);
			//DELETE_CRITICAL_SECTION(CriticalSection);

	}

	void TLogEngine::initThread(void)
	{
		//	if(!FProperties.Threaded)
		//		return;

		LogEngineThreadInfo* info = new LogEngineThreadInfo;
		info->LogQueue = &LogQueue;
		info->LogEngine = this;

		std::thread thr(ThreadProc, info);
		m_thread.swap(thr);
		std::stringstream s;
		s << thr.get_id();
		unsigned long threadID;
		s >> threadID;

		//#ifdef WIN32
		//	unsigned long threadID;
		//	hThread = CreateThread(nullptr, 0, ThreadProc, info, 0, &threadID); // <<<< TODO: right param?
		//#else 
		//#ifdef HAVE_PTHREAD_H
		//	if (pthread_create(&hThread, nullptr, ThreadProc, info) != 0)
		//	{
				// TODO: exception on failure?
		//	}
		//#endif
	}

	int TLogEngine::ThreadProc(void* parameter)
	{
		LogEngineThreadInfo* info = (LogEngineThreadInfo*)parameter;

		LogEvent* current_msg;
		do
		{
			current_msg = info->LogQueue->WaitForElement();
			if (current_msg)
			{
				//OutputDebugString("new event");

				LogEvent* event = current_msg;
				//info->LogEngine->writeEvent(event);
				delete event;
			}

		} while (current_msg);

		delete info;

		return 0;
	}

	void TLogEngine::resetStatistics()
	{
		FFileBytesWritten = 0;
		FTotalBytesWritten = 0;
		FInitialFileSize = 0;
		FMessageCount[llOff] = 0; // TODO find better way to initialie here?
		FMessageCount[llCritical] = 0;
		FMessageCount[llError] = 0;
		FMessageCount[llWarning] = 0;
		FMessageCount[llInfo] = 0;
		FMessageCount[llDebug] = 0;
		FMessageCount[llTrace] = 0;
	}

	void TLogEngine::Start(void)
	{
		if (FStarted)
			return;

		//	ENTER_CRITICAL_SECTION(CriticalSection);

		resetStatistics();

		//FLogStream = new TFileStream(FProperties.LogFileName, fmWrite);
		//FLogStream->Seek(0, smFromEnd);
		//FInitialFileSize = FLogStream->Length();
		FStarted = true;

		initThread();

		writeStart();

		//	LEAVE_CRITICAL_SECTION(CriticalSection);
	}

	void TLogEngine::Stop(void)
	{
		if (!FStarted)
			return;

		//	if(FProperties.Threaded) 
		//	{
		//		LogQueue.PushElement(nullptr);
		//		m_thread.join(); // waiting till thread finishes

		//#ifdef WIN32
		//		WaitForSingleObject(hThread, INFINITE);
		//		CloseHandle(hThread);
		//#else
		//#ifdef HAVE_PTHREAD_H
		//		if (pthread_join(hThread, NULL))
		//		{
					// TODO: exception on error?
		//		}
		//#endif
		//	}

		writeStop(); // writeStop and writeStart do not use threads at all

		//delete FLogStream;
		//FLogStream = nullptr;
		FStarted = false;
	}

	ulong TLogEngine::getFileLength()
	{
		return FInitialFileSize + FFileBytesWritten;
	}

	void TLogEngine::internalWrite(const std::string& msg)
	{
		//ENTER_CRITICAL_SECTION(CriticalSection);
		mutexguard lock(mtx);

		if (!FStarted)
			throw LogException("The LogEngine is not started!");

		//if(FProperties.MaxLogSize > 0 && getFileLength() > FProperties.MaxLogSize * 1024)
		//	truncLogFile();

		//int written = FLogStream->Write(msg);
		//written += FLogStream->WriteCRLF();

		//FFileBytesWritten += written;
		//FTotalBytesWritten += written;
		//FLogStream->Flush();

		//LEAVE_CRITICAL_SECTION(CriticalSection);
	}

	void TLogEngine::writeStart()
	{
		LogEvent event("", llOff, GET_THREAD_ID(), GetCurrDateTime());

		//internalWrite(FProperties.StartAppLine.format(event));
	}

	void TLogEngine::writeStop()
	{
		LogEvent event("", llOff, GET_THREAD_ID(), GetCurrDateTime());

		//internalWrite(FProperties.StopAppLine.format(event));
	}
	

	std::string TLogEngine::generateBackupName(void)
	{
		std::string s;
		
		switch(1 FProperties.BackupType)
		{
			case lbTimeStamp:
				s = StripFileExt(FProperties.LogFileName) + "(" + FormatCurrDateTime("%d-%m-%Y %H.%M.%S") + ")" + LogExt;
				break;
			case lbSingle:
				s = StripFileExt(FProperties.LogFileName) + LogExt + BackupExt;
				break;
			default:
				throw LogException("Wrong Backup Type.");
		}
		
		return s;
	}

	void TLogEngine::truncLogFile(void)
	{
		//ENTER_CRITICAL_SECTION(CriticalSection);
		//mutexguard lock(mtx);

		//delete FLogStream;
		//FLogStream = nullptr;

		//if (FProperties.BackupType == lbNone)
		//{
		//	remove(FProperties.LogFileName.c_str()); // for lbNone we remove existing log file and start it from beginning
		//}
		//else
		//{
		//	std::string newName = generateBackupName();
		//	remove(newName.c_str());
		//	rename(FProperties.LogFileName.c_str(), newName.c_str());
		//}

		//FLogStream = new TFileStream(FProperties.LogFileName, fmWrite);
		//FInitialFileSize = FLogStream->Length();
		//FFileBytesWritten = 0;

		//LEAVE_CRITICAL_SECTION(CriticalSection);
	}


	// Default value of instance of LogEngine
	TLogEngine* TLogEngine::loginstance = nullptr;

	void TLogEngine::InitLogEngine(const std::string& ConfigFileName)
	{
		if (loginstance)
		{
			delete loginstance;
		}

		loginstance = new TLogEngine(ConfigFileName);
		loginstance->Start();
	}

	//void TLogEngine::InitLogEngine()
	//{
	//	if(loginstance)
	//	{
	//		delete loginstance;	
	//	}
	//	
	//	loginstance = new TLogEngine();
	//	loginstance->Start();
	//}
	//
	//void TLogEngine::CloseLogEngine()
	//{
	//	if(loginstance)
	//	{
	//		delete loginstance;	
	//		loginstance = nullptr;
	//	}
	//}
	//
	//TLogEngine* TLogEngine::getInstance()
	//{
	//	if(nullptr == loginstance)
	//	{
	//		loginstance = new TLogEngine();
	//		loginstance->Start();
	//	}
	//
	//	return loginstance;
	//}

	*/

	/************************************************************************/
	/* Global Functions                                                     */
	/************************************************************************/


	//void InitLogEngine(const std::string& ConfigFileName)
	//{
	//	TLogEngine::InitLogEngine(ConfigFileName);
	//}

	//void InitLogEngine(void)
	//{
	//	TLogEngine::InitLogEngine();
	//}
	//
	//void CloseLogEngine(void)
	//{
	//	TLogEngine::CloseLogEngine();
	//}

	
LOGENGINE_NS_END
