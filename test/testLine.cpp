
#include "Shared.h"
#include "testLine.h"
#include "Pattern.h"
#include "LogEvent.h"
#include "LogEngine.h"

CPPUNIT_TEST_SUITE_REGISTRATION(LineTest);
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(LineTest, "LineTest");

using namespace LogEngine;

struct tm getFixedTime()
{
	timeb tm;
	tm.millitm = 111;
	tm.time = 444;
#if defined WIN32 && !defined __BORLANDC__
	struct tm tmt;
	localtime_s(&tmt, &tm.time);
	return tmt;
#else
	return *localtime((time_t*)(&tm.time));
#endif	
}


void LineTest::setUp ()
{

}

void LineTest::tearDown ()
{
    // free memory allocated in setUp, do other things
}

void LineTest::testLine1()
{
	Pattern line("");
	struct tm tp = getFixedTime();
	LogEvent event(nullptr, "MSG", Levels::llError, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT(res.empty());
}

void LineTest::testLine2()
{
	Pattern line("%DATE%Line%TIME%");
	struct tm tp = getFixedTime();
	LogEvent event(nullptr, "MSG", Levels::llError, 1, tp);
	
	std::string res = line.Format(event);
    CPPUNIT_ASSERT_EQUAL(std::string("01-Jan-1970Line03:07:24"), res);
}

void LineTest::testLine3()
{
	Pattern line("Line");
	struct tm tp = getFixedTime();
	LogEvent event(nullptr, "MSG", Levels::llDebug, 1, tp);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("Line"), res);
}

void LineTest::testLine4()
{
	Pattern line("%DATE%");
	struct tm tp = getFixedTime();
	LogEvent event(nullptr, "MSG", Levels::llTrace, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("01-Jan-1970"), res);
}

void LineTest::testLine5()
{
	Pattern line("%DATE%%DATE%%THREAD%");
	struct tm tp = getFixedTime();
	LogEvent event(nullptr, "MSG", Levels::llError, 555, tp);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("01-Jan-197001-Jan-1970555"), res);
}

void LineTest::testLine6()
{
	Pattern line("%DsssATE%%DATE%");
	struct tm tp = getFixedTime();
	LogEvent event(nullptr, "MSG", Levels::llError, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("%DsssATE%01-Jan-1970"), res);
}
void LineTest::testLine6_1()
{
	Pattern line("%DATE %DsssATE%");
	struct tm tp = getFixedTime();
	LogEvent event(nullptr, "MSG", Levels::llError, 1, tp);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("%DATE %DsssATE%"), res);
}

void LineTest::testLine7()
{
	Pattern line("%DsssATE%");
	struct tm tp = getFixedTime();
	LogEvent event(nullptr, "MSG", Levels::llCritical, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("%DsssATE%"), res);
}

void LineTest::testLine8()
{
	Pattern line("DsssATE%");
	struct tm tp = getFixedTime();
	LogEvent event(nullptr, "MSG", Levels::llError, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("DsssATE%"), res);
}

void LineTest::testLine9()
{
	Pattern line("dff%%dsf"); // %% automatically replaced by %
	struct tm tp = getFixedTime();
	LogEvent event(nullptr, "MSG", Levels::llDebug, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("dff%dsf"), res);
}

void LineTest::testLine10()
{
	Pattern line("d%%d%d"); // %% automatically replaced by %
	struct tm tp = getFixedTime();
	LogEvent event(nullptr, "MSG", Levels::llError, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("d%d%d"), res);
}

void LineTest::testLine11()
{
	Pattern line("Line%DATE%mama%MSG%%TIME%");
	struct tm tp = getFixedTime();
	LogEvent event(nullptr, "MSG", Levels::llInfo, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("Line01-Jan-1970mamaMSG03:07:24"), res);
	LogEvent event2(nullptr, "789", Levels::llError, 1, tp);
	
	res = line.Format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("Line01-Jan-1970mama78903:07:24"), res);
}

void LineTest::testLine12()
{
	Pattern line("testLine12 %DAte% %MSG% %time% #%THREAD");
	struct tm tp = getFixedTime();
	LogEvent event(nullptr, "message", Levels::llCritical, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine12 01-Jan-1970 message 03:07:24 #%THREAD"), res);
	
	LogEvent event2(nullptr, "789", Levels::llError, 2, tp);
	res = line.Format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine12 01-Jan-1970 789 03:07:24 #%THREAD"), res);
}

void LineTest::testLine13()
{
	Pattern line("testLine13 %dATE% %Msg% %tIME% %THRD% #%THREAD%");
	struct tm tp = getFixedTime();
	LogEvent event(nullptr, "message", Levels::llError, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine13 01-Jan-1970 message 03:07:24 %THRD% #1"), res);
	
	LogEvent event2(nullptr, "789", Levels::llError, 2, tp);
	res = line.Format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine13 01-Jan-1970 789 03:07:24 %THRD% #2"), res);
}

void LineTest::testLine14()
{
	Pattern line("testLine14 %DATETIME%..%DATE%..%TIME%");
	struct tm tp = getFixedTime();
	LogEvent event(nullptr, "message", Levels::llInfo, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine14 01-Jan-1970 03:07:24..01-Jan-1970..03:07:24"), res);
}

void LineTest::testLine15()
{
	Pattern line("testLine15 %OSVERSION%");
	struct tm tp = getFixedTime();
	LogEvent event(nullptr, "message", Levels::llError, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine15 10.0.26100.3775"), res);
}

void LineTest::testLine16()
{
	// logger must have requred property in his collection
	Logger& logger = GetLogger("loggerforAPPNAME");
	logger.SetProperty(APPNAME_PROPERTY, DefaultAppName);

	Pattern line("testLine16 %APPNAME%");
	struct tm tp = getFixedTime();
	LogEvent event(&logger, "message", Levels::llError, 1, tp);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine16 " DefaultAppName), res);
}

void LineTest::testLine17()
{
	// logger must have requred property in his collection
	Logger& logger = GetLogger("loggerforAPPVER");
	logger.SetProperty(APPVERSION_PROPERTY, DefaultAppVersion);

	Pattern line("testLine17 %APPVERSION%");
	struct tm tp = getFixedTime();
	LogEvent event(&logger, "message", Levels::llInfo, 1, tp);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine17 " DefaultAppVersion), res);
}

void LineTest::testLine18()
{
	Pattern line("testLine18 %OS% %Msg%");
	struct tm tp = getFixedTime();
	LogEvent event(nullptr, "msgggg", Levels::llError, 1, tp);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine18 6.3 SP 0.0 4 msgggg"), res);
}

void LineTest::testLine19()
{
	Pattern line("testLine19 %DATE% %MSG% ! %TIME% ! %THRD% ! #%THrEAD% ! %DateTime%" );
	struct tm tp = getFixedTime();
	LogEvent event(nullptr, "message", Levels::llError, 1234, tp);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine19 01-Jan-1970 message ! 03:07:24 ! %THRD% ! #1234 ! 01-Jan-1970 03:07:24"), res);

	LogEvent event2(nullptr, "", Levels::llError, 98765, tp);
	res = line.Format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine19 01-Jan-1970  ! 03:07:24 ! %THRD% ! #98765 ! 01-Jan-1970 03:07:24"), res);
}

void LineTest::testLine20()
{
	// logger should have requred property in his collection
	Logger& logger = GetLogger("loggerforAPPVER");
	logger.SetProperty("APPVERSION", "DefaultAppName");
	logger.SetProperty("APPNAME_PROPERTY", "DefaultAppName");
	logger.SetProperty(APPVERSION_PROPERTY, DefaultAppVersion);
	logger.SetProperty(APPVERSION_PROPERTY, DefaultAppVersion);
	logger.SetProperty(APPNAME_PROPERTY, DefaultAppName);
	logger.SetProperty(APPNAME_PROPERTY, DefaultAppName);

	Pattern line("testLine20 %DATE% %TIME% [#%thread%] [%appName%] [%loglevel%] %MSG%");
	struct tm tp = getFixedTime();
	LogEvent event(&logger, "message", Levels::llError, 1234, tp);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine20 01-Jan-1970 03:07:24 [#1234] [" DefaultAppName "] [ERROR] message"), res);

	LogEvent event2(&logger, "", Levels::llInfo, 98765, tp);
	res = line.Format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine20 01-Jan-1970 03:07:24 [#98765] [" DefaultAppName "] [INFO] "), res);
}

void LineTest::testLine21()
{
	Logger& logger = GetLogger("testloggername");
	Pattern line("testLine21 %LOGGERNAME% %loggername% [%appName%] [%loglevel%] %msg%");
	struct tm tp = getFixedTime();
	LogEvent event(&logger, "message", Levels::llError, 1234, tp);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine21 testloggername testloggername [" DefaultAppName "] [ERROR] message"), res);

	LogEvent event2(&logger, "", Levels::llInfo, 98765, tp);
	res = line.Format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine21 testloggername testloggername [" DefaultAppName "] [INFO] "), res);
}

void LineTest::testLine22()
{
	// logger should have requred property in his collection
    // in this test there is no required property in the logger
	Logger& logger = GetLogger("loggerforAPPVER");

	Pattern line("testLine22 %APPNAME%");
	LogEvent event(&logger, "message", Levels::llInfo, 1, getFixedTime());

	// if logger does not have required property then default value is returned
    // no exception is thrown in that case
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine22 " DefaultAppName), res);
}

