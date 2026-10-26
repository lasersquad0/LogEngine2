
#ifdef WIN32
#include <direct.h>
#else
//#include <unistd.h>
#endif 

#include "Shared.h"
#include "LogEngine.h"
#include "testPatternLayout.h"
#include "Properties.h"
#include "RotatingFileSink.h"

CPPUNIT_TEST_SUITE_REGISTRATION( PatternTest);
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PatternTest, "PatternTest");

using namespace LogEngine;

static struct tm getFixedTime()
{
	timeb tm;
	tm.millitm = 222;
	tm.time = 555;
#if defined WIN32
	struct tm tmt;
	localtime_s(&tmt, &tm.time);
	return tmt;
#else
	return *localtime((time_t*)(&tm.time));
#endif	
}

void PatternTest::setUp()
{
	FixedTime = getFixedTime();

#ifdef WIN32
	std::ignore = _mkdir(LOG_FILES_FOLDER);
#else
	mkdir(LOG_FILES_FOLDER, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
}

void PatternTest::tearDown()
{

}

void PatternTest::testLog0()
{
	PatternLayout layout;
	LogEvent event(nullptr, "message", llCritical, 1, FixedTime);
	Properties props;
	std::string res = layout.Format(event, props);
	CPPUNIT_ASSERT_EQUAL(std::string(" message"), CutLogLine(res));

}

// testing StringSink 
void PatternTest::testLog1()
{
	PatternLayout layout;
	layout.SetPattern(llWarning, "%%%MSG%XXX%CUSTOM%%APPNAME%");
	layout.SetPattern(llInfo, "%CUSTOM% %LOGGERNAME% %OS%");
	layout.SetPattern(llDebug, "XXX %APPVERSION% %OSVERSION% XXX");

	Properties props;
	LogEvent event(nullptr, "mess", llCritical, 1111, FixedTime);

	std::string str = layout.Format(event, props);
	CPPUNIT_ASSERT_EQUAL(std::string(" mess"), CutLogLine(str));

	event.msgLevel = llWarning;
	str = layout.Format(event, props);
	CPPUNIT_ASSERT_EQUAL(std::string("%messXXX%CUSTOM%nonameapp"), CutLogLine(str));

	event.msgLevel = llInfo;
	str = layout.Format(event, props);
#ifdef WIN32
	CPPUNIT_ASSERT_EQUAL(std::string("%CUSTOM%  Windows 10 Enterprise"), CutLogLine(str));
#else
	CPPUNIT_ASSERT_EQUAL(std::string("%CUSTOM%  Linux CMDB-182726 6.18.54.1-microsoft-standard-WSL2"), CutLogLine(str));
#endif
	
	event.msgLevel = llDebug;
	str = layout.Format(event, props);
	CPPUNIT_ASSERT_EQUAL(std::string("XXX 0.0.0.0 10.0.26100.9444 XXX"), CutLogLine(str));

	event.msgLevel = llTrace;
	str = layout.Format(event, props);
	CPPUNIT_ASSERT_EQUAL(std::string(" mess"), CutLogLine(str));
}

void PatternTest::testLog2()
{
	PatternLayout layout;
	layout.SetPattern(llWarning, "%%%MSG%XXX%CUSTOM%%APPNAME%");
	layout.SetPattern(llInfo, "%CUSTOM% %LOGGERNAME% %OS%");
	layout.SetPattern(llDebug, "XXX %APPVERSION% %OSVERSION% XXX");

	Properties props;
	props.SetValue("%APPVERSION%", "wrong app version");
	props.SetValue("%APPNAME%", "Wrong app name");
	props.SetValue("%CUSTOM%", "Wrong custom param value");
	props.SetValue("appname", "My app_name");
	props.SetValue("appversion", "My 4.3.2.1");
	props.SetValue("Custom", "My custom param");

	LogEvent event(nullptr, "mess", llCritical, 1111, FixedTime);

	std::string str = layout.Format(event, props);
	CPPUNIT_ASSERT_EQUAL(std::string(" mess"), CutLogLine(str));

	event.msgLevel = llWarning;
	str = layout.Format(event, props);
	CPPUNIT_ASSERT_EQUAL(std::string("%messXXXMy custom paramMy app_name"), CutLogLine(str));

	event.msgLevel = llInfo;
	str = layout.Format(event, props);
#ifdef WIN32
	CPPUNIT_ASSERT_EQUAL(std::string("My custom param  Windows 10 Enterprise"), CutLogLine(str));
#else
	std::cout << std::endl;
	std::cout << "len=" << str.size() <<std::endl;
	std::cout << std::hex << std::showbase << (int)str[64] << (int)str[65] << (int)str[66] << (int)str[67] << (int)str[68] << std::endl;
	std::string str2 = DelCRLF(str);
	std::cout << str2 << "Postfix" << std::endl;
	CPPUNIT_ASSERT_EQUAL(std::string("My custom param  Linux CMDB-182726 6.18.54.1-microsoft-standard-WSL2"), CutLogLine(str));
#endif

	event.msgLevel = llDebug;
	str = layout.Format(event, props);
	CPPUNIT_ASSERT_EQUAL(std::string("XXX My 4.3.2.1 10.0.26100.9444 XXX"), CutLogLine(str));

	event.msgLevel = llTrace;
	str = layout.Format(event, props);
	CPPUNIT_ASSERT_EQUAL(std::string(" mess"), CutLogLine(str));

	event.msgLevel = llWarning;
	props.SetValue("CUSTOM", "newCval");
	props.SetValue("AppName", "newAppName");
	str = layout.Format(event, props);
	CPPUNIT_ASSERT_EQUAL(std::string("%messXXXnewCvalnewAppName"), CutLogLine(str));
}

// set different layouts to StringSink 
void PatternTest::testLog3()
{
	std::shared_ptr<StringSinkST> sink(new StringSinkST("strsink"));

	CPPUNIT_ASSERT_EQUAL(llInfo, sink->GetLogLevel());
	CPPUNIT_ASSERT_EQUAL(std::string("strsink"), sink->GetName());
	CPPUNIT_ASSERT_EQUAL(std::string(""), sink->GetOutput());
	CPPUNIT_ASSERT_EQUAL(0ull, sink->GetBytesWritten());
	CPPUNIT_ASSERT_EQUAL(0ull, sink->GetMessageCounts()[llCritical]);
	CPPUNIT_ASSERT_EQUAL(0ull, sink->GetMessageCounts()[llError]);
	CPPUNIT_ASSERT_EQUAL(0ull, sink->GetMessageCounts()[llWarning]);
	CPPUNIT_ASSERT_EQUAL(0ull, sink->GetMessageCounts()[llInfo]);
	CPPUNIT_ASSERT_EQUAL(0ull, sink->GetMessageCounts()[llDebug]);
	CPPUNIT_ASSERT_EQUAL(0ull, sink->GetMessageCounts()[llTrace]);
	
	Layout* oldLayout = sink->GetLayout();
	CPPUNIT_ASSERT(oldLayout != nullptr);

	sink->SetLayout(nullptr); // nullptr does not change layout in sink
	CPPUNIT_ASSERT_EQUAL(sink->GetLayout(), oldLayout);

	Layout* newLayout = new PatternLayout; // sink will call delete on this layout in its destructor, that is why we cannot pass local variable here.
	sink->SetLayout(newLayout);
	CPPUNIT_ASSERT_EQUAL(sink->GetLayout(), newLayout);
}

