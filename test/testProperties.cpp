
#include "Shared.h"
#include "testProperties.h"
#include "Properties.h"
//#include "LogEngine.h"

CPPUNIT_TEST_SUITE_REGISTRATION( LogEnginePropertiesTest );

using namespace LogEngine;

void LogEnginePropertiesTest::setUp ()
{

}

void LogEnginePropertiesTest::tearDown ()
{
    // free memory allocated in setUp, do other things
}

void LogEnginePropertiesTest::testProperties1()
{
	Properties props;
	
	props.SetValue("DetailLevel", "4");
	props.SetValue("MaxLogSize", "1");
	props.SetValue("BackupType", "None");

	CPPUNIT_ASSERT_EQUAL(3u, props.Count());
	CPPUNIT_ASSERT_EQUAL(4, props.GetInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(1, props.GetInt("MaxLogSize"));
	CPPUNIT_ASSERT(props.GetString("BackupType") == "None");
}

void LogEnginePropertiesTest::testProperties2()
{
	Properties props;

	std::string s = "  123 ";
	std::string ss = s;
	std::string sss = Trim(s);

	CPPUNIT_ASSERT_EQUAL(s, ss); // check that trim does not change string s
	CPPUNIT_ASSERT_EQUAL(sss, std::string("123"));

	CPPUNIT_ASSERT(Trim("").empty());
	CPPUNIT_ASSERT(Trim(" ") == "");
	CPPUNIT_ASSERT(Trim("  ") == "");
	CPPUNIT_ASSERT(Trim("   \n") == "\n");
	CPPUNIT_ASSERT(Trim("\n") == "\n");
    CPPUNIT_ASSERT_EQUAL(std::string("a"), Trim("a"));
	CPPUNIT_ASSERT_EQUAL(std::string("aa"), Trim("aa"));
	CPPUNIT_ASSERT_EQUAL(std::string("aaa"), Trim("aaa"));
	CPPUNIT_ASSERT_EQUAL(std::string("a"), Trim(" a"));
	CPPUNIT_ASSERT_EQUAL(std::string("a"), Trim("  a"));
	CPPUNIT_ASSERT(Trim("\n  a") == "\n  a");
	CPPUNIT_ASSERT_EQUAL(std::string("a"), Trim("a "));
	CPPUNIT_ASSERT_EQUAL(std::string("a"), Trim("a  "));
	CPPUNIT_ASSERT(Trim("a  \n") == "a  \n");
	CPPUNIT_ASSERT_EQUAL(std::string("a"), Trim(" a "));
	CPPUNIT_ASSERT_EQUAL(std::string("aaa"), Trim("  aaa   "));
	CPPUNIT_ASSERT_EQUAL(std::string("a a a"), Trim(" a a a  "));
}

void LogEnginePropertiesTest::testProperties3()
{
	Properties props;
	props.SetValue("DetailLevel", "");
	props.SetValue("MaxLogSize", "");
	props.SetValue("BackupType", "");
	
	CPPUNIT_ASSERT_EQUAL(3u, props.Count());
	CPPUNIT_ASSERT_EQUAL(0, props.GetInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(0, props.GetInt("MaxLogSize"));
	CPPUNIT_ASSERT(props.GetString("BackupType").empty());
	
}

void LogEnginePropertiesTest::testProperties4()
{
	Properties props;
	props.SetValue("DetailLevel", "1");
	props.SetValue("DetailLevel", "0");

	CPPUNIT_ASSERT_EQUAL(1u, props.Count());
	CPPUNIT_ASSERT_EQUAL(0, props.GetInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(0ul, props.GetUInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(0, props.GetInt("MaxLogSize"));
	CPPUNIT_ASSERT(props.GetString("BackupType").empty());
}

void LogEnginePropertiesTest::testProperties5()
{
	Properties props;
	props.SetValue("DetailLevel", "4");
	props.SetValue("MaxLogSize", "1");
	props.SetValue("BackupType", "Single");

	CPPUNIT_ASSERT_EQUAL(3u, props.Count());
	CPPUNIT_ASSERT_EQUAL(4, props.GetInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(1, props.GetInt("MaxLogSize"));
	CPPUNIT_ASSERT_EQUAL(4ul, props.GetUInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(1ul, props.GetUInt("MaxLogSize"));
	CPPUNIT_ASSERT_EQUAL(std::string("Single"), props.GetString("BackupType"));
}

void LogEnginePropertiesTest::testProperties6()
{
	Properties props;
	props.SetValue("DetailLevel", "4");
	props.SetValue("MaxLogSize", "1");
	props.SetValue("BackupType", "Timestamp");

	CPPUNIT_ASSERT_EQUAL(3u, props.Count());
	CPPUNIT_ASSERT_EQUAL(4, props.GetInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(1, props.GetInt("MaxLogSize"));
	CPPUNIT_ASSERT_EQUAL(std::string("Timestamp"), props.GetString("BackupType"));
}

void LogEnginePropertiesTest::testProperties7()
{
	//std::ifstream fin(TEST_FILES_FOLDER "test6.lfg"/*, std::ios::in*/);
	//CPPUNIT_ASSERT_MESSAGE("Cannot open file" TEST_FILES_FOLDER "test6.lfg", !fin.fail());

	Properties props;
	props.SetValue("DetailLevel", "a"); // incorrect value
	props.SetValue("MaxLogSize", "1");
	props.SetValue("BackupType", "None");

	
	CPPUNIT_ASSERT_EQUAL(3u, props.Count());
	CPPUNIT_ASSERT_EQUAL(0, props.GetInt("DetailLevel")); //default value for int is used
	CPPUNIT_ASSERT_EQUAL(0ul, props.GetUInt("DetailLevel")); //default value for int is used
	CPPUNIT_ASSERT_EQUAL(1, props.GetInt("MaxLogSize"));
	CPPUNIT_ASSERT_EQUAL(std::string("None"), props.GetString("BackupType"));
}

void LogEnginePropertiesTest::testProperties8()
{
	Properties props;
	props.SetValue("DetailLevel", "12345678987654321"); // too large value
	props.SetValue("MaxLogSize", "one"); // incorrect value for int param
	props.SetValue("BackupType", "");

	CPPUNIT_ASSERT_EQUAL(3u, props.Count());
	CPPUNIT_ASSERT_EQUAL(0ul, props.GetUInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(0, props.GetInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(0, props.GetInt("MaxLogSize"));
	CPPUNIT_ASSERT(props.GetString("BackupType").empty());
}

void LogEnginePropertiesTest::testProperties9()
{
	Properties props;
	props.SetValue("DetailLevel", "00000");
	props.SetValue("MaxLogSize", "100");
	props.SetValue("BackupType", "None");
	props.SetValue("ApplicationName", "ExampleApp");
	props.SetValue("Version", "E.E.E.E");
	props.SetValue("LogFileName", "logs/ExampleApp.log");
	props.SetValue("StartAppLine", "\n%APPNAME% %APPVERSION% startup.\nLog is started at %DATETIME%.");
	props.SetValue("StopAppLine", "%APPNAME% %APPVERSION% normal shutdown.\nLog is stopped at %DATETIME%.");
	props.SetValue("SeparatorLine", "---------------------------------------------------------------------");
	props.SetValue("ErrorLine", "%TIME% #%THREAD% : %MSG%");
	props.SetValue("Warningline", "%TIME% #%THREAD% : %MSG%");
	props.SetValue("InfoLine", "%TIME% #%THREAD% : %MSG%");

	CPPUNIT_ASSERT_EQUAL(12u, props.Count());
	CPPUNIT_ASSERT_EQUAL(0, props.GetInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(100, props.GetInt("MaxLogSize"));
	CPPUNIT_ASSERT_EQUAL(std::string("None"), props.GetString("BackupType"));
	CPPUNIT_ASSERT_EQUAL(std::string("ExampleApp"), props.GetString("ApplicationName"));
	CPPUNIT_ASSERT_EQUAL(std::string("E.E.E.E"), props.GetString("Version"));
	CPPUNIT_ASSERT_EQUAL(std::string("logs/ExampleApp.log"), props.GetString("LogFileName"));
	std::string s = props.GetString("StartAppLine");
	CPPUNIT_ASSERT_EQUAL(std::string("\n%APPNAME% %APPVERSION% startup.\nLog is started at %DATETIME%."), s);
	CPPUNIT_ASSERT_EQUAL(std::string("%APPNAME% %APPVERSION% normal shutdown.\nLog is stopped at %DATETIME%."), props.GetString("StopAppLine"));
	CPPUNIT_ASSERT_EQUAL(std::string("---------------------------------------------------------------------"), props.GetString("SeparatorLine"));
	s = props.GetString("ErrorLine");
	CPPUNIT_ASSERT_EQUAL(std::string("%TIME% #%THREAD% : %MSG%"), s);
	CPPUNIT_ASSERT_EQUAL(std::string("%TIME% #%THREAD% : %MSG%"), props.GetString("WarningLine"));
	CPPUNIT_ASSERT_EQUAL(std::string("%TIME% #%THREAD% : %MSG%"), props.GetString("InfoLine"));
}

void LogEnginePropertiesTest::testProperties10()
{
	Properties props;
	props.SetValue("DetailLevel", "0000011");
	props.SetValue("MaxLogSize", "101");
	props.SetValue("BackupType", "TimeStamp");
	props.SetValue("ApplicationName", "ExampleApp11");
	props.SetValue("Version", "1.1.1.1");
	props.SetValue("LogFileName", "c:\\temp\\ExampleApp11.log");

	CPPUNIT_ASSERT_EQUAL(6u, props.Count());
	CPPUNIT_ASSERT_EQUAL(11, props.GetInt("DetailLevel"));
	CPPUNIT_ASSERT_EQUAL(std::string("TimeStamp"), props.GetString("BackupType"));
	CPPUNIT_ASSERT_EQUAL(101, props.GetInt("MaxLogSize"));
	CPPUNIT_ASSERT_EQUAL(std::string("1.1.1.1"), props.GetString("Version"));
	CPPUNIT_ASSERT_EQUAL(std::string("ExampleApp11"), props.GetString("applicationname"));
	CPPUNIT_ASSERT_EQUAL(std::string("c:\\temp\\ExampleApp11.log"), props.GetString("LogFileName"));
}

void LogEnginePropertiesTest::testProperties11()
{
    Properties props;
    props.SetValue("LogFileName", "DreamChatLog.log");

    try
    {
		std::string s = props.GetValue("BackupType");
    }
    catch(THArrayException & ex)
    {
        const char* str = "THArrayException : THash<I,V>::GetValue(Key) : Key not found !";
        CPPUNIT_ASSERT_EQUAL(str, ex.what());       
    }
    catch(...)
    {
        CPPUNIT_ASSERT_MESSAGE("Incorrect exception is thrown", false);
    }
}

void LogEnginePropertiesTest::testProperties12()
{
	Properties props;

	props.SetValue("DetailLevel", "-1");
	props.SetValue("MaxLogSize", "MaxLogSize"); // incorrect value
	props.SetValue("BackupType", "Nones");
	props.SetValue("ApplicationName", "Exam\npleApp");
	props.SetValue("Version", "t.t.t.t");
	props.SetValue("LogFileName", "logs/Error\nLog.log");


	CPPUNIT_ASSERT_EQUAL(6u, props.Count());
	CPPUNIT_ASSERT_EQUAL(5ul, props.GetUInt("DetailLevel", 5ul)); // in file it is '-1'
	CPPUNIT_ASSERT_EQUAL(1000ul, props.GetUInt("MaxLogSize", 1000ul)); // should be default value 100 while in file it is non digit value
	CPPUNIT_ASSERT_EQUAL(std::string("Nones"), props.GetString("BackupType")); // in file it is errorenos value
	CPPUNIT_ASSERT_EQUAL(std::string("Exam\npleApp"), props.GetString("ApplicationName"));
	CPPUNIT_ASSERT_EQUAL(std::string("t.t.t.t"), props.GetString("Version"));
	CPPUNIT_ASSERT_EQUAL(std::string("logs/Error\nLog.log"), props.GetString("LogFileName"));

}
