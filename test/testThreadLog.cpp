
#include <chrono>
#include <thread>
#include "Shared.h"
#include "Common.h"
#include <LogEngine.h>
#include "testThreadLog.h"


CPPUNIT_TEST_SUITE_REGISTRATION( ThreadLogTest );

using namespace LogEngine;

void ThreadLogTest::setUp ()
{

}

void ThreadLogTest::tearDown ()
{
	ShutdownLoggers();
}

struct ThreadInfoStruct
{
    Logger *logger;
    std::atomic_bool begin;
};

int testThreadProc(void* param)
{
	ThreadInfoStruct *info = (ThreadInfoStruct*) param;
	Logger *log = info->logger;

	if(log == nullptr)
	    return 0; // <<< unused, so 0. (void*) 1 looks silly =)

	while (!info->begin.load()) // waiting for begin
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	
	std::string s = IntToStr(GetThreadID());
	log->Crit(s + " is executing 1.");
	log->Error(s + " is executing 2.");
	log->Info(s + " is executing 3.");
	log->Warn(s + " is executing 4.");
	log->Debug(s + " is executing 5.");
	log->Trace(s + " is executing 6.");

	log->CritFmt("{} is executing {}.", GetThreadID(), "11");
	log->ErrorFmt("{} is executing {}.", GetThreadID(), "22");
	log->WarnFmt("{} is executing {}.", GetThreadID(), "33");
	log->LogFmt(Levels::llInfo, "{} is executing {}.", GetThreadID(), "44");
	log->LogFmt(Levels::llDebug, "{} is executing {}.", GetThreadID(), "55");
	log->LogFmt(Levels::llTrace, "{} is executing {}.", GetThreadID(), "66");

	return 0;
}

#define nthreads 10

void ThreadLogTest::testCallLogFromManyThreads()
{
	// create thread safe logger
	Logger& logger = GetFileLoggerMT("testCallLogFromManyThreads", LOG_FILES_FOLDER "testCallLogFromManyThreads.log");
	
	logger.Info("begin creating threads");
	
	THArray<std::thread*> threads;
	ThreadInfoStruct info = { &logger, false };
	
	logger.Info("Creating threads");
	
	for(uint i = 0; i < nthreads; i++)
	{
		std::thread* thr = new std::thread(testThreadProc, &info);
		threads.AddValue(thr);

		logger.InfoFmt("Created thread #{}", GetThreadID(thr->get_id()));
	}

	logger.Info("all threads created");

	logger.Info("begin resuming");
	
	info.begin.store(true); 
	
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


void ThreadLogTest::testAsyncLog1()
{
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

	log.SetAsyncMode(false); // this call waits till logging thread finishes i.e. saves all log events into file
}


// measure log time when AsynMode=true
void ThreadLogTest::testThreadLogMeasureTime1()
{
	std::string fileName = LOG_FILES_FOLDER "testThreadLogMeasureTime1.log";
	std::string loggerName = "testThreadLogMeasureTime1";

	remove(fileName.c_str());
	Logger& log = GetFileLoggerST(loggerName, fileName); // do not need thread safe logger here

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


// measure log time when AsynMode=false
void ThreadLogTest::testNONThreadLogMeasureTime1()
{
	std::string fileName = LOG_FILES_FOLDER "testNONThreadLogMeasureTime1.log";
	remove(fileName.c_str());
	Logger& log = GetFileLoggerST("testNONThreadLogMeasureTime1", fileName); // do not need thread safe logger here

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


