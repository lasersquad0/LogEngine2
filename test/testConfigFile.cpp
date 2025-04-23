
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
    CPPUNIT_ASSERT_EQUAL(std::string("sink.super.log"), rsink->getFileName());
    CPPUNIT_ASSERT_EQUAL(DefaultMaxBackupIndex, rsink->GetMaxBackupIndex()); 
    CPPUNIT_ASSERT_EQUAL(20*1024*1024ull, rsink->GetMaxLogSize()); 
    PatternLayout* lay = /*dynamic_cast<PatternLayout*>*/(rsink->GetLayout());
    CPPUNIT_ASSERT_EQUAL(std::string(DefaultLinePattern), lay->GetAllPatterns());
    CPPUNIT_ASSERT_EQUAL(std::string("! %TIME% #%THREAD% %OS% %OSVERSION% %APPNAME% %APPVERSION% : %MSG%"), lay->GetPattern(Levels::llError));
    CPPUNIT_ASSERT_EQUAL(std::string("# %TIME% #%THREAD% %OSVERSION% %OS% %APPVERSION% %APPNAME% : %MSG%"), lay->GetPattern(Levels::llWarning));
    CPPUNIT_ASSERT_EQUAL(std::string("*!* %TIME% #%THREAD% %APPNAME% %OS% %OSVERSION% %APPVERSION : %MSG%"), lay->GetPattern(Levels::llCritical));
    CPPUNIT_ASSERT_EQUAL(std::string("*!* %TIME% #%THREAD% %APPVERSION% %OS% %OSVERSION% %APPNAME% : %MSG%"), lay->GetPattern(Levels::llInfo));

}

void ConfigFileTest::testConfigFile8()
{
    InitFromFile(TEST_FILES_FOLDER "test8.lfg");

    CPPUNIT_ASSERT_EQUAL(1u, LoggersCount());

    Logger& logger1 = GetLogger("FLog");
    CPPUNIT_ASSERT_EQUAL(LL_DEFAULT, logger1.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger1.GetAsyncMode());
    CPPUNIT_ASSERT_EQUAL(1u, logger1.SinkCount());
    auto sink = logger1.GetSink("SuperFSink");

    CPPUNIT_ASSERT_EQUAL(Levels::llError, sink->GetLogLevel());
    FileSinkST* fsink = dynamic_cast<FileSinkST*>(sink.get());
    CPPUNIT_ASSERT_EQUAL(true, sink.get() == fsink); // check sink type
    CPPUNIT_ASSERT_EQUAL(std::string("sink super.log"), fsink->getFileName());
    PatternLayout* lay = /*dynamic_cast<PatternLayout*>*/(fsink->GetLayout());
    CPPUNIT_ASSERT_EQUAL(std::string("ALL %TIME% #%THREAD% %OS% %OSVERSION% %APPNAME% %APPVERSION% : %MSG%"), lay->GetAllPatterns());
    CPPUNIT_ASSERT_EQUAL(std::string("ALL %TIME% #%THREAD% %OS% %OSVERSION% %APPNAME% %APPVERSION% : %MSG%"), lay->GetPattern(Levels::llError));
    CPPUNIT_ASSERT_EQUAL(std::string("W %TIME% #%THREAD% %OSVERSION% %OS% %APPVERSION% %APPNAME% : %MSG%"), lay->GetPattern(Levels::llWarning));
    CPPUNIT_ASSERT_EQUAL(std::string("C %TIME% #%THREAD% %APPNAME% %OS% %OSVERSION% %APPVERSION : %MSG%"), lay->GetPattern(Levels::llCritical));
    CPPUNIT_ASSERT_EQUAL(std::string("I %TIME% #%THREAD% %APPVERSION% %OS% %OSVERSION% %APPNAME% : %MSG%"), lay->GetPattern(Levels::llInfo));

}

// bad LFG file with missing Sink section
void ConfigFileTest::testConfigFile9()
{
    CPPUNIT_ASSERT_THROW(InitFromFile(TEST_FILES_FOLDER "test9.lfg"), FileException);
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


void ConfigFileTest::testConfigFile21()
{
    std::string ver = DisplaySystemVersion();
    printf("\n");
    printf(ver.c_str());
}

