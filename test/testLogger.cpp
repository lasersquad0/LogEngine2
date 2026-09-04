
#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#include <stdarg.h>
#endif 

#include "Shared.h"
#include "LogEngine.h"
#include "testLogger.h"
#include "Properties.h"
#include "RotatingFileSink.h"

CPPUNIT_TEST_SUITE_REGISTRATION( LoggerTest);
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(LoggerTest, "LoggerTest");

using namespace LogEngine;

void LoggerTest::setUp()
{
#ifdef WIN32
	std::ignore = _mkdir(LOG_FILES_FOLDER);
#else
	mkdir(LOG_FILES_FOLDER, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
}

void LoggerTest::tearDown()
{
	ShutdownLoggers();
	// free memory allocated in setUp, do other things
}

void LoggerTest::testLog0()
{
	Logger& logger1 = GetStdoutLogger("mystdout");

	logger1.Error("int:" + IntToStr((int)sizeof(int)));
	logger1.Warn("long:" + IntToStr((int)sizeof(long)));
	logger1.Info("long int:" + IntToStr((int)sizeof(long int)));

	Logger& logger2 = GetStderrLogger("mystderr");

	logger2.Error("long long:" + IntToStr((int)sizeof(long long)));
	logger2.Warn("int long:" + IntToStr((int)sizeof(int long)));
	logger2.Info("long long int:" + IntToStr((int)sizeof(long long int)));

	Logger& logger3 = GetFileLogger("myfilelogger", LOG_FILES_FOLDER "testLog0.log");

	logger3.Error("error logger msg");
	logger3.Warn("warn logger msg");
	logger3.Info("info logger msg");
}

// testing logger with StringSink 
void LoggerTest::testLog1()
{
	Logger& logger = GetLogger("def"); // get "empty" logger (logger without any sinks)
	std::shared_ptr<StringSinkST> sink(new StringSinkST("strsink"));
	logger.AddSink(sink);

	std::string s;
	
	logger.Log(" ", llInfo);
	s = sink->GetOutput();
	CPPUNIT_ASSERT_EQUAL_MESSAGE(s, std::string("I "), cutLog(s));

	sink->Clear();
	logger.Log("", llInfo);
	s = sink->GetOutput();
	CPPUNIT_ASSERT_EQUAL_MESSAGE(s, std::string("I"), cutLog(s));

	sink->Clear();
	logger.Log("testLog1info", llInfo);
	s = sink->GetOutput();
	CPPUNIT_ASSERT_EQUAL_MESSAGE(s, std::string("ItestLog1info"), cutLog(s));

	sink->Clear();
	logger.Crit("testLog1critical");
	s = sink->GetOutput();
	CPPUNIT_ASSERT_EQUAL_MESSAGE(s, std::string("*testLog1critical"), cutLog(s));

	sink->Clear();
	logger.Error("testLog1error");
	s = sink->GetOutput();
	CPPUNIT_ASSERT_EQUAL_MESSAGE(s, std::string("EtestLog1error"), cutLog(s));
	
	sink->Clear();
	logger.Trace("testLog1trace"); // does not send anything here because log level=llInfo
	s = sink->GetOutput();
	CPPUNIT_ASSERT_EQUAL_MESSAGE(s, std::string(""), cutLog(s));

	sink->Clear();
	logger.Trace("testLog1debug"); // does not send anything here because log level=llInfo
	s = sink->GetOutput();
	CPPUNIT_ASSERT_EQUAL_MESSAGE(s, std::string(""), cutLog(s));
}

// testing logger with StringSink 
void LoggerTest::testLog2()
{
//#if defined(WIN32) && !defined(__BORLANDC__)
	Logger& logg = GetLogger("testLog2");
	std::shared_ptr<StringSinkMT> sink(new StringSinkMT("strsink"));
	logg.AddSink(sink);

	std::string s;

	logg.LogFmt(llWarning, "");
	s = sink->GetOutput();
	// Warn pattern contains # in the beginning which breaks cutLog(), removing it before passing to cutLog()
	CPPUNIT_ASSERT_EQUAL(std::string("W"), cutLog(s.erase(1, 1)));

	sink->Clear();
	logg.LogFmt(llWarning, "testLog2warning");
	s = sink->GetOutput();
	CPPUNIT_ASSERT_EQUAL(std::string("WtestLog2warning"), cutLog(s.erase(1, 1)));

	sink->Clear();
	logg.LogFmt(llError, "testLog2 {}.", "error");
	s = sink->GetOutput();
	CPPUNIT_ASSERT_EQUAL(std::string("EtestLog2 error."), cutLog(s));

	sink->Clear();
	logg.CritFmt("testLog2test {0}. {1}", "critical parameter", 3);
	s = sink->GetOutput();
	CPPUNIT_ASSERT_EQUAL(std::string("*testLog2test critical parameter. 3"), cutLog(s));

	sink->Clear();
	logg.LogFmt(llDebug, "testLog2debug {}.{}.{}.{}.{}", 1,2,3,4,5);
	s = sink->GetOutput();
	CPPUNIT_ASSERT_EQUAL(std::string(""), cutLog(s));

	sink->Clear();
	logg.LogFmt(llInfo, "testLog2info {}.{}.{}.{}.{}", 1, 2, "RT", 4, 5.1);
	s = sink->GetOutput();
	std::string ss = cutLog(s);
	CPPUNIT_ASSERT_EQUAL_MESSAGE(s, std::string("ItestLog2info 1.2.RT.4.5.1"), cutLog(s));
//#endif
}

// testing logger with StringSink 
void LoggerTest::testLog3()
{
	//TODO implement version of test compatible with BORLANDC
#if defined(WIN32) && !defined(__BORLANDC__) 
	Logger& logg = GetLogger("testLog3");
	std::shared_ptr<StringSinkST>sink1(new StringSinkST("strsink1"));
	std::shared_ptr<StringSinkST>sink2(new StringSinkST("strsink2"));
	logg.AddSink(sink1);
	logg.AddSink(sink2);

	std::string s1,s2;

	logg.LogFmt(llWarning, "");
	s1 = sink1->GetOutput();
	s2 = sink2->GetOutput();
	CPPUNIT_ASSERT_EQUAL(s1, s2);
	CPPUNIT_ASSERT_EQUAL(std::string("W"), cutLog(s1.erase(1, 1)));
	CPPUNIT_ASSERT_EQUAL(std::string("W"), cutLog(s2.erase(1, 1)));

	sink1->Clear();
	sink2->Clear();
	logg.LogFmt(llWarning, "testLog2warning");
	s1 = sink1->GetOutput();
	s2 = sink2->GetOutput();
	CPPUNIT_ASSERT_EQUAL(s1, s2);
	CPPUNIT_ASSERT_EQUAL(std::string("WtestLog2warning"), cutLog(s1.erase(1, 1)));
	CPPUNIT_ASSERT_EQUAL(std::string("WtestLog2warning"), cutLog(s2.erase(1, 1)));

	sink1->Clear();
	sink2->Clear();
	logg.LogFmt(llError, "testLog2 {}.", "error");
	s1 = sink1->GetOutput();
	s2 = sink2->GetOutput();
	CPPUNIT_ASSERT_EQUAL(s1, s2);
	CPPUNIT_ASSERT_EQUAL(std::string("EtestLog2 error."), cutLog(s1));
	CPPUNIT_ASSERT_EQUAL(std::string("EtestLog2 error."), cutLog(s2));

	sink1->Clear();
	sink2->Clear();
	logg.CritFmt("testLog2test {0}. {1}", "critical parameter", 3);
	s1 = sink1->GetOutput();
	s2 = sink2->GetOutput();
	CPPUNIT_ASSERT_EQUAL(s1, s2);
	CPPUNIT_ASSERT_EQUAL(std::string("*testLog2test critical parameter. 3"), cutLog(s1));
	CPPUNIT_ASSERT_EQUAL(std::string("*testLog2test critical parameter. 3"), cutLog(s2));

	sink1->Clear();
	sink2->Clear();
	logg.LogFmt(llDebug, "testLog2debug {}.{}.{}.{}.{}", 1, 2, 3, 4, 5);
	s1 = sink1->GetOutput();
	s2 = sink2->GetOutput();
	CPPUNIT_ASSERT_EQUAL(s1, s2);
	CPPUNIT_ASSERT_EQUAL(std::string(""), cutLog(s1));
	CPPUNIT_ASSERT_EQUAL(std::string(""), cutLog(s2));

	sink1->Clear();
	sink2->Clear();
	logg.LogFmt(llInfo, "testLog2info {}.{}.{}.{}.{}", 1, 2, "RT", 4, 5.1);
	s1 = sink1->GetOutput();
	s2 = sink2->GetOutput();
	CPPUNIT_ASSERT_EQUAL(s1, s2);
	CPPUNIT_ASSERT_EQUAL(std::string("ItestLog2info 1.2.RT.4.5.1"), cutLog(s1));
	CPPUNIT_ASSERT_EQUAL(std::string("ItestLog2info 1.2.RT.4.5.1"), cutLog(s2));
#endif
}

// testing logger with StringSink 
void LoggerTest::testLog4()
{
	Logger& logg = GetLogger("testLog4");
	logg.SetLogLevel(llInfo);
	std::shared_ptr<StringSinkST> sink1(new StringSinkST("strsink1"));
	sink1->SetLogLevel(llError);
	std::shared_ptr<StringSinkST> sink2(new StringSinkST("strsink2"));
	sink2->SetLogLevel(llDebug);
	logg.AddSink(sink1);
	logg.AddSink(sink2);

	std::string s1, s2;

	logg.Crit("testLog4crit {}.{}");
	s1 = sink1->GetOutput();
	s2 = sink2->GetOutput();
	CPPUNIT_ASSERT_EQUAL(s1, s2);
	CPPUNIT_ASSERT_EQUAL(std::string("*testLog4crit {}.{}"), cutLog(s1));
	CPPUNIT_ASSERT_EQUAL(std::string("*testLog4crit {}.{}"), cutLog(s2));

	sink1->Clear();
	sink2->Clear();
	logg.Error("testLog4error 1234");
	s1 = sink1->GetOutput();
	s2 = sink2->GetOutput();
	CPPUNIT_ASSERT_EQUAL(s1, s2);
	CPPUNIT_ASSERT_EQUAL(std::string("EtestLog4error 1234"), cutLog(s1));
	CPPUNIT_ASSERT_EQUAL(std::string("EtestLog4error 1234"), cutLog(s2));

	sink1->Clear();
	sink2->Clear();
	logg.Warn("testLog4warning test3");
	s1 = sink1->GetOutput();
	s2 = sink2->GetOutput();
	//CPPUNIT_ASSERT_EQUAL(s1, s2);
	CPPUNIT_ASSERT_EQUAL(std::string(""), cutLog(s1));
	CPPUNIT_ASSERT_EQUAL(std::string("WtestLog4warning test3"), cutLog(s2.erase(1, 1)));

	sink1->Clear();
	sink2->Clear();
	logg.Info("testLog4info test4");
	s1 = sink1->GetOutput();
	s2 = sink2->GetOutput();
	//CPPUNIT_ASSERT_EQUAL(s1, s2);
	CPPUNIT_ASSERT_EQUAL(std::string(""), cutLog(s1));
	CPPUNIT_ASSERT_EQUAL(std::string("ItestLog4info test4"), cutLog(s2));

	sink1->Clear();
	sink2->Clear();
	logg.Debug("testLog4warning test5");
	s1 = sink1->GetOutput();
	s2 = sink2->GetOutput();
	//CPPUNIT_ASSERT_EQUAL(s1, s2);
	CPPUNIT_ASSERT_EQUAL(std::string(""), cutLog(s1));
	CPPUNIT_ASSERT_EQUAL(std::string(""), cutLog(s2));

	sink1->Clear();
	sink2->Clear();
	logg.Debug("testLog4warning test6");
	s1 = sink1->GetOutput();
	s2 = sink2->GetOutput();
	CPPUNIT_ASSERT_EQUAL(s1, s2);
	CPPUNIT_ASSERT_EQUAL(std::string(""), cutLog(s1));
	CPPUNIT_ASSERT_EQUAL(std::string(""), cutLog(s2));

	sink1->Clear();
	sink2->Clear();
	logg.Log("testLog4warning test7", llOff);
	s1 = sink1->GetOutput();
	s2 = sink2->GetOutput();
	CPPUNIT_ASSERT_EQUAL(s1, s2);
	CPPUNIT_ASSERT_EQUAL(std::string(" testLog4warning test7"), cutLog(s1));
	CPPUNIT_ASSERT_EQUAL(std::string(" testLog4warning test7"), cutLog(s2));
}

// test case: when two file sinks try to write into the same file. in this case exception is thrown for the second sink.
void LoggerTest::testLogTwoSinksOneFile()
{
	FileSinkST *fs1;
	CPPUNIT_ASSERT_NO_THROW(fs1 = new FileSinkST("filesink1", LOG_FILES_FOLDER "testLog5.log"));
 	CPPUNIT_ASSERT_THROW(new FileSinkST("filesink2", LOG_FILES_FOLDER "testLog5.log"), IOException);

	CPPUNIT_ASSERT_EQUAL(false, LoggerExist("testLog5"));

	// logger "testLog5" does not exist, but file "testLog5.log" is locked by file sink "filesink1" => exception thrown
	// anyway empty logger with name "testLog5" has been created
	CPPUNIT_ASSERT_THROW(GetFileLogger("testLog5", LOG_FILES_FOLDER "testLog5.log"), IOException);

	CPPUNIT_ASSERT_EQUAL(true, LoggerExist("testLog5"));
	CPPUNIT_ASSERT_NO_THROW(GetFileLogger("testLog5", LOG_FILES_FOLDER "testLog5_2.log"));
	CPPUNIT_ASSERT_EQUAL(true, LoggerExist("testLog5"));

	// line below does NOT throw an exception because existing logger will be returned
	CPPUNIT_ASSERT_NO_THROW(GetFileLogger("testLog5", LOG_FILES_FOLDER "testLog5_2.log"));

	CPPUNIT_ASSERT_EQUAL(false, LoggerExist("testLog5_2"));
	// and one more time try first file name
	CPPUNIT_ASSERT_THROW(GetFileLogger("testLog5_2", LOG_FILES_FOLDER "testLog5.log"), IOException);
	CPPUNIT_ASSERT_EQUAL(true, LoggerExist("testLog5_2"));
	delete fs1;

	// now file is NOT locked, NO THROW
	CPPUNIT_ASSERT_NO_THROW(GetFileLogger("testLog5_2", LOG_FILES_FOLDER "testLog5.log"));
	CPPUNIT_ASSERT_EQUAL(true, LoggerExist("testLog5_2"));
}

// test case: when two file sinks try to write into the same file. in this case exception is thrown for the second sink.
void LoggerTest::testLogTwoSinksOneFileLock()
{
	FileSinkST* fs1;
	CPPUNIT_ASSERT_NO_THROW(fs1 = new FileLockSinkST("filelocksink1", LOG_FILES_FOLDER "testLogTwoSinksOneFileLock.log"));
	CPPUNIT_ASSERT_THROW(new FileSinkST("filelocksink2", LOG_FILES_FOLDER "testLogTwoSinksOneFileLock.log"), IOException);
	CPPUNIT_ASSERT_THROW(GetFileLogger("testLogTwoSinksOneFileLock", LOG_FILES_FOLDER "testLogTwoSinksOneFileLock.log"), IOException);
	
	// anyway empty logger with name "testLogTwoSinksOneFileLock" has been created
	CPPUNIT_ASSERT_EQUAL(true, LoggerExist("testLogTwoSinksOneFileLock"));

	CPPUNIT_ASSERT_NO_THROW(GetFileLogger("testLogTwoSinksOneFileLock", LOG_FILES_FOLDER "testLogTwoSinksOneFileLock_2.log"));
	//line below does NOT throw an exception because existing logger will be returned
	CPPUNIT_ASSERT_NO_THROW(GetFileLogger("testLogTwoSinksOneFileLock", LOG_FILES_FOLDER "testLogTwoSinksOneFileLock_2.log"));
	// and one more time
	CPPUNIT_ASSERT_NO_THROW(GetFileLogger("testLogTwoSinksOneFileLock", LOG_FILES_FOLDER "testLogTwoSinksOneFileLock_2.log"));
    // and one more time try first file name, logger name should be different here
	CPPUNIT_ASSERT_THROW(GetFileLogger("testLogTwoSinksOneFileLock2", LOG_FILES_FOLDER "testLogTwoSinksOneFileLock.log"), IOException);

	// anyway empty logger with name "testLogTwoSinksOneFileLock2" has been created
	CPPUNIT_ASSERT_EQUAL(true, LoggerExist("testLogTwoSinksOneFileLock2"));

	delete fs1;
}

// various scenarios when one logger contains different sinks including duplicates
void LoggerTest::testLogDuplicateSinks1()
{
	std::shared_ptr<Sink> consoleSink(new LogEngine::StdoutSinkST("consolesink"));
	consoleSink->SetPattern("[testLogMultiSink][consolesink] %loglevel% %Msg%");

	std::shared_ptr<Sink> fileSink(new LogEngine::FileSinkMT("filesink", LOG_FILES_FOLDER "multisink.txt"));
	fileSink->SetPattern("[testLogMultiSink][filesink] [%loglevel%] [%Msg%]");
	
	LogEngine::Logger logger("multisink", { fileSink, consoleSink, consoleSink, fileSink }); // actually two sinks are added to the logger instead to four
	CPPUNIT_ASSERT_EQUAL(2u, logger.SinkCount());

	CPPUNIT_ASSERT_EQUAL(llInfo, logger.GetLogLevel());
	CPPUNIT_ASSERT_EQUAL(llInfo, fileSink->GetLogLevel());
	CPPUNIT_ASSERT_EQUAL(llInfo, consoleSink->GetLogLevel());

	logger.SetLogLevel(llDebug, true); // this log level propagates to all logger sinks
	
	CPPUNIT_ASSERT_EQUAL(llDebug, logger.GetLogLevel());
	CPPUNIT_ASSERT_EQUAL(llDebug, fileSink->GetLogLevel());
	CPPUNIT_ASSERT_EQUAL(llDebug, consoleSink->GetLogLevel());

	logger.Debug("MSG#1: should appear in both console and file one time each");
}

// multi logger with duplicate sinks. diplicate sink will not be added into the logger 
void LoggerTest::testLogDuplicateSinks2()
{
	std::shared_ptr<Sink> consoleSink(new LogEngine::StdoutSinkST("consolesink"));
	consoleSink->SetPattern("[testLogMultiSink][consolesink] %loglevel% %Msg%");

	std::shared_ptr<Sink> stdoutSink(new LogEngine::StdoutSinkST("stdoutsink"));
	stdoutSink->SetPattern("[testLogMultiSink][stdoutsink] [%loglevel%] [%Msg%]");

	std::shared_ptr<Sink> errSink(new LogEngine::StderrSinkST("ERRsink"));
	errSink->SetPattern("[testLogMultiSink][ERRsink] (%loglevel%) [%Msg%]");

	auto& logger = LogEngine::GetMultiLogger("multisink", { stdoutSink, errSink, consoleSink, consoleSink, errSink, stdoutSink });
	CPPUNIT_ASSERT_EQUAL(uint(3), logger.SinkCount());

	CPPUNIT_ASSERT_EQUAL(llInfo, logger.GetLogLevel());
	CPPUNIT_ASSERT_EQUAL(llInfo, consoleSink->GetLogLevel());
	CPPUNIT_ASSERT_EQUAL(llInfo, errSink->GetLogLevel());
	CPPUNIT_ASSERT_EQUAL(llInfo, stdoutSink->GetLogLevel());

	logger.SetLogLevel(llDebug, true); // this log level propagates to all logger sinks
	
	CPPUNIT_ASSERT_EQUAL(llDebug, logger.GetLogLevel());
	CPPUNIT_ASSERT_EQUAL(llDebug, errSink->GetLogLevel());
	CPPUNIT_ASSERT_EQUAL(llDebug, consoleSink->GetLogLevel());
	CPPUNIT_ASSERT_EQUAL(llDebug, stdoutSink->GetLogLevel());

	logger.Debug("MSG#2: should appear in all three sinks: console, stdout and stderr by one time each.");
}

// adding duplicate sinks to the logger. diplicate sink will not be added into the logger 
void LoggerTest::testLogDuplicateSinks3()
{
	std::shared_ptr<Sink>consoleSink(new LogEngine::StdoutSinkMT("consolesink"));
	consoleSink->SetPattern("[testLogMultiSink][consolesink] %loglevel% %Msg%");

	std::shared_ptr<Sink> fileSink(new LogEngine::FileSinkMT("filesink", LOG_FILES_FOLDER "multisink.txt"));
	fileSink->SetPattern("[testLogMultiSink][filesink] [%loglevel%] [%Msg%]");

	SinkList slist;
	slist.AddValue(consoleSink);
	slist.AddValue(fileSink);
	slist.AddValue(consoleSink);
	slist.AddValue(fileSink);
	CPPUNIT_ASSERT_EQUAL(4u, slist.Count());

	auto& logger = LogEngine::GetMultiLogger("multisink", slist);
	CPPUNIT_ASSERT_EQUAL(2u, logger.SinkCount());

	CPPUNIT_ASSERT_EQUAL(llInfo, logger.GetLogLevel());
	CPPUNIT_ASSERT_EQUAL(llInfo, consoleSink->GetLogLevel());
	CPPUNIT_ASSERT_EQUAL(llInfo, fileSink->GetLogLevel());

	logger.SetLogLevel(llDebug, true); // this log level propagates to all logger sinks
	
	CPPUNIT_ASSERT_EQUAL(llDebug, logger.GetLogLevel());
	CPPUNIT_ASSERT_EQUAL(llDebug, consoleSink->GetLogLevel());
	CPPUNIT_ASSERT_EQUAL(llDebug, fileSink->GetLogLevel());

	logger.Debug("MSG#3: should appear in both console and file one time each");
}
void LoggerTest::testLogStrategyNone()
{
	std::string fileName = LOG_FILES_FOLDER "testStrategyNone.log";

	// preparing parameters
	/*Properties prop;
	prop.SetValue("ApplicationName", "testLogBackupTypeNone()");
	prop.SetValue("version", "3.3.3");
	prop.SetValue("backuptype", "None");
	prop.SetValue("maxlogsize", "1");
	prop.SetValue("logfilename", fileName);
	prop.SetValue("InfoLine",  "%TIME% : %MSG%"); // fixed length pattern needed for this test to run properly on Win and Linux
	prop.SetValue("ErrorLine", "%TIME% : %MSG%"); // fixed length pattern needed for this test to run properly on Win and Linux
	*/
	
	remove(fileName.c_str());
	
	Logger& logger = GetLogger("StrategyNone");
	std::shared_ptr<RotatingFileSinkST> rsink(new RotatingFileSinkST("rsNoneFileSink", fileName, 1024, rsNone));
	//auto layout = new PatternLayout();
	rsink->SetPattern("%TIME% : %MSG%");
	//rsink->SetLayout(layout);
	logger.AddSink(rsink);
	
	CPPUNIT_ASSERT_EQUAL(1024ull, rsink->GetMaxLogSize());
	CPPUNIT_ASSERT_EQUAL(rsNone, rsink->GetStrategy());
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetBytesWritten());
	//CPPUNIT_ASSERT_EQUAL(79ul, log->GetTotalBytesWritten());

	while(true)
	{
		logger.Error("1234567890");
		if(rsink->GetBytesWritten() > 1024)
			break;
	}
	
	rsink->Flush();
	struct stat st;
	stat(fileName.c_str(), &st);

	CPPUNIT_ASSERT_EQUAL((ullong)st.st_size, rsink->GetBytesWritten());
	
	logger.Info("L"); // file will be truncated and re-written from beginning (because BackupType=lbNone)
	//CPPUNIT_ASSERT_EQUAL((ullong)st.st_size + 14ul, log->GetTotalBytesWritten());

	// it will be 13ull and 47ull under Linux because of \r\n and \n
	CPPUNIT_ASSERT_EQUAL(14ull, rsink->GetBytesWritten()); 
	CPPUNIT_ASSERT_EQUAL(45ull, rsink->GetMessageCounts()[llError]);

	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[llWarning]);
	CPPUNIT_ASSERT_EQUAL(1ull, rsink->GetMessageCounts()[llInfo]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[llDebug]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[llTrace]);
}


void LoggerTest::testLogStrategySingle()
{
	std::string fileName = LOG_FILES_FOLDER "testStrategySingle.log";

	remove(fileName.c_str());
	remove((fileName + BackupExt).c_str());

	Logger& logger = GetLogger("StrategySingle");
	std::shared_ptr<RotatingFileSinkMT> rsink(new RotatingFileSinkMT("rsSingle", fileName, 1024, rsSingle));
	//auto layout = new PatternLayout();
	rsink->SetPattern("%TIME% : %MSG%");
	//rsink->SetLayout(layout);
	logger.AddSink(rsink);
	
	CPPUNIT_ASSERT_EQUAL(1024ull, rsink->GetMaxLogSize());
	CPPUNIT_ASSERT_EQUAL(rsSingle, rsink->GetStrategy());
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetBytesWritten());
	//CPPUNIT_ASSERT_EQUAL(0ull, logger->GetTotalBytesWritten());
	CPPUNIT_ASSERT_EQUAL(false , std::filesystem::exists(fileName + BackupExt));
	
	while (true)
	{
		logger.Error("1234567890");
		if (rsink->GetBytesWritten() > 1024)
			break;
	}

	rsink->Flush();
	struct stat st;
	stat(fileName.c_str(), &st);

	CPPUNIT_ASSERT_EQUAL((ullong)st.st_size, rsink->GetBytesWritten());

	logger.Info("L"); // file will be renamed to .bak and new log file created
	CPPUNIT_ASSERT_EQUAL(14ull, rsink->GetBytesWritten());
	//CPPUNIT_ASSERT_EQUAL((ullong)st.st_size + 14ul, log->GetTotalBytesWritten());

	CPPUNIT_ASSERT_EQUAL(true, std::filesystem::exists(fileName + BackupExt));
	CPPUNIT_ASSERT_EQUAL(true, std::filesystem::exists(fileName));

	CPPUNIT_ASSERT_EQUAL(45ull, rsink->GetMessageCounts()[llError]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[llWarning]);
	CPPUNIT_ASSERT_EQUAL(1ull, rsink->GetMessageCounts()[llInfo]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[llDebug]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[llTrace]);
}


void LoggerTest::testLogStrategyTimeStamp()
{
	std::string fileName = LOG_FILES_FOLDER "testStrategyTimeStamp.txt"; //nonstandard file extension, should be preserved in timestamp files

	remove(fileName.c_str());
	
	Logger& logger = GetLogger("StrategyTimeStamp");
	std::shared_ptr<RotatingFileSinkST> rsink(new RotatingFileSinkST("rsTimeStamp", fileName, 1024, rsTimeStamp));
	auto layout = new PatternLayout();
	layout->SetPattern("%TIME% : %MSG%");
	rsink->SetLayout(layout);
	logger.AddSink(rsink);

	CPPUNIT_ASSERT_EQUAL(1024ull, rsink->GetMaxLogSize());
	CPPUNIT_ASSERT_EQUAL(rsTimeStamp, rsink->GetStrategy());
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetBytesWritten());
	//CPPUNIT_ASSERT_EQUAL(79ull, log->GetTotalBytesWritten());

	while (true)
	{
		logger.Error("1234567890");
		if (rsink->GetBytesWritten() > 1024)
			break;
	}

	rsink->Flush();
	struct stat st;
	stat(fileName.c_str(), &st);

	CPPUNIT_ASSERT_EQUAL((ullong)st.st_size, rsink->GetBytesWritten());

	logger.Info("L"); // existing log file will be renamed to new name with time stamp 
	CPPUNIT_ASSERT_EQUAL(14ull, rsink->GetBytesWritten());
	//CPPUNIT_ASSERT_EQUAL((ullong)st.st_size + 14ul, log->GetTotalBytesWritten());

	CPPUNIT_ASSERT_EQUAL(true, std::filesystem::exists(fileName));

	CPPUNIT_ASSERT_EQUAL(45ull, rsink->GetMessageCounts()[llError]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[llWarning]);
	CPPUNIT_ASSERT_EQUAL(1ull, rsink->GetMessageCounts()[llInfo]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[llDebug]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[llTrace]);
}

void LoggerTest::testLogStrategyBakNumber()
{
	std::string fileName = LOG_FILES_FOLDER "testStrategyBakNumber.log";

	remove(fileName.c_str());

	Logger& logger = GetLogger("StrategyBakNumber");
	std::shared_ptr<RotatingFileSinkST> rsink(new RotatingFileSinkST("rsNumbers", fileName, 1024, rsNumbers));
	auto layout = new PatternLayout();
	layout->SetPattern("%TIME% : %MSG%");
	rsink->SetLayout(layout);
	logger.AddSink(rsink);

	CPPUNIT_ASSERT_EQUAL(1024ull, rsink->GetMaxLogSize());
	CPPUNIT_ASSERT_EQUAL(rsNumbers, rsink->GetStrategy());
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetBytesWritten());
	//CPPUNIT_ASSERT_EQUAL(79ull, log->GetTotalBytesWritten());

	// generate first .bak file
	while (true)
	{
		logger.Error("1234567890");
		if (rsink->GetBytesWritten() > 1024)
			break;
	}

	rsink->Flush();
	struct stat st;
	stat(fileName.c_str(), &st);
	CPPUNIT_ASSERT_EQUAL((ullong)st.st_size, rsink->GetBytesWritten());

	logger.Info("L"); // existing log file will be renamed to new name with index '1' in name
	CPPUNIT_ASSERT_EQUAL(14ull, rsink->GetBytesWritten());
	//CPPUNIT_ASSERT_EQUAL((ullong)st.st_size + 14ul, log->GetTotalBytesWritten());

	CPPUNIT_ASSERT_EQUAL(true, std::filesystem::exists(StripFileExt(fileName) + ".1" + BackupExt));

	CPPUNIT_ASSERT_EQUAL(45ull, rsink->GetMessageCounts()[llError]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[llWarning]);
	CPPUNIT_ASSERT_EQUAL(1ull, rsink->GetMessageCounts()[llInfo]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[llDebug]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[llTrace]);

	// generate second .bak file
	while (true)
	{
		logger.Error("1234567890");
		if (rsink->GetBytesWritten() > 1024)
			break;
	}

	rsink->Flush();
	stat(fileName.c_str(), &st);
	CPPUNIT_ASSERT_EQUAL((ullong)st.st_size, rsink->GetBytesWritten());

	logger.Info("L"); // existing log file will be renamed to new name with index '2' in name
	CPPUNIT_ASSERT_EQUAL(14ull, rsink->GetBytesWritten());
	//CPPUNIT_ASSERT_EQUAL((ullong)st.st_size + 14ul, log->GetTotalBytesWritten());

	CPPUNIT_ASSERT_EQUAL(true, std::filesystem::exists(StripFileExt(fileName) + ".1" + BackupExt));
	CPPUNIT_ASSERT_EQUAL(true, std::filesystem::exists(StripFileExt(fileName) + ".2" + BackupExt));

	CPPUNIT_ASSERT_EQUAL(89ull, rsink->GetMessageCounts()[llError]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[llWarning]);
	CPPUNIT_ASSERT_EQUAL(2ull, rsink->GetMessageCounts()[llInfo]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[llDebug]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[llTrace]);
}

void LoggerTest::testLogStatistic()
{
	Logger& log = GetLogger("testLogStatistic");
	log.SetLogLevel(llTrace);
	std::shared_ptr<FileSinkMT> fsink(new FileSinkMT("fsink", LOG_FILES_FOLDER "testLogStatistic.log"));
	fsink->SetLogLevel(llTrace);
	auto layout = new PatternLayout();
	layout->SetPattern("%TIME% : %MSG%");
	fsink->SetLayout(layout);
	log.AddSink(fsink);

	CPPUNIT_ASSERT_EQUAL(0ull, fsink->GetMessageCounts()[llCritical]);
	CPPUNIT_ASSERT_EQUAL(0ull, fsink->GetMessageCounts()[llError]);
	CPPUNIT_ASSERT_EQUAL(0ull, fsink->GetMessageCounts()[llWarning]);
	CPPUNIT_ASSERT_EQUAL(0ull, fsink->GetMessageCounts()[llInfo]);
	CPPUNIT_ASSERT_EQUAL(0ull, fsink->GetMessageCounts()[llDebug]);
	CPPUNIT_ASSERT_EQUAL(0ull, fsink->GetMessageCounts()[llTrace]);
	CPPUNIT_ASSERT_EQUAL(0ull, fsink->GetBytesWritten());

	log.Crit("C");
	log.Error("E");
	log.Warn("W");
	log.Info("I");
	log.Debug("D");
	log.Trace("T");
	log.Crit("C");
	log.Error("E");
	log.Warn("W");

	CPPUNIT_ASSERT_EQUAL(2ull, fsink->GetMessageCounts()[llCritical]);
	CPPUNIT_ASSERT_EQUAL(2ull, fsink->GetMessageCounts()[llError]);
	CPPUNIT_ASSERT_EQUAL(2ull, fsink->GetMessageCounts()[llWarning]);
	CPPUNIT_ASSERT_EQUAL(1ull, fsink->GetMessageCounts()[llInfo]);
	CPPUNIT_ASSERT_EQUAL(1ull, fsink->GetMessageCounts()[llDebug]);
	CPPUNIT_ASSERT_EQUAL(1ull, fsink->GetMessageCounts()[llTrace]);
	CPPUNIT_ASSERT_EQUAL(126ull, fsink->GetBytesWritten());

	for (size_t i = 0; i < 1001; i++)
	{
		log.Trace("TTTTTTTT");
	}

	CPPUNIT_ASSERT_EQUAL(2ull, fsink->GetMessageCounts()[llCritical]);
	CPPUNIT_ASSERT_EQUAL(2ull, fsink->GetMessageCounts()[llError]);
	CPPUNIT_ASSERT_EQUAL(2ull, fsink->GetMessageCounts()[llWarning]);
	CPPUNIT_ASSERT_EQUAL(1ull, fsink->GetMessageCounts()[llInfo]);
	CPPUNIT_ASSERT_EQUAL(1ull, fsink->GetMessageCounts()[llDebug]);
	CPPUNIT_ASSERT_EQUAL(1002ull, fsink->GetMessageCounts()[llTrace]);
	CPPUNIT_ASSERT_EQUAL(21147ull, fsink->GetBytesWritten());

}

void LoggerTest::testGetFileLogger()
{
	CPPUNIT_ASSERT_EQUAL(0u, LoggersCount());
	Logger& logger1 = GetFileLogger("testGetFileLogger", "filename.log");
	CPPUNIT_ASSERT_EQUAL(1u, logger1.SinkCount());
	CPPUNIT_ASSERT_EQUAL(1u, LoggersCount());

	Logger& logger2 = GetFileLoggerMT("testGetFileLogger", "filename.log"); // already existing logger has returned
	CPPUNIT_ASSERT_EQUAL(1u, logger2.SinkCount());
	CPPUNIT_ASSERT_EQUAL(1u, LoggersCount());

	Logger& logger3 = GetFileLogger("testGetFileLogger", "filename2.log"); // already existing logger has returned (with old filename!!!)
	CPPUNIT_ASSERT_EQUAL(1u, logger3.SinkCount());
	CPPUNIT_ASSERT_EQUAL(1u, LoggersCount());
}

void LoggerTest::testGetStdoutLogger()
{
	CPPUNIT_ASSERT_EQUAL(0u, LoggersCount());
	Logger& logger1 = GetStdoutLogger("testGetStdoutLogger");
	CPPUNIT_ASSERT_EQUAL(1u, logger1.SinkCount());
	CPPUNIT_ASSERT_EQUAL(1u, LoggersCount());

	Logger& logger2 = GetStdoutLogger("testGetStdoutLogger"); // already existing logger has returned
	CPPUNIT_ASSERT_EQUAL(1u, logger2.SinkCount());
	CPPUNIT_ASSERT_EQUAL(1u, LoggersCount());

	Logger& logger3 = GetStdoutLoggerMT("testGetStdoutLogger"); // already existing logger has returned
	CPPUNIT_ASSERT_EQUAL(1u, logger3.SinkCount());
	CPPUNIT_ASSERT_EQUAL(1u, LoggersCount());

}

void LoggerTest::testGetStderrLogger()
{
	CPPUNIT_ASSERT_EQUAL(0u, LoggersCount());
	Logger& logger1 = GetStderrLoggerMT("testGetStderrLogger");
	CPPUNIT_ASSERT_EQUAL(1u, logger1.SinkCount());
	CPPUNIT_ASSERT_EQUAL(1u, LoggersCount());

	Logger& logger2 = GetStderrLogger("testGetStderrLogger"); // already existing logger has returned
	CPPUNIT_ASSERT_EQUAL(1u, logger2.SinkCount());
	CPPUNIT_ASSERT_EQUAL(1u, LoggersCount());

	Logger& logger3 = GetStderrLogger("testGetStderrLogger"); // already existing logger has returned 
	CPPUNIT_ASSERT_EQUAL(1u, logger3.SinkCount());
	CPPUNIT_ASSERT_EQUAL(1u, LoggersCount());
}

// file logger, async=false, ST- threading sync is OFF
void LoggerTest::testLogPerfromanceST_ASyncFalse()
{
	// for clear benchmarking we generate strings to log to before starting a timer
	const int MESS_NUM = 1'000'000;
	auto arr = new std::string[MESS_NUM];
	for (size_t i = 0; i < MESS_NUM; i++)
	{
		if(i % 2 == 0)
		  arr[i] = "Log message #" + std::to_string(i);
		else
   		  arr[i] = "Long information message for benchmarking purposes #" + std::to_string(i);
	}

	// remove previous file if any
	std::string fileName = LOG_FILES_FOLDER "BenchmarkST1.log";
	remove(fileName.c_str());

	auto start = std::chrono::high_resolution_clock::now();

	auto& logger = GetFileLoggerST("performanceST1", fileName);
	logger.SetAsyncMode(false);

	for (size_t i = 0; i < MESS_NUM; i++)
	{
		logger.Info(arr[i]);
	}

	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << std::format(" Writing {} messages to log file. Time (sync, ST): {}", MESS_NUM, MillisecToStr<std::string>(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()));

	delete[] arr;
}

// file logger, async=TRUE, MT- threading sync is OFF
void LoggerTest::testLogPerfromanceST_ASyncTrue()
{
	// for clear benchmarking we generate strings to log to before starting a timer
	const int MESS_NUM = 1'000'000;
	auto arr = new std::string[MESS_NUM];
	for (size_t i = 0; i < MESS_NUM; i++)
	{
		if (i % 2 == 0)
			arr[i] = "Log message #" + std::to_string(i);
		else
			arr[i] = "Long information message for benchmarking purposes #" + std::to_string(i);
	}

	// remove previous file if any
	std::string fileName = LOG_FILES_FOLDER "BenchmarkST2.log";
	remove(fileName.c_str());

	auto start = std::chrono::high_resolution_clock::now();

	auto& logger = GetFileLoggerST("performanceST2", fileName);
	logger.SetAsyncMode(true);

	for (size_t i = 0; i < MESS_NUM; i++)
	{
		logger.Info(arr[i]);
	}

	logger.SetAsyncMode(false); // send stop signal to async thread and wait till its finishes execution

	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << std::format(" Writing {:L} messages to log file. Time (async, ST): {}", MESS_NUM, MillisecToStr<std::string>(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()));

	delete[] arr;
}

// file logger, async=false, MT- threading support is ON
void LoggerTest::testLogPerfromanceMT_ASyncFalse()
{
	// for clear benchmarking we generate strings to log to before starting a timer
	const int MESS_NUM = 1'000'000;
	auto arr = new std::string[MESS_NUM];
	for (size_t i = 0; i < MESS_NUM; i++)
	{
		if (i % 2 == 0)
			arr[i] = "Log message #" + std::to_string(i);
		else
			arr[i] = "Long information message for benchmarking purposes #" + std::to_string(i);
	}
	
	// remove previous file if any
	std::string fileName = LOG_FILES_FOLDER "BenchmarkMT1.log";
	remove(fileName.c_str());

	auto start = std::chrono::high_resolution_clock::now();

	auto& logger = GetFileLoggerMT("performanceMT1", fileName);
	logger.SetAsyncMode(false);

	for (size_t i = 0; i < MESS_NUM; i++)
	{
		logger.Info(arr[i]);
	}

	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << std::format(" Writing {:L} messages to log file. Time (sync, MT): {}", MESS_NUM, MillisecToStr<std::string>(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()));

	delete[] arr;
}

// file logger, async=false, MT- threading support is ON
void LoggerTest::testLogPerfromanceLock1()
{
	// for clear benchmarking we generate strings to log to before starting a timer
	const int MESS_NUM = 1'000'000;
	auto arr = new std::string[MESS_NUM];
	for (size_t i = 0; i < MESS_NUM; i++)
	{
		if (i % 2 == 0)
			arr[i] = "Log message #" + std::to_string(i);
		else
			arr[i] = "Long information message for benchmarking purposes #" + std::to_string(i);
	}

	// remove previous file if any
	std::string fileName = LOG_FILES_FOLDER "BenchmarkLock1.log";
	remove(fileName.c_str());

	auto start = std::chrono::high_resolution_clock::now();

	Logger& logger = GetLogger("perfomanceLock1"); // get "empty" logger (logger without any sinks)
	std::shared_ptr<FileLockSinkST> sink(new FileLockSinkST("filelocksink", fileName));
	logger.AddSink(sink);
	logger.SetAsyncMode(false);

	for (size_t i = 0; i < MESS_NUM; i++)
	{
		logger.Info(arr[i]);
	}

	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << std::format(" Writing {:L} messages to log file. Time (sync, LockST): {}", MESS_NUM, MillisecToStr<std::string>(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()));

	delete[] arr;
}

/*

void LoggerTest::testLogRotation1()
{
	std::string logfname = LOG_FILES_FOLDER "TestLogRotation.log";
	remove(logfname.c_str()); // name sure that previously created log file does not exist

	Properties props;
	props.SetValue("LogFileName", logfname);
	props.SetValue("BackupType", "TimeStamp");
	props.SetValue("MaxLogSize", "1"); // 1 Kilobyte
	
	InitLogEngine(props);
	TLogEngine* log = TLogEngine::getInstance();

	CPPUNIT_ASSERT_EQUAL(0u, log->GetMessageCount(lmNone));

	ulong StartMsgLen = 64;
	ulong MaxLogSize = 1024;
	CPPUNIT_ASSERT_EQUAL(StartMsgLen, log->GetBytesWritten());
	CPPUNIT_ASSERT_EQUAL(StartMsgLen, log->GetTotalBytesWritten());

	std::string str;
	str.resize(MaxLogSize - StartMsgLen - 2, 'M'); // we deduc 2 because WriteStr writes str AND 'CRLF' - 3 bytes in total
	log->WriteStr(str); 
	
	CPPUNIT_ASSERT_EQUAL(MaxLogSize, log->GetBytesWritten());
	CPPUNIT_ASSERT_EQUAL(MaxLogSize, log->GetTotalBytesWritten());
	log->WriteStr("F"); // 3 bytes written
	CPPUNIT_ASSERT_EQUAL(MaxLogSize + 3, log->GetBytesWritten());
	CPPUNIT_ASSERT_EQUAL(MaxLogSize + 3, log->GetTotalBytesWritten());
	
	log->WriteStr("A"); // 3 bytes written
	CPPUNIT_ASSERT_EQUAL(3ul, log->GetBytesWritten());
	CPPUNIT_ASSERT_EQUAL(MaxLogSize + 6, log->GetTotalBytesWritten());

// wait to make sure that next backup file will have different name.
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	str.resize(MaxLogSize + 1, 'M');
	log->WriteStr(str);
	CPPUNIT_ASSERT_EQUAL(MaxLogSize + 6, log->GetBytesWritten());
	CPPUNIT_ASSERT_EQUAL(MaxLogSize + 6 + MaxLogSize + 3, log->GetTotalBytesWritten());

	log->WriteStr("G");
	CPPUNIT_ASSERT_EQUAL(3ul, log->GetBytesWritten());
	CPPUNIT_ASSERT_EQUAL(MaxLogSize + 6 + MaxLogSize + 6, log->GetTotalBytesWritten());

	CPPUNIT_ASSERT_EQUAL(5u, log->GetMessageCount(lmNone));
	CPPUNIT_ASSERT_EQUAL(0u, log->GetMessageCount(lmError));
	CPPUNIT_ASSERT_EQUAL(0u, log->GetMessageCount(lmWarning));
	CPPUNIT_ASSERT_EQUAL(0u, log->GetMessageCount(lmInfo));

	CloseLogEngine();
}
*/

#undef TEST_FILES_FOLDER
