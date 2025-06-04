
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
	FixedTime = getFixedTime();
}

void LineTest::tearDown ()
{
	ShutdownLoggers();
    // free memory allocated in setUp, do other things
}

void LineTest::testLine1()
{
	Pattern line("");
	LogEvent event(nullptr, "MSG", Levels::llError, 1, FixedTime);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT(res.empty());
}

void LineTest::testLine2()
{
	Pattern line("%DATE%Line%TIME%");
	LogEvent event(nullptr, "MSG", Levels::llError, 1, FixedTime);
	
	std::string res = line.Format(event);
    CPPUNIT_ASSERT_EQUAL(std::string("01/01/70Line03:07:24"), res);
}

void LineTest::testLine3()
{
	Pattern line("Line");
	LogEvent event(nullptr, "MSG", Levels::llDebug, 1, FixedTime);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("Line"), res);
}

void LineTest::testLine4()
{
	Pattern line("%DATE%");
	LogEvent event(nullptr, "MSG", Levels::llTrace, 1, FixedTime);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("01/01/70"), res);

	Properties props;
	props.SetValue("%DATE%", "%d-%b-%Y"); // setup date format for log line
	res = line.Format(event, props);
	CPPUNIT_ASSERT_EQUAL(std::string("01-Jan-1970"), res);

	props.SetValue("%DATE%", "%d %b %Y"); // setup date format for log line
	res = line.Format(event, props);
	CPPUNIT_ASSERT_EQUAL(std::string("01 Jan 1970"), res);

}

void LineTest::testLine5()
{
	Pattern line("%DATE%%DATE%%THREAD%");
	LogEvent event(nullptr, "MSG", Levels::llError, 555, FixedTime);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("01/01/7001/01/70555"), res);
}

void LineTest::testLine6()
{
	Pattern line("%DsssATE%%DATE%");
	LogEvent event(nullptr, "MSG", Levels::llError, 1, FixedTime);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("%DsssATE%01/01/70"), res);
}
void LineTest::testLine6_1()
{
	Pattern line("%DATE %DsssATE%");
	LogEvent event(nullptr, "MSG", Levels::llError, 1, FixedTime);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("%DATE %DsssATE%"), res);
}

void LineTest::testLine7()
{
	Pattern line("%DsssATE%");
	LogEvent event(nullptr, "MSG", Levels::llCritical, 1, FixedTime);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("%DsssATE%"), res);
}

void LineTest::testLine8()
{
	Pattern line("DsssATE%");
	LogEvent event(nullptr, "MSG", Levels::llError, 1, FixedTime);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("DsssATE%"), res);
}

void LineTest::testLine9()
{
	Pattern line("dff%%dsf"); // %% automatically replaced by %

	LogEvent event(nullptr, "MSG", Levels::llDebug, 1, FixedTime);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("dff%dsf"), res);
}

void LineTest::testLine10()
{
	Pattern line("d%%d%d"); // %% automatically replaced by %

	LogEvent event(nullptr, "MSG", Levels::llError, 1, FixedTime);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("d%d%d"), res);
}

void LineTest::testLine11()
{
	Pattern line("Line%DATE%mama%MSG%%TIME%");
	LogEvent event(nullptr, "MSG", Levels::llInfo, 1, FixedTime);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("Line01/01/70mamaMSG03:07:24"), res);
	LogEvent event2(nullptr, "789", Levels::llError, 1, FixedTime);
	
	Properties props;
	props.SetValue(DateMacro, "%d-%b-%Y");
	props.SetValue(TimeMacro, "%H::%M::%S");
	res = line.Format(event2, props);
	CPPUNIT_ASSERT_EQUAL(std::string("Line01-Jan-1970mama78903::07::24"), res);
}

void LineTest::testLine12()
{
	Pattern line("testLine12 %DAte% %MSG% %time% #%THREAD");
	LogEvent event(nullptr, "message", Levels::llCritical, 1, FixedTime);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine12 01/01/70 message 03:07:24 #%THREAD"), res);
	
	Properties props;
	props.SetValue("%date%", "%d-%b-%y"); // param names here are in lower case to check that param names are case INsensitive
	props.SetValue("%time%", "%R");
	LogEvent event2(nullptr, "789", Levels::llError, 2, FixedTime);
	res = line.Format(event2, props);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine12 01-Jan-70 789 03:07 #%THREAD"), res);
}

void LineTest::testLine13()
{
	Pattern line("testLine13 %dATE% %Msg% %tIME% %THRD% #%THREAD%");
	LogEvent event(nullptr, "message", Levels::llError, 1, FixedTime);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine13 01/01/70 message 03:07:24 %THRD% #1"), res);
	
	LogEvent event2(nullptr, "789", Levels::llError, 2, FixedTime);
	res = line.Format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine13 01/01/70 789 03:07:24 %THRD% #2"), res);
}

void LineTest::testLine14()
{
	Pattern line("testLine14 %DATETIME%..%DATE%..%TIME%");
	LogEvent event(nullptr, "message", Levels::llInfo, 1, FixedTime);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine14 01/01/70 03:07:24..01/01/70..03:07:24"), res);
}

void LineTest::testLine15()
{
	Pattern line("testLine15 %OSVERSION%");
	LogEvent event(nullptr, "message", Levels::llError, 1, FixedTime);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine15 10.0.26100.3912"), res);
}

void LineTest::testLine16()
{
	// logger must have requred property in his collection
	Logger& logger = GetLogger("loggerforAPPNAME");
	logger.SetProperty(APPNAME_PROPERTY, DefaultAppName);

	Pattern line("testLine16 %APPNAME%");
	LogEvent event(&logger, "message", Levels::llError, 1, FixedTime);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine16 " DefaultAppName), res);
}

void LineTest::testLine17()
{
	// logger must have requred property in his collection
	Logger& logger = GetLogger("loggerforAPPVER");
	logger.SetProperty(APPVERSION_PROPERTY, DefaultAppVersion);

	Pattern line("testLine17 %APPVERSION%");
	LogEvent event(&logger, "message", Levels::llInfo, 1, FixedTime);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine17 " DefaultAppVersion), res);
}

void LineTest::testLine18()
{
	Pattern line("testLine18 %OS% %Msg%");
	LogEvent event(nullptr, "msgggg", Levels::llError, 1, FixedTime);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine18 6.3 SP 0.0 4 msgggg"), res);
}

void LineTest::testLine19()
{
	Pattern line("testLine19 %DATE% %MSG% ! %TIME% ! %THRD% ! #%THrEAD% ! %DateTime%" );
	LogEvent event(nullptr, "message", Levels::llError, 1234, FixedTime);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine19 01/01/70 message ! 03:07:24 ! %THRD% ! #1234 ! 01/01/70 03:07:24"), res);

	Properties props;
	props.SetValue(DateMacro, "%F");
	LogEvent event2(nullptr, "", Levels::llError, 98765, FixedTime);
	res = line.Format(event2, props);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine19 1970-01-01  ! 03:07:24 ! %THRD% ! #98765 ! 01/01/70 03:07:24"), res);
}

void LineTest::testLine20()
{
	// logger should have requred property in his collection
	Logger& logger = GetLogger("loggerforAPPVER");
	logger.SetProperty("APPVERSION", "DefaultAppName");
	logger.SetProperty("%APPNAME_PROPERTY%", "AnotherAppName");
	logger.SetProperty(APPVERSION_PROPERTY, "MyAppVersion1");
	logger.SetProperty(APPVERSION_PROPERTY, "MyAppVersion2");
	logger.SetProperty(APPNAME_PROPERTY, "FirstAppName");
	logger.SetProperty(APPNAME_PROPERTY, "FirstAppName2");

	Pattern line("testLine20 %DATE% %TIME% [#%thread%] [%appName%] [%loglevel%] %MSG%");
	
	LogEvent event1(&logger, "message", Levels::llError, 1234, FixedTime);

	std::string res = line.Format(event1);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine20 01/01/70 03:07:24 [#1234] [FirstAppName2] [ERROR] message"), res);

	LogEvent event2(&logger, "", Levels::llInfo, 98765, FixedTime);
	res = line.Format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine20 01/01/70 03:07:24 [#98765] [FirstAppName2] [INFO] "), res);

	// another line format with the same event
	logger.SetProperty(DateMacro, "%d-%m-%Y");
	Pattern line2("testLine20 %DATE% %TIME% [%appversion%] [%loglevel%] %MSG%");
	res = line2.Format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine20 01-01-1970 03:07:24 [MyAppVersion2] [INFO] "), res);
}

void LineTest::testLine21()
{
	Logger& logger = GetLogger("testloggername");
	
	Pattern line("testLine21 %LOGGERNAME% %loggername% [%appName%] [%loglevel%] %msg%");
	LogEvent event(&logger, "message", Levels::llError, 1234, FixedTime);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine21 testloggername testloggername [" DefaultAppName "] [ERROR] message"), res);

	LogEvent event2(&logger, "", Levels::llInfo, 98765, FixedTime);
	res = line.Format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine21 testloggername testloggername [" DefaultAppName "] [INFO] "), res);
}

void LineTest::testLine22()
{
	// logger should have requred property in his collection
    // in this test there is no required property in the logger
	Logger& logger = GetLogger("loggerforAPPVER");

	Pattern line("testLine22 %APPNAME%");
	LogEvent event(&logger, "message", Levels::llInfo, 1, FixedTime);

	// if logger does not have required property then default value is returned
    // no exception is thrown in that case
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine22 " DefaultAppName), res);
}

void LineTest::testLine23()
{
	// logger should have requred property in his collection
	Properties props;
	props.SetValue("APPVERSION", "DefaultAppName");
	props.SetValue("APPNAME_PROPERTY", "AnotherAppName");
	props.SetValue(APPVERSION_PROPERTY, "12.12.12.12");
	props.SetValue(APPNAME_PROPERTY, "Super application");

	Pattern line("testLine23 %DATETIME% (%APPNAME%) %APPVERSION% [%loglevel%] %MSG%");
	
	LogEvent event(nullptr, "logmessage", Levels::llError, 1234, FixedTime);
	std::string res = line.Format(event, props);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine23 01/01/70 03:07:24 (Super application) 12.12.12.12 [ERROR] logmessage"), res);

	LogEvent event2(nullptr, "", Levels::llInfo, 98765, FixedTime);
	res = line.Format(event2, props);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine23 01/01/70 03:07:24 (Super application) 12.12.12.12 [INFO] "), res);

	// another line format with the same event
	props.SetValue(APPNAME_PROPERTY, "another name");
	Pattern line2("testLine23 %DATETIME% (%appname%) %appversion% %MSG%");
	res = line2.Format(event2, props);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine23 01/01/70 03:07:24 (another name) 12.12.12.12 "), res);

}

