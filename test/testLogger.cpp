
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
	std::ignore = mkdir(LOG_FILES_FOLDER);
#else
	mkdir(LOG_FILES_FOLDER, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
}

void LoggerTest::tearDown()
{
	ClearLoggers();
	// free memory allocated in setUp, do other things
}

void LoggerTest::testLog0()
{
	Logger& logger1 = GetStdoutLogger("stdout");

	logger1.Error("int:" + IntToStr((int)sizeof(int)));
	logger1.Warn("long:" + IntToStr((int)sizeof(long)));
	logger1.Info("long int:" + IntToStr((int)sizeof(long int)));

	Logger& logger2 = GetStderrLogger("stderr");

	logger2.Error("long long:" + IntToStr((int)sizeof(long long)));
	logger2.Warn("int long:" + IntToStr((int)sizeof(int long)));
	logger2.Info("long long int:" + IntToStr((int)sizeof(long long int)));

	Logger& logger3 = GetFileLogger("filelogger", LOG_FILES_FOLDER "testLog0.log");

	logger3.Error("error logger msg");
	logger3.Warn("warn logger msg");
	logger3.Info("info logger msg");
}

void LoggerTest::testLog1()
{
	Logger& logger = GetLogger("def");
	StringSink* sink = new StringSink("strsink");
	logger.AddSink(sink);

	std::string s;
	
	logger.Log(" ", Levels::llInfo);
	s = sink->GetOutput();
	CPPUNIT_ASSERT_EQUAL_MESSAGE(s, std::string("I "), cutLog(s));

	sink->Clear();
	logger.Log("", Levels::llInfo);
	s = sink->GetOutput();
	CPPUNIT_ASSERT_EQUAL_MESSAGE(s, std::string("I"), cutLog(s));

	sink->Clear();
	logger.Log("testLog1info", Levels::llInfo);
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

void LoggerTest::testLog2()
{
//#if defined(WIN32) && !defined(__BORLANDC__)
	Logger& logg = GetLogger("testLog2");
	StringSink* sink = new StringSink("strsink");
	logg.AddSink(sink);

	std::string s;

	logg.LogFmt(Levels::llWarning, "");
	s = sink->GetOutput();
	// Warn pattern contains # in the beginning which breaks cutLog(), removing it before passing to cutLog()
	CPPUNIT_ASSERT_EQUAL(std::string("W"), cutLog(s.erase(1, 1)));

	sink->Clear();
	logg.LogFmt(Levels::llWarning, "testLog2warning");
	s = sink->GetOutput();
	CPPUNIT_ASSERT_EQUAL(std::string("WtestLog2warning"), cutLog(s.erase(1, 1)));

	sink->Clear();
	logg.LogFmt(Levels::llError, "testLog2 {}.", "error");
	s = sink->GetOutput();
	CPPUNIT_ASSERT_EQUAL(std::string("EtestLog2 error."), cutLog(s));

	sink->Clear();
	logg.CritFmt("testLog2test {0}. {1}", "critical parameter", 3);
	s = sink->GetOutput();
	CPPUNIT_ASSERT_EQUAL(std::string("*testLog2test critical parameter. 3"), cutLog(s));

	sink->Clear();
	logg.LogFmt(Levels::llDebug, "testLog2debug {}.{}.{}.{}.{}", 1,2,3,4,5);
	s = sink->GetOutput();
	CPPUNIT_ASSERT_EQUAL(std::string(""), cutLog(s));

	sink->Clear();
	logg.LogFmt(Levels::llInfo, "testLog2info {}.{}.{}.{}.{}", 1, 2, "RT", 4, 5.1);
	s = sink->GetOutput();
	std::string ss = cutLog(s);
	CPPUNIT_ASSERT_EQUAL_MESSAGE(s, std::string("ItestLog2info 1.2.RT.4.5.1"), cutLog(s));
//#endif
}

void LoggerTest::testLog3()
{
#if defined(WIN32) && !defined(__BORLANDC__)
	Logger& logg = GetLogger("testLog3");
	StringSink* sink1 = new StringSink("strsink1");
	StringSink* sink2 = new StringSink("strsink2");
	logg.AddSink(sink1);
	logg.AddSink(sink2);

	std::string s1,s2;

	logg.LogFmt(Levels::llWarning, "");
	s1 = sink1->GetOutput();
	s2 = sink2->GetOutput();
	CPPUNIT_ASSERT_EQUAL(s1, s2);
	CPPUNIT_ASSERT_EQUAL(std::string("W"), cutLog(s1.erase(1, 1)));
	CPPUNIT_ASSERT_EQUAL(std::string("W"), cutLog(s2.erase(1, 1)));

	sink1->Clear();
	sink2->Clear();
	logg.LogFmt(Levels::llWarning, "testLog2warning");
	s1 = sink1->GetOutput();
	s2 = sink2->GetOutput();
	CPPUNIT_ASSERT_EQUAL(s1, s2);
	CPPUNIT_ASSERT_EQUAL(std::string("WtestLog2warning"), cutLog(s1.erase(1, 1)));
	CPPUNIT_ASSERT_EQUAL(std::string("WtestLog2warning"), cutLog(s2.erase(1, 1)));

	sink1->Clear();
	sink2->Clear();
	logg.LogFmt(Levels::llError, "testLog2 {}.", "error");
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
	logg.LogFmt(Levels::llDebug, "testLog2debug {}.{}.{}.{}.{}", 1, 2, 3, 4, 5);
	s1 = sink1->GetOutput();
	s2 = sink2->GetOutput();
	CPPUNIT_ASSERT_EQUAL(s1, s2);
	CPPUNIT_ASSERT_EQUAL(std::string(""), cutLog(s1));
	CPPUNIT_ASSERT_EQUAL(std::string(""), cutLog(s2));

	sink1->Clear();
	sink2->Clear();
	logg.LogFmt(Levels::llInfo, "testLog2info {}.{}.{}.{}.{}", 1, 2, "RT", 4, 5.1);
	s1 = sink1->GetOutput();
	s2 = sink2->GetOutput();
	CPPUNIT_ASSERT_EQUAL(s1, s2);
	CPPUNIT_ASSERT_EQUAL(std::string("ItestLog2info 1.2.RT.4.5.1"), cutLog(s1));
	CPPUNIT_ASSERT_EQUAL(std::string("ItestLog2info 1.2.RT.4.5.1"), cutLog(s2));
#endif
}

void LoggerTest::testLog4()
{
	Logger& logg = GetLogger("testLog4");
	logg.SetLogLevel(Levels::llInfo);
	StringSink* sink1 = new StringSink("strsink1");
	sink1->SetLogLevel(Levels::llError);
	StringSink* sink2 = new StringSink("strsink2");
	sink2->SetLogLevel(Levels::llDebug);
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
	logg.Log("testLog4warning test7", Levels::llOff);
	s1 = sink1->GetOutput();
	s2 = sink2->GetOutput();
	CPPUNIT_ASSERT_EQUAL(s1, s2);
	CPPUNIT_ASSERT_EQUAL(std::string(" testLog4warning test7"), cutLog(s1));
	CPPUNIT_ASSERT_EQUAL(std::string(" testLog4warning test7"), cutLog(s2));
}

// test case: when two file sinks try to write into the same file. in this case exception is thrown for the second sink.
void LoggerTest::testLog5()
{
	FileSink *fs1;
	CPPUNIT_ASSERT_NO_THROW(fs1 = new FileSink("filesink1", LOG_FILES_FOLDER "testLog5.log"));
 	CPPUNIT_ASSERT_THROW(new FileSink("filesink1", LOG_FILES_FOLDER "testLog5.log"), IOException);
	CPPUNIT_ASSERT_THROW(GetFileLogger("testLog5", LOG_FILES_FOLDER "testLog5.log"), IOException);

	CPPUNIT_ASSERT_NO_THROW(GetFileLogger("testLog5", LOG_FILES_FOLDER "testLog5_2.log"));
	CPPUNIT_ASSERT_NO_THROW(GetFileLogger("testLog5", LOG_FILES_FOLDER "testLog5_2.log"));

	delete fs1;
}

void LoggerTest::testLogStrategyNone()
{
	std::string fileName = LOG_FILES_FOLDER "testStrategyNone.log";

	// preparing parameters
	Properties prop;
	prop.SetValue("ApplicationName", "testLogBackupTypeNone()");
	prop.SetValue("version", "3.3.3");
	prop.SetValue("backuptype", "None");
	prop.SetValue("maxlogsize", "1");
	prop.SetValue("logfilename", fileName);
	prop.SetValue("InfoLine",  "%TIME% : %MSG%"); // fixed length pattern needed for this test to run properly on Win and Linux
	prop.SetValue("ErrorLine", "%TIME% : %MSG%"); // fixed length pattern needed for this test to run properly on Win and Linux

	
	remove(fileName.c_str());
	
	Logger& logger = GetLogger("StrategyNone");
	auto rsink = new RotatingFileSink("rsNone", fileName, 1024, rsNone);
	auto layout = new PatternLayout();
	layout->SetAllPatterns("%TIME% : %MSG%");
	rsink->SetLayout(layout);
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
	CPPUNIT_ASSERT_EQUAL(45ull, rsink->GetMessageCounts()[Levels::llError]);

	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[Levels::llWarning]);
	CPPUNIT_ASSERT_EQUAL(1ull, rsink->GetMessageCounts()[Levels::llInfo]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[Levels::llDebug]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[Levels::llTrace]);
}


void LoggerTest::testLogStrategySingle()
{
	std::string fileName = LOG_FILES_FOLDER "testStrategySingle.log";

	remove(fileName.c_str());
	remove((fileName + BackupExt).c_str());

	Logger& logger = GetLogger("StrategySingle");
	auto rsink = new RotatingFileSink("rsSingle", fileName, 1024, rsSingle);
	auto layout = new PatternLayout();
	layout->SetAllPatterns("%TIME% : %MSG%");
	rsink->SetLayout(layout);
	logger.AddSink(rsink);

	CPPUNIT_ASSERT_EQUAL(1024ull, rsink->GetMaxLogSize());
	CPPUNIT_ASSERT_EQUAL(rsSingle, rsink->GetStrategy());
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetBytesWritten());
	//CPPUNIT_ASSERT_EQUAL(79ul, log->GetTotalBytesWritten());
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

	CPPUNIT_ASSERT_EQUAL(45ull, rsink->GetMessageCounts()[Levels::llError]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[Levels::llWarning]);
	CPPUNIT_ASSERT_EQUAL(1ull, rsink->GetMessageCounts()[Levels::llInfo]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[Levels::llDebug]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[Levels::llTrace]);
}


void LoggerTest::testLogStrategyTimeStamp()
{
	std::string fileName = LOG_FILES_FOLDER "testStrategyTimeStamp.log";

	remove(fileName.c_str());
	
	Logger& logger = GetLogger("StrategyTimeStamp");
	auto rsink = new RotatingFileSink("rsTimeStamp", fileName, 1024, rsTimeStamp);
	auto layout = new PatternLayout();
	layout->SetAllPatterns("%TIME% : %MSG%");
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

	CPPUNIT_ASSERT_EQUAL(45ull, rsink->GetMessageCounts()[Levels::llError]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[Levels::llWarning]);
	CPPUNIT_ASSERT_EQUAL(1ull, rsink->GetMessageCounts()[Levels::llInfo]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[Levels::llDebug]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[Levels::llTrace]);
}

void LoggerTest::testLogStrategyBakNumber()
{
	std::string fileName = LOG_FILES_FOLDER "testStrategyBakNumber.log";

	remove(fileName.c_str());

	Logger& logger = GetLogger("StrategyBakNumber");
	auto rsink = new RotatingFileSink("rsNumbers", fileName, 1024, rsNumbers);
	auto layout = new PatternLayout();
	layout->SetAllPatterns("%TIME% : %MSG%");
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

	logger.Info("L"); // existing log file will be renamed to new name with time stamp 
	CPPUNIT_ASSERT_EQUAL(14ull, rsink->GetBytesWritten());
	//CPPUNIT_ASSERT_EQUAL((ullong)st.st_size + 14ul, log->GetTotalBytesWritten());

	CPPUNIT_ASSERT_EQUAL(true, std::filesystem::exists(StripFileExt(fileName) + ".1" + BackupExt));

	CPPUNIT_ASSERT_EQUAL(45ull, rsink->GetMessageCounts()[Levels::llError]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[Levels::llWarning]);
	CPPUNIT_ASSERT_EQUAL(1ull, rsink->GetMessageCounts()[Levels::llInfo]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[Levels::llDebug]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[Levels::llTrace]);

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

	logger.Info("L"); // existing log file will be renamed to new name with time stamp 
	CPPUNIT_ASSERT_EQUAL(14ull, rsink->GetBytesWritten());
	//CPPUNIT_ASSERT_EQUAL((ullong)st.st_size + 14ul, log->GetTotalBytesWritten());

	CPPUNIT_ASSERT_EQUAL(true, std::filesystem::exists(StripFileExt(fileName) + ".1" + BackupExt));
	CPPUNIT_ASSERT_EQUAL(true, std::filesystem::exists(StripFileExt(fileName) + ".2" + BackupExt));

	CPPUNIT_ASSERT_EQUAL(89ull, rsink->GetMessageCounts()[Levels::llError]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[Levels::llWarning]);
	CPPUNIT_ASSERT_EQUAL(2ull, rsink->GetMessageCounts()[Levels::llInfo]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[Levels::llDebug]);
	CPPUNIT_ASSERT_EQUAL(0ull, rsink->GetMessageCounts()[Levels::llTrace]);
}

void LoggerTest::testLogStatistic()
{
	Logger& log = GetLogger("testLogStatistic");
	log.SetLogLevel(Levels::llTrace);
	auto fsink = new FileSink("fsink", "testLogStatistic.log");
	fsink->SetLogLevel(Levels::llTrace);
	log.AddSink(fsink);

	CPPUNIT_ASSERT_EQUAL(0ull, fsink->GetMessageCounts()[Levels::llCritical]);
	CPPUNIT_ASSERT_EQUAL(0ull, fsink->GetMessageCounts()[Levels::llError]);
	CPPUNIT_ASSERT_EQUAL(0ull, fsink->GetMessageCounts()[Levels::llWarning]);
	CPPUNIT_ASSERT_EQUAL(0ull, fsink->GetMessageCounts()[Levels::llInfo]);
	CPPUNIT_ASSERT_EQUAL(0ull, fsink->GetMessageCounts()[Levels::llDebug]);
	CPPUNIT_ASSERT_EQUAL(0ull, fsink->GetMessageCounts()[Levels::llTrace]);

	log.Crit("C");
	log.Error("E");
	log.Warn("W");
	log.Info("I");
	log.Debug("D");
	log.Trace("T");
	log.Crit("C");
	log.Error("E");
	log.Warn("W");

	CPPUNIT_ASSERT_EQUAL(2ull, fsink->GetMessageCounts()[Levels::llCritical]);
	CPPUNIT_ASSERT_EQUAL(2ull, fsink->GetMessageCounts()[Levels::llError]);
	CPPUNIT_ASSERT_EQUAL(2ull, fsink->GetMessageCounts()[Levels::llWarning]);
	CPPUNIT_ASSERT_EQUAL(1ull, fsink->GetMessageCounts()[Levels::llInfo]);
	CPPUNIT_ASSERT_EQUAL(1ull, fsink->GetMessageCounts()[Levels::llDebug]);
	CPPUNIT_ASSERT_EQUAL(1ull, fsink->GetMessageCounts()[Levels::llTrace]);

	for (size_t i = 0; i < 1001; i++)
	{
		log.Trace("TTTTTTTT");
	}

	CPPUNIT_ASSERT_EQUAL(2ull, fsink->GetMessageCounts()[Levels::llCritical]);
	CPPUNIT_ASSERT_EQUAL(2ull, fsink->GetMessageCounts()[Levels::llError]);
	CPPUNIT_ASSERT_EQUAL(2ull, fsink->GetMessageCounts()[Levels::llWarning]);
	CPPUNIT_ASSERT_EQUAL(1ull, fsink->GetMessageCounts()[Levels::llInfo]);
	CPPUNIT_ASSERT_EQUAL(1ull, fsink->GetMessageCounts()[Levels::llDebug]);
	CPPUNIT_ASSERT_EQUAL(1002ull, fsink->GetMessageCounts()[Levels::llTrace]);
}

void LoggerTest::testGetFileLogger()
{
	Logger& logger1 = GetFileLogger("testGetFileLogger", "filename.log");
	CPPUNIT_ASSERT_EQUAL(1u, logger1.SinkCount());

	Logger& logger2 = GetFileLogger("testGetFileLogger", "filename.log"); // already existing logger has returned
	CPPUNIT_ASSERT_EQUAL(1u, logger2.SinkCount());

	Logger& logger3 = GetFileLogger("testGetFileLogger", "filename2.log"); // already existing logger has returned (with old filename!!!)
	CPPUNIT_ASSERT_EQUAL(1u, logger3.SinkCount());

}

void LoggerTest::testGetStdoutLogger()
{
	Logger& logger1 = GetStdoutLogger("testGetStdoutLogger");
	CPPUNIT_ASSERT_EQUAL(1u, logger1.SinkCount());

	Logger& logger2 = GetStdoutLogger("testGetStdoutLogger"); // already existing logger has returned
	CPPUNIT_ASSERT_EQUAL(1u, logger2.SinkCount());

	Logger& logger3 = GetStdoutLogger("testGetStdoutLogger"); // already existing logger has returned (with old filename!!!)
	CPPUNIT_ASSERT_EQUAL(1u, logger3.SinkCount());

}

void LoggerTest::testGetStderrLogger()
{
	Logger& logger1 = GetStderrLogger("testGetStderrLogger");
	CPPUNIT_ASSERT_EQUAL(1u, logger1.SinkCount());

	Logger& logger2 = GetStderrLogger("testGetStderrLogger"); // already existing logger has returned
	CPPUNIT_ASSERT_EQUAL(1u, logger2.SinkCount());

	Logger& logger3 = GetStderrLogger("testGetStderrLogger"); // already existing logger has returned (with old filename!!!)
	CPPUNIT_ASSERT_EQUAL(1u, logger3.SinkCount());

}

/*
void LoggerTest::testLogEmptyFileName()
{
	// preparing parameters
	Properties prop;
	prop.SetValue("BackupType", "None");
	prop.SetValue("DetailLevel", "DefaultDetailLevel"); 
	prop.SetValue("MaxLogSize", "100");
	prop.SetValue("ApplicationName", "logs/LogEngine tests");
	prop.SetValue("Version", "4.4.4");

	InitLogEngine(prop);

	TLogEngine *log = TLogEngine::getInstance();

	std::string s = log->GetAppName();
	CPPUNIT_ASSERT_EQUAL(std::string("logs/LogEngine tests"), s);
	s = log->GetLogFileName();
	CPPUNIT_ASSERT_EQUAL(std::string("logs/LogEngine tests.log"), s);

	CloseLogEngine();
}



void LoggerTest::testWrong_LFG_File()
{
	try
	{
		InitLogEngine(TEST_FILES_FOLDER "test000.lfg");
	}
	catch (LogException& ex) 
	{
		CPPUNIT_ASSERT_EQUAL((char*)("[LogException] Cannot open config file: " TEST_FILES_FOLDER "test000.lfg"), (char*)ex.what());
	}
 


	InitLogEngine(TEST_FILES_FOLDER "test10.lfg");
	log = getLogEngine();
	CPPUNIT_ASSERT_EQUAL(4, log->GetLogDetailLevel());

	InitLogEngine(TEST_FILES_FOLDER "test11.lfg");
	log = getLogEngine();
	CPPUNIT_ASSERT_EQUAL(7, log->GetLogDetailLevel());

	printf("PASSED\n");
}

void LoggerTest::testBadLFGFile()
{
#ifdef WIN32 // '\n' is not alowed in Windows filenames, but allowed in Linux filenames
	CPPUNIT_ASSERT_THROW( InitLogEngine(TEST_FILES_FOLDER "test13.lfg"), IOException );
#else
	InitLogEngine(TEST_FILES_FOLDER "test13.lfg"); // just make sure that no exception is thrown in contrast to Windows
#endif
	CloseLogEngine();
}

void LoggerTest::testBadLFGFile2()
{
	InitLogEngine(TEST_FILES_FOLDER "test14.lfg");
	TLogEngine* log = TLogEngine::getInstance();

	CPPUNIT_ASSERT_EQUAL(0u, log->GetLogDetailLevel());
	CPPUNIT_ASSERT_EQUAL(lbNone, log->GetBackupType());
	CPPUNIT_ASSERT_EQUAL(1000u, log->GetMaxLogSize());
	CPPUNIT_ASSERT_EQUAL(std::string("d.d.d.d"), log->GetVersionInfo());
	CPPUNIT_ASSERT_EQUAL(std::string("BadLFGFileApp"), log->GetAppName());
	CPPUNIT_ASSERT_EQUAL(std::string("logs/BadLFGLog.log"), log->GetLogFileName());

	CloseLogEngine();
}

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
