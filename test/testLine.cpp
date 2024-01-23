
#include "Shared.h"
#include "testLine.h"
#include "Pattern.h"
#include "LogEvent.h"

CPPUNIT_TEST_SUITE_REGISTRATION( LogEngineLineTest );

using namespace LogEngine;

struct tm getFixedTime()
{
	timeb tm;
	tm.millitm = 111;
	tm.time = 444;
	return *std::localtime(&tm.time);
}


void LogEngineLineTest::setUp ()
{

}

void LogEngineLineTest::tearDown ()
{
    // free memory allocated in setUp, do other things
}

void LogEngineLineTest::testLine1()
{
	Pattern line("");
	struct tm tp = getFixedTime();
	LogEvent event("MSG", llError, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT(res.empty());
}

void LogEngineLineTest::testLine2()
{
	Pattern line("%DATE%Line%TIME%");
	struct tm tp = getFixedTime();
	LogEvent event("MSG", llError, 1, tp);
	
	std::string res = line.Format(event);
    CPPUNIT_ASSERT_EQUAL(std::string("01-Jan-1970Line03:07:24"), res);
}

void LogEngineLineTest::testLine3()
{
	Pattern line("Line");
	struct tm tp = getFixedTime();
	LogEvent event("MSG", llDebug, 1, tp);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("Line"), res);
}

void LogEngineLineTest::testLine4()
{
	Pattern line("%DATE%");
	struct tm tp = getFixedTime();
	LogEvent event("MSG", llTrace, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("01-Jan-1970"), res);
}

void LogEngineLineTest::testLine5()
{
	Pattern line("%DATE%%DATE%%THREAD%");
	struct tm tp = getFixedTime();
	LogEvent event("MSG", llError, 555, tp);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("01-Jan-197001-Jan-1970555"), res);
}

void LogEngineLineTest::testLine6()
{
	Pattern line("%DsssATE%%DATE%");
	struct tm tp = getFixedTime();
	LogEvent event("MSG", llError, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("%DsssATE%01-Jan-1970"), res);
}
void LogEngineLineTest::testLine6_1()
{
	Pattern line("%DATE %DsssATE%");
	struct tm tp = getFixedTime();
	LogEvent event("MSG", llError, 1, tp);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("%DATE %DsssATE%"), res);
}

void LogEngineLineTest::testLine7()
{
	Pattern line("%DsssATE%");
	struct tm tp = getFixedTime();
	LogEvent event("MSG", llCritical, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("%DsssATE%"), res);
}

void LogEngineLineTest::testLine8()
{
	Pattern line("DsssATE%");
	struct tm tp = getFixedTime();
	LogEvent event("MSG", llError, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("DsssATE%"), res);
}

void LogEngineLineTest::testLine9()
{
	Pattern line("dff%%dsf");
	struct tm tp = getFixedTime();
	LogEvent event("MSG", llDebug, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("dff%%dsf"), res);
}

void LogEngineLineTest::testLine10()
{
	Pattern line("d%%d%d");
	struct tm tp = getFixedTime();
	LogEvent event("MSG", llError, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("d%%d%d"), res);
}

void LogEngineLineTest::testLine11()
{
	Pattern line("Line%DATE%mama%MSG%%TIME%");
	struct tm tp = getFixedTime();
	LogEvent event("MSG", llInfo, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("Line01-Jan-1970mamaMSG03:07:24"), res);
	LogEvent event2("789", llError, 1, tp);
	
	res = line.Format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("Line01-Jan-1970mama78903:07:24"), res);
}

void LogEngineLineTest::testLine12()
{
	Pattern line("testLine12 %DATE% %MSG% %TIME% #%THREAD");
	struct tm tp = getFixedTime();
	LogEvent event("message", llCritical, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine12 01-Jan-1970 message 03:07:24 #%THREAD"), res);
	
	LogEvent event2("789", llError, 2, tp);
	res = line.Format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine12 01-Jan-1970 789 03:07:24 #%THREAD"), res);
}

void LogEngineLineTest::testLine13()
{
	Pattern line("testLine13 %DATE% %MSG% %TIME% %THRD% #%THREAD%");
	struct tm tp = getFixedTime();
	LogEvent event("message", llError, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine13 01-Jan-1970 message 03:07:24 %THRD% #1"), res);
	
	LogEvent event2("789", llError, 2, tp);
	res = line.Format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine13 01-Jan-1970 789 03:07:24 %THRD% #2"), res);
}

void LogEngineLineTest::testLine14()
{
	Pattern line("testLine14 %DATETIME%..%DATE%..%TIME%");
	struct tm tp = getFixedTime();
	LogEvent event("message", llInfo, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine14 01-Jan-1970 03:07:24..01-Jan-1970..03:07:24"), res);
}

void LogEngineLineTest::testLine15()
{
	Pattern line("testLine15 %OSVERSION%");
	struct tm tp = getFixedTime();
	LogEvent event("message", llError, 1, tp);
	
	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine15 10.0.22621.2506"), res);
}

void LogEngineLineTest::testLine16()
{
	Pattern line("testLine16 %APPNAME%");
	struct tm tp = getFixedTime();
	LogEvent event("message", llError, 1, tp);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine16 " DefaultAppName), res);
}

void LogEngineLineTest::testLine17()
{
	Pattern line("testLine17 %APPVERSION%");
	struct tm tp = getFixedTime();
	LogEvent event("message", llInfo, 1, tp);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine17 " DefaultAppVersion), res);
}

void LogEngineLineTest::testLine18()
{
	Pattern line("testLine18 %OS% %Msg%");
	struct tm tp = getFixedTime();
	LogEvent event("msgggg", llError, 1, tp);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine18 6.3 SP 0.0 4 msgggg"), res);
}

void LogEngineLineTest::testLine19()
{
	Pattern line("testLine19 %DATE% %MSG% ! %TIME% ! %THRD% ! #%THrEAD% ! %DateTime% ! %APPVERSIOn% ! %appName%" );
	struct tm tp = getFixedTime();
	LogEvent event("message", llError, 1234, tp);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine19 01-Jan-1970 message ! 03:07:24 ! %THRD% ! #1234 ! 01-Jan-1970 03:07:24 ! " DefaultAppVersion " ! " DefaultAppName), res);

	LogEvent event2("", llError, 98765, tp);
	res = line.Format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine19 01-Jan-1970  ! 03:07:24 ! %THRD% ! #98765 ! 01-Jan-1970 03:07:24 ! " DefaultAppVersion " ! " DefaultAppName), res);
}

void LogEngineLineTest::testLine20()
{
	Pattern line("testLine20 %DATE% %TIME% [#%thread%] [%appName%] [%loglevel%] %MSG%");
	struct tm tp = getFixedTime();
	LogEvent event("message", llError, 1234, tp);

	std::string res = line.Format(event);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine20 01-Jan-1970 03:07:24 [#1234] [" DefaultAppName "] [ERROR] message"), res);

	LogEvent event2("", llInfo, 98765, tp);
	res = line.Format(event2);
	CPPUNIT_ASSERT_EQUAL(std::string("testLine20 01-Jan-1970 03:07:24 [#98765] [" DefaultAppName "] [INFO] "), res);
}

