
#include <chrono>
#include <thread>
#include "Shared.h"
#include "Common.h"
#include <LogEngine.h>
#include "testThreadLog.h"


CPPUNIT_TEST_SUITE_REGISTRATION( ThreadLogTest );

void ThreadLogTest::setUp ()
{

}

void ThreadLogTest::tearDown ()
{
    // free memory allocated in setUp, do other things
}

using namespace LogEngine;

struct ThreadInfoStruct
{
    Logger *logger;
    bool begin;
};

int testThreadProc(void* param)
{
	ThreadInfoStruct *info = (ThreadInfoStruct*) param;
	Logger *log = info->logger;

	if(!log)
	    return 0; // <<< unused, so 0. (void*) 1 looks silly =)

	while (!info->begin)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	
	std::string s = IntToStr(GetThreadID());
	log->Crit(s + " is executing 1.");
	log->Error(s + " is executing 2.");
	log->Info(s + " is executing 3.");
	log->Warn(s + " is executing 4.");
	log->Debug(s + " is executing 5.");
	log->Trace(s + " is executing 6.");

	log->LogFmt("{} is executing {}.", Levels::llCritical, GetThreadID(), "11");
	log->LogFmt("{} is executing {}.", Levels::llError, GetThreadID(), "22");
	log->LogFmt("{} is executing {}.", Levels::llWarning, GetThreadID(), "33");
	log->LogFmt("{} is executing {}.", Levels::llInfo,GetThreadID(), "44");
	log->LogFmt("{} is executing {}.", Levels::llDebug, GetThreadID(), "55");
	log->LogFmt("{} is executing {}.", Levels::llTrace, GetThreadID(), "66");

	return 0;
}

#define nthreads 10
/*
void ThreadLogTest::testCallLogFromManyThreads()
{
	//Properties prop;
	//prop.SetValue("LogFileName", LOG_FILES_FOLDER "ThreadsLog.log");
	//prop.SetValue("ApplicationName", "testCallLogFromManyThreads");
	//InitLogEngine(prop);

	Logger& logger = GetFileLogger("testCallLogFromManyThreads", LOG_FILES_FOLDER "testCallLogFromManyThreads.log");
	
	logger.Info("begin creating threads");
	
	THArray<std::thread*> threads;
	//THArray<unsigned long> ids;

	//std::vector<THREAD_TYPE> handles;
	//std::vector<unsigned long> ids;

	//unsigned long thrID = 0;
	//THREAD_TYPE hThread;

	ThreadInfoStruct info = { &logger, false };

	for(uint i = 0; i < nthreads; i++)
	{
		logger.Info("Creating threads");

		std::thread* thr = new std::thread(testThreadProc, &info);
		threads.AddValue(thr);

		logger.LogFmt("Created thread #{}", Levels::llInfo, thr->get_id());
	}

	logger.Info("all threads created");

	logger.Info("begin resuming");
	
	info.begin = true; //TODO small data race here, consider use atomics
	
	logger.Info("all threads resumed");

	// waiting till all threads finished
	for (uint i = 0; i < threads.Count(); i++)
	{
		threads[i]->join();
	}

	//freeing memory
	for (uint i = 0; i < threads.Count(); i++)
	{
		delete threads[i];
	}
}
*/

void ThreadLogTest::testAsyncLog1()
{
	//Properties prop;
	//prop.SetValue("LogFileName", LOG_FILES_FOLDER "ThreadedLog.log");
	//prop.SetValue("Threaded", "trUe");
	//InitLogEngine(prop);

	Logger& log = GetFileLogger("testAsyncLog1", LOG_FILES_FOLDER "testAsyncLog1.log");
	
	std::string str("main thread ID: "); 
	str += IntToStr(GetThreadID());
	log.Info(str);

	log.SetLogLevel(Levels::llDebug);
	log.SetAsyncMode(true);

	log.Crit("threaded crit #1");
	log.Error("threaded error #1");
	log.Warn("threaded warning #1");
	log.Info("threaded info #1");
	log.Debug("threaded debug #1");
	log.Trace("threaded trace #1");

	log.SetAsyncMode(false); 
}


void ThreadLogTest::testThreadLogMeasureTime1()
{
	std::string fileName = LOG_FILES_FOLDER "testThreadLogMeasureTime1.log";
	std::string loggerName = "testThreadLogMeasureTime1";

	remove(fileName.c_str());
	Logger& log = GetFileLogger(loggerName, fileName);

	log.SetAsyncMode(true);

	auto start1 = std::chrono::high_resolution_clock::now();
	const int NUM_LOGS = 100'000;

	for (size_t i = 0; i < NUM_LOGS; i++)
	{
		log.Warn("threaded warning ###" + std::to_string(i));
	}

	auto stop = std::chrono::high_resolution_clock::now();

	std::cout << std::endl << NUM_LOGS << " written in ASYNC mode.\n";
	std::cout << "  Excec time: " << millisecToStr(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start1).count()) << std::endl;
	
	std::fstream ff(fileName, std::ios::in);
	if (!ff)
		throw IOException(loggerName + " Cannot open file for reading: " + fileName);

	std::cout << "verifying log file..." << std::endl;

	log.WaitQueue(); // wait till separate thread writes all log messages to file

	std::string ln;
	size_t n;
	for (size_t i = 0; i < NUM_LOGS; i++)
	{
		std::getline(ff, ln);
		n = ln.find("###");
		std::string seq = ln.substr(n + 3);
		std::string si = std::to_string(i);
		CPPUNIT_ASSERT_EQUAL(seq, si);
	}

	std::cout << "Done" << std::endl;
	
}


void ThreadLogTest::testNONThreadLogMeasureTime1()
{
	std::string fileName = LOG_FILES_FOLDER "testNONThreadLogMeasureTime1.log";
	remove(fileName.c_str());
	Logger& log = GetFileLogger("testNONThreadLogMeasureTime1", fileName);

	auto start1 = std::chrono::high_resolution_clock::now();
	
	const int NUM_LOGS = 100'000;
	for (size_t i = 0; i < NUM_LOGS; i++)
	{
		log.Warn("NON-threaded warning ###" + std::to_string(i));
	}

	auto stop = std::chrono::high_resolution_clock::now();

	std::cout << std::endl << NUM_LOGS << " written in SYNC mode.\n";
	std::cout << "  Excec time: " << millisecToStr(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start1).count()) << std::endl;

	std::fstream ff(fileName, std::ios::in);
	if (!ff)
		throw IOException("[testNONThreadLogMeasureTime1] Cannot open file for reading: " + fileName);

	std::cout << "verifying log file..." << std::endl;
	std::string ln;
	size_t n;
	for (size_t i = 0; i < NUM_LOGS; i++)
	{
		std::getline(ff, ln);
		n = ln.find("###");
		std::string seq = ln.substr(n + 3);
		std::string si = std::to_string(i);
		CPPUNIT_ASSERT_EQUAL(seq, si);
	}

	std::cout << "Done" << std::endl;
}


