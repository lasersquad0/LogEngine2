
#include "Shared.h"
#include "LogEngine.h"
#include "RotatingFileSink.h"
#include "testConfigFile.h"


CPPUNIT_TEST_SUITE_REGISTRATION(ConfigFileTest);


using namespace LogEngine;

void ConfigFileTest::setUp()
{
}

void ConfigFileTest::tearDown()
{
    ShutdownLoggers();
}

// incorrect logger section name - error
void ConfigFileTest::testConfigFile01()
{
    CPPUNIT_ASSERT_THROW(InitFromFile(TEST_FILES_FOLDER "test01.lfg"), LogException);
}

// empty logger name - error
void ConfigFileTest::testConfigFile02()
{
    CPPUNIT_ASSERT_THROW(InitFromFile(TEST_FILES_FOLDER "test02.lfg"), LogException);
}

// section name that is neither logger nor sink - ok, no error
void ConfigFileTest::testConfigFile03()
{
    InitFromFile(TEST_FILES_FOLDER "test03.lfg");

    CPPUNIT_ASSERT_EQUAL(0u, LoggersCount());
}

void ConfigFileTest::testConfigFile1()
{
    InitFromFile(TEST_FILES_FOLDER "test1.lfg");

    CPPUNIT_ASSERT_EQUAL(2u, LoggersCount());

    Logger& logger1 = GetLogger("1");
    CPPUNIT_ASSERT_EQUAL(0u, logger1.SinkCount());
    CPPUNIT_ASSERT_EQUAL(Levels::llDebug, logger1.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger1.GetAsyncMode());

    Logger& logger2 = GetLogger("a");
    CPPUNIT_ASSERT_EQUAL(0u, logger2.SinkCount());
    CPPUNIT_ASSERT_EQUAL(LL_DEFAULT, logger2.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger2.GetAsyncMode());

}

void ConfigFileTest::testConfigFile2()
{
    InitFromFile(TEST_FILES_FOLDER "test2.lfg");
    
    CPPUNIT_ASSERT_EQUAL(2u, LoggersCount());
    
    Logger& logger1 = LogEngine::GetLogger("!");
    CPPUNIT_ASSERT_EQUAL(LL_DEFAULT, logger1.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger1.GetAsyncMode());
    CPPUNIT_ASSERT_EQUAL(0u, logger1.SinkCount());

    Logger& logger2 = LogEngine::GetLogger("!!");
    CPPUNIT_ASSERT_EQUAL(0u, logger2.SinkCount());
    CPPUNIT_ASSERT_EQUAL(LL_DEFAULT, logger2.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger2.GetAsyncMode());

}

void ConfigFileTest::testConfigFile3()
{
    InitFromFile(TEST_FILES_FOLDER "test3.lfg");

    CPPUNIT_ASSERT_EQUAL(1u, LoggersCount());

    Logger& logger = GetLogger("..");
    CPPUNIT_ASSERT_EQUAL(Levels::llTrace, logger.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(true, logger.GetAsyncMode());
    CPPUNIT_ASSERT_EQUAL(0u, logger.SinkCount());

}

void ConfigFileTest::testConfigFile4()
{
    InitFromFile(TEST_FILES_FOLDER "test4.lfg");

    CPPUNIT_ASSERT_EQUAL(2u, LoggersCount());

    Logger& logger1 = GetLogger("ABC");
    CPPUNIT_ASSERT_EQUAL(Levels::llDebug, logger1.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger1.GetAsyncMode());
    CPPUNIT_ASSERT_EQUAL(0u, logger1.SinkCount());

    Logger& logger2 = GetLogger("CdE");
    CPPUNIT_ASSERT_EQUAL(Levels::llCritical, logger2.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger2.GetAsyncMode());
    CPPUNIT_ASSERT_EQUAL(0u, logger2.SinkCount());

}

void ConfigFileTest::testConfigFile5()
{
    InitFromFile(TEST_FILES_FOLDER "test5.lfg");

    CPPUNIT_ASSERT_EQUAL(0u, LoggersCount());
}

void ConfigFileTest::testConfigFile6()
{
    InitFromFile(TEST_FILES_FOLDER "test6.lfg");

    CPPUNIT_ASSERT_EQUAL(1u, LoggersCount());

    Logger& logger1 = GetLogger("RotLog");
    CPPUNIT_ASSERT_EQUAL(LL_DEFAULT, logger1.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger1.GetAsyncMode());
    CPPUNIT_ASSERT_EQUAL(1u, logger1.SinkCount());
    auto sink = logger1.GetSink("Siii");
    
    RotatingFileSinkST* sinkPtr = dynamic_cast<RotatingFileSinkST*>(sink.get());
    CPPUNIT_ASSERT_EQUAL(Levels::llError, sink->GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(true, sink.get() == sinkPtr); // check sink type
}

void ConfigFileTest::testConfigFile7()
{
    InitFromFile(TEST_FILES_FOLDER "test7.lfg");

    CPPUNIT_ASSERT_EQUAL(1u, LoggersCount());

    Logger& logger1 = GetLogger("RLog");
    CPPUNIT_ASSERT_EQUAL(LL_DEFAULT, logger1.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger1.GetAsyncMode());
    CPPUNIT_ASSERT_EQUAL(1u, logger1.SinkCount());
    auto sink = logger1.GetSink("SuperSink");

    CPPUNIT_ASSERT_EQUAL(Levels::llError, sink->GetLogLevel());
    RotatingFileSinkMT* rsink = dynamic_cast<RotatingFileSinkMT*>(sink.get());
    CPPUNIT_ASSERT_EQUAL(true, sink.get() == rsink); // check sink type
    CPPUNIT_ASSERT_EQUAL(std::string("sink.super.log"), rsink->GetFileName());
    CPPUNIT_ASSERT_EQUAL(DefaultMaxBackupIndex, rsink->GetMaxBackupIndex()); 
    CPPUNIT_ASSERT_EQUAL(20*1024*1024ull, rsink->GetMaxLogSize()); 
    Layout* lay = /*dynamic_cast<PatternLayout*>*/(rsink->GetLayout());
    CPPUNIT_ASSERT_EQUAL(std::string(DefaultLinePattern), lay->GetPattern());
    CPPUNIT_ASSERT_EQUAL(std::string("! %TIME% #%THREAD% %OS% %OSVERSION% %APPNAME% %APPVERSION% : %MSG%"), lay->GetPattern(Levels::llError));
    CPPUNIT_ASSERT_EQUAL(std::string("# %TIME% #%THREAD% %OSVERSION% %OS% %APPVERSION% %APPNAME% : %MSG%"), lay->GetPattern(Levels::llWarning));
    CPPUNIT_ASSERT_EQUAL(std::string("*!* %TIME% #%THREAD% %APPNAME% %OS% %OSVERSION% %APPVERSION : %MSG%"), lay->GetPattern(Levels::llCritical));
    CPPUNIT_ASSERT_EQUAL(std::string("*!* %TIME% #%THREAD% %APPVERSION% %OS% %OSVERSION% %APPNAME% : %MSG%"), lay->GetPattern(Levels::llInfo));

}

void ConfigFileTest::testConfigFile8()
{
    InitFromFile(TEST_FILES_FOLDER "test8.lfg");

    CPPUNIT_ASSERT_EQUAL(2u, LoggersCount());

    // two loggers refer to the same Sink
    Logger& logger1 = GetLogger("FLog");
    CPPUNIT_ASSERT_EQUAL(LL_DEFAULT, logger1.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger1.GetAsyncMode());
    CPPUNIT_ASSERT_EQUAL(1u, logger1.SinkCount());
    auto sink1 = logger1.GetSink("SuperFSink");

    Logger& logger2 = GetLogger("FLog2");
    CPPUNIT_ASSERT_EQUAL(Levels::llTrace, logger2.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger2.GetAsyncMode());
    CPPUNIT_ASSERT_EQUAL(1u, logger2.SinkCount());
    auto sink2 = logger2.GetSink("SuperFSink");
    CPPUNIT_ASSERT_EQUAL(sink1, sink2);

    CPPUNIT_ASSERT_EQUAL(Levels::llError, sink1->GetLogLevel());
    FileSinkST* fsink = dynamic_cast<FileSinkST*>(sink1.get());
    CPPUNIT_ASSERT_EQUAL(true, sink1.get() == fsink); // check sink type
    CPPUNIT_ASSERT_EQUAL(std::string("sink super.log"), fsink->GetFileName());
    Layout* lay = fsink->GetLayout();
    CPPUNIT_ASSERT_EQUAL(std::string("ALL %TIME% #%THREAD% %OS% %OSVERSION% %APPNAME% %APPVERSION% : %MSG%"), lay->GetPattern());
    CPPUNIT_ASSERT_EQUAL(std::string("ALL %TIME% #%THREAD% %OS% %OSVERSION% %APPNAME% %APPVERSION% : %MSG%"), lay->GetPattern(Levels::llError));
    CPPUNIT_ASSERT_EQUAL(std::string("W %TIME% #%THREAD% %OSVERSION% %OS% %APPVERSION% %APPNAME% : %MSG%"), lay->GetPattern(Levels::llWarning));
    CPPUNIT_ASSERT_EQUAL(std::string("C %TIME% #%THREAD% %APPNAME% %OS% %OSVERSION% %APPVERSION : %MSG%"), lay->GetPattern(Levels::llCritical));
    CPPUNIT_ASSERT_EQUAL(std::string("I %TIME% #%THREAD% %APPVERSION% %OS% %OSVERSION% %APPNAME% : %MSG%"), lay->GetPattern(Levels::llInfo));

}

// bad LFG file with missing Sink section
// IniReader reads it successfully but InitFromFile throws an exception. 
// That is why LogException is thrown.
void ConfigFileTest::testConfigFile9()
{
    CPPUNIT_ASSERT_THROW(InitFromFile(TEST_FILES_FOLDER "test9.lfg"), LogException);
}

// Mistake in sink type name in ini file.
// test10.lfg file contains STDoutt instead of STDout
// This is critical mistake that is why exception is thrown
void ConfigFileTest::testConfigFile10()
{
    CPPUNIT_ASSERT_THROW(InitFromFile(TEST_FILES_FOLDER "test10.lfg"), LogException);
}

// noncritical mistakes in logleves, AsyncModes, and ThreadSafety parameter values
// defaul values of these parameters are used instead of throwing exceptions.
void ConfigFileTest::testConfigFile11()
{
    InitFromFile(TEST_FILES_FOLDER "test11.lfg");

    CPPUNIT_ASSERT_EQUAL(1u, LoggersCount());

    Logger& logger1 = GetLogger("mftLog");
    CPPUNIT_ASSERT_EQUAL(2u, logger1.SinkCount()); // make sure this is existing logger
    CPPUNIT_ASSERT_EQUAL(LL_DEFAULT, logger1.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger1.GetAsyncMode());
    
    auto sinknull = logger1.GetSink("nullsink");
    CPPUNIT_ASSERT(sinknull == nullptr);

    auto sink = logger1.GetSink("sinkfile");
    CPPUNIT_ASSERT(sink != nullptr);
    CPPUNIT_ASSERT_EQUAL(LL_DEFAULT, sink->GetLogLevel());
    
    FileSinkMT* fsink = dynamic_cast<FileSinkMT*>(sink.get()); 
    CPPUNIT_ASSERT_EQUAL(true, sink.get() == fsink); // check sink type
    CPPUNIT_ASSERT_EQUAL(std::string("Log.log"), fsink->GetFileName());
    sink = logger1.GetSink("sinkconsole");
    CPPUNIT_ASSERT_EQUAL(LL_DEFAULT, sink->GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(true, sink.get() == dynamic_cast<StdoutSinkST*>(sink.get())); // StdoutSinkST means that ThreadSafety=default thread safety=single 
}

void ConfigFileTest::testConfigFile12()
{
    InitFromFile(TEST_FILES_FOLDER "test12.lfg");

    CPPUNIT_ASSERT_EQUAL(1u, LoggersCount());

    Logger& logger = GetLogger("P");
    CPPUNIT_ASSERT_EQUAL(Levels::llTrace, logger.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger.GetAsyncMode());
    CPPUNIT_ASSERT_EQUAL(1u, logger.SinkCount());
 
    // set up properties in any place.
    logger.SetProperty(APPNAME_PROPERTY, "myappname");
    logger.SetProperty(APPVERSION_PROPERTY, "myappversion");

    auto sink = logger.GetSink("P");

    CPPUNIT_ASSERT_EQUAL(Levels::llTrace, sink->GetLogLevel());
    FileSinkST* fsink = dynamic_cast<FileSinkST*>(sink.get());
    CPPUNIT_ASSERT(fsink == nullptr);
    StringSinkST* ssink = dynamic_cast<StringSinkST*>(sink.get());
    CPPUNIT_ASSERT(ssink != nullptr);
    CPPUNIT_ASSERT_EQUAL(true, sink.get() == ssink); // check sink type

    //CPPUNIT_ASSERT_EQUAL(std::string("pattern.log"), ssink->GetFileName());
    Layout* lay = sink->GetLayout();
    CPPUNIT_ASSERT_EQUAL(std::string("ALL %TIME% #%THREAD% %OS% %OSVERSION% %APPNAME% %APPVERSION% : %MSG%"), lay->GetPattern());
    CPPUNIT_ASSERT_EQUAL(std::string("ALL %TIME% #%THREAD% %OS% %OSVERSION% %APPNAME% %APPVERSION% : %MSG%"), lay->GetPattern(Levels::llError));
    CPPUNIT_ASSERT_EQUAL(std::string("[WARN] Mess:%Msg%%% Empty:%% Unknown:%%"), lay->GetPattern(Levels::llWarning));
    CPPUNIT_ASSERT_EQUAL(std::string("%%%APPNAME% %%%APPVERSION% : %MSG%"), lay->GetPattern(Levels::llCritical));
    CPPUNIT_ASSERT_EQUAL(std::string("%APPVERSION% %APPNAME% : %MSG%."), lay->GetPattern(Levels::llInfo));
    CPPUNIT_ASSERT_EQUAL(std::string("[DEBUG] MesS:%MSg% Empty:%% unknown:% [/DEBUG]"), lay->GetPattern(Levels::llDebug));
    CPPUNIT_ASSERT_EQUAL(std::string("[TRACE] Mess:%Msg% Empty:%% Unknown:%"), lay->GetPattern(Levels::llTrace));

    logger.Debug("debug_message");
    std::string s = ssink->GetOutput();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Log lines are not equal.", std::string("[DEBUG] MesS:debug_message Empty:% unknown:% [/DEBUG]\n"), s);

    ssink->Clear();
    logger.Trace("trace_message");
    s = ssink->GetOutput();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Log lines are not equal.", std::string("[TRACE] Mess:trace_message Empty:% Unknown:%\n"), s);

    ssink->Clear();
    logger.Info("info_message");
    s = ssink->GetOutput();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Log lines are not equal.", std::string("myappversion myappname : info_message.\n"), s);

    ssink->Clear();
    logger.Crit("crit_message");
    s = ssink->GetOutput();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Log lines are not equal.", std::string("%myappname %myappversion : crit_message\n"), s);

    ssink->Clear();
    logger.Warn("warn_message");
    s = ssink->GetOutput();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Log lines are not equal.", std::string("[WARN] Mess:warn_message% Empty:% Unknown:%\n"), s);
}

// .lfg file, section [logger.xxxx] contains two sinks with equal names.
// in this case only one sink is added to the logger. logger checks if the sink has already added into logger and does not add second one
void ConfigFileTest::testConfigFile13()
{
    InitFromFile(TEST_FILES_FOLDER "test13.lfg");

    CPPUNIT_ASSERT_EQUAL(1u, LoggersCount());

    Logger& logger1 = GetLogger("Loooger");
    CPPUNIT_ASSERT_EQUAL(LL_DEFAULT, logger1.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger1.GetAsyncMode());
    CPPUNIT_ASSERT_EQUAL(1u, logger1.SinkCount());

    auto sink = logger1.GetSink("logsink");
    CPPUNIT_ASSERT_EQUAL(Levels::llError, sink->GetLogLevel());
    auto rsink = dynamic_cast<RotatingFileSinkST*>(sink.get());
    CPPUNIT_ASSERT(rsink != nullptr);
    CPPUNIT_ASSERT_EQUAL(true, sink.get() == rsink); // check sink type
    CPPUNIT_ASSERT_EQUAL(std::string("sink .log"), rsink->GetFileName());
    
}

// .lfg file contains two sink sections the same sink names.
// if .lfg file contains two sections with equal names then parameters of these two section 
// are "merged" like it was single section with all parameters
// parameters with equal names get double values
// for example merged section[sink.logSINK] in test14.lfg will have one parameter FileName
// that contains two values (refered by index): sink1log.log and sink2log.log
void ConfigFileTest::testConfigFile14()
{
    InitFromFile(TEST_FILES_FOLDER "test14.lfg");

    CPPUNIT_ASSERT_EQUAL(1u, LoggersCount());

    Logger& logger1 = GetLogger("Loooger");
    CPPUNIT_ASSERT_EQUAL(LL_DEFAULT, logger1.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger1.GetAsyncMode());
    CPPUNIT_ASSERT_EQUAL(1u, logger1.SinkCount());

    auto sink = logger1.GetSink("logsink");
    CPPUNIT_ASSERT_EQUAL(Levels::llError, sink->GetLogLevel());
    auto rsink = dynamic_cast<RotatingFileSinkST*>(sink.get());
    CPPUNIT_ASSERT(rsink != nullptr);
    CPPUNIT_ASSERT_EQUAL(true, sink.get() == rsink); // check sink type
    CPPUNIT_ASSERT_EQUAL(std::string("sink1log.log"), rsink->GetFileName());

}

void ConfigFileTest::testConfigFile20()
{
    InitFromFile(TEST_FILES_FOLDER "example2.lfg");

    CPPUNIT_ASSERT_EQUAL(2u, LoggersCount());

    Logger& logger1 = GetLogger("MainLogger");
    CPPUNIT_ASSERT_EQUAL(Levels::llDebug, logger1.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger1.GetAsyncMode());
    CPPUNIT_ASSERT_EQUAL(2u, logger1.SinkCount());
    auto sink = logger1.GetSink("s1");
    CPPUNIT_ASSERT_EQUAL(Levels::llTrace, sink->GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(true, sink.get() == dynamic_cast<FileSinkST*>(sink.get())); // check sink type
    sink = logger1.GetSink("s2");
    CPPUNIT_ASSERT_EQUAL(Levels::llError, sink->GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(true, sink.get() == dynamic_cast<StdoutSinkMT*>(sink.get())); // check sink type

    Logger& logger2 = GetLogger("Second");
    CPPUNIT_ASSERT_EQUAL(Levels::llWarning, logger2.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(true, logger2.GetAsyncMode());
    CPPUNIT_ASSERT_EQUAL(3u, logger2.SinkCount());

    sink = logger2.GetSink("s3");
    CPPUNIT_ASSERT_EQUAL(Levels::llTrace, sink->GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(true, sink.get() == dynamic_cast<FileSinkST*>(sink.get())); // check sink type

    sink = logger2.GetSink("s2");
    CPPUNIT_ASSERT_EQUAL(Levels::llError, sink->GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(true, sink.get() == dynamic_cast<StdoutSinkMT*>(sink.get())); // check sink type
    
    sink = logger2.GetSink("rotating");
    RotatingFileSinkST* rsink = dynamic_cast<RotatingFileSinkST*>(sink.get());
    CPPUNIT_ASSERT_EQUAL(LL_DEFAULT, sink->GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(true, sink.get() == rsink); // check sink type
    CPPUNIT_ASSERT_EQUAL(true, sink.get() == dynamic_cast<FileSinkST*>(sink.get())); // check sink type
    CPPUNIT_ASSERT_EQUAL(6*1024ull, rsink->GetMaxLogSize());
    CPPUNIT_ASSERT_EQUAL(5u, rsink->GetMaxBackupIndex());
    CPPUNIT_ASSERT_EQUAL(rsNumbers, rsink->GetStrategy());

}

// missing .LFG file
void ConfigFileTest::testConfigFile21()
{
    CPPUNIT_ASSERT_THROW(InitFromFile(TEST_FILES_FOLDER "testXXX.lfg"), FileException);
}

