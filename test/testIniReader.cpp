#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif 

#include "Shared.h"
#include "testIniReader.h"
#include "IniReader.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TIniReaderTest);
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TIniReaderTest, "TIniReaderTest");


void TIniReaderTest::setUp()
{
#ifdef WIN32
	std::ignore = mkdir(LOG_FILES_FOLDER);
#else
	mkdir(LOG_FILES_FOLDER, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
}

void TIniReaderTest::tearDown()
{
	// free memory allocated in setUp, do other things
}

void TIniReaderTest::testIniReader1()
{
	IniReader rd;
	rd.LoadIniFile(TEST_FILES_FOLDER "testini1.ini");

	CPPUNIT_ASSERT_EQUAL(2u, rd.SectionsCount());
	CPPUNIT_ASSERT_EQUAL(2u, rd.ValuesCount(MISSING_SECTION));
	CPPUNIT_ASSERT_EQUAL(std::string{ "1" }, rd.GetValue(MISSING_SECTION, "maxLogSize"));
	CPPUNIT_ASSERT_EQUAL(std::string{ "" }, rd.GetValue(MISSING_SECTION, "BackupType"));
	CPPUNIT_ASSERT_EQUAL(1u, rd.ValuesCount("correct section name"));
	CPPUNIT_ASSERT_EQUAL(std::string{ "11" }, rd.GetValue("correct section name", "DEBUGLEVEL")); // case insensitive comparing here

	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount("any name"));
	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount(""));
}

void TIniReaderTest::testIniReader2()
{
	IniReader rd;
	rd.LoadIniFile(TEST_FILES_FOLDER "testini2.ini");

	CPPUNIT_ASSERT_EQUAL(2u, rd.SectionsCount());
	CPPUNIT_ASSERT_EQUAL(1u, rd.ValuesCount("correct section name"));
	CPPUNIT_ASSERT_EQUAL(std::string{ "00" }, rd.GetValue("correct section name", "DebugLevel"));
	CPPUNIT_ASSERT_EQUAL(std::string{ "default00" }, rd.GetValue("correct section name", "MaxLogSize", "default00")); // section exists, parameter does not exist
	CPPUNIT_ASSERT_EQUAL(2u, rd.ValuesCount("without second brace"));
	CPPUNIT_ASSERT_EQUAL(std::string{ "debug" }, rd.GetValue("without second brace", "level1"));
	CPPUNIT_ASSERT_EQUAL(std::string{ "warning" }, rd.GetValue("without second brace", "level2"));

	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount("any name"));
	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount(" "));
	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount("\n"));
	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount("\r"));
	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount("*"));
}


void TIniReaderTest::testIniReader3()
{
	IniReader rd;
	rd.LoadIniFile(TEST_FILES_FOLDER "testini3.ini");

	CPPUNIT_ASSERT_EQUAL(1u, rd.SectionsCount());
	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount(""));
	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount("name"));
	CPPUNIT_ASSERT_EQUAL(std::string{ "just def value" }, rd.GetValue("missing section name", "MaxLogSize", "just def value"));  // section doe snot exist
	CPPUNIT_ASSERT_EQUAL(std::string{ "" }, rd.GetValue("", "MaxLo", ""));

	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount(""));
	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount("\t"));
}

void TIniReaderTest::testIniReader4()
{
	IniReader rd;
	rd.LoadIniFile(TEST_FILES_FOLDER "testini4.ini");

	CPPUNIT_ASSERT_EQUAL(3u, rd.SectionsCount());
	CPPUNIT_ASSERT_EQUAL(2u, rd.ValuesCount(MISSING_SECTION));
	CPPUNIT_ASSERT_EQUAL(std::string{"11"}, rd.GetValue(MISSING_SECTION, "debuglevel"));
	CPPUNIT_ASSERT_EQUAL(std::string{""}, rd.GetValue(MISSING_SECTION, "]not correct section name["));
	CPPUNIT_ASSERT_EQUAL(1u, rd.ValuesCount(""));  // leading spaces are deleted from section name as well so section " " becomes ""
	CPPUNIT_ASSERT_EQUAL(std::string{""}, rd.GetValue("", "secvalue"));
	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount(" "));
	CPPUNIT_ASSERT_EQUAL(1u, rd.ValuesCount("!"));
}

void TIniReaderTest::testIniReader5()
{
	IniReader rd;
	rd.LoadIniFile(TEST_FILES_FOLDER "testini5.ini");

	CPPUNIT_ASSERT_EQUAL(2u, rd.SectionsCount());
	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount("leading tab section name"));
	CPPUNIT_ASSERT_EQUAL(2u, rd.ValuesCount("section after empty section"));
	CPPUNIT_ASSERT_EQUAL(std::string{ "no_value" }, rd.GetValue("leading tab section name", ""));
	CPPUNIT_ASSERT_EQUAL(std::string{ "val" }, rd.GetValue("section after empty section", "param"));
	CPPUNIT_ASSERT_EQUAL(std::string{ "" }, rd.GetValue("section after empty section", "param2"));
}

void TIniReaderTest::testIniReader6()
{
	IniReader rd;
	rd.LoadIniFile(TEST_FILES_FOLDER "testini6.ini");

	CPPUNIT_ASSERT_EQUAL(5u, rd.SectionsCount());
	CPPUNIT_ASSERT_EQUAL(true, rd.HasSection("only section names here"));
	CPPUNIT_ASSERT_EQUAL(true, rd.HasSection("ONLY section NAMES HERE")); // case insensitive
	CPPUNIT_ASSERT_EQUAL(true, rd.HasSection("1"));
	CPPUNIT_ASSERT_EQUAL(true, rd.HasSection("2"));
	CPPUNIT_ASSERT_EQUAL(true, rd.HasSection("3"));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasSection("1 "));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasSection(" 2"));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasSection("	3"));
	CPPUNIT_ASSERT_EQUAL(true, rd.HasSection("gg"));
	CPPUNIT_ASSERT_EQUAL(true, rd.HasSection("GG"));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasSection("only section names here "));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasSection("only section names her"));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasSection(""));
	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount("only section names here"));
	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount("1"));
	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount("2"));
	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount("3"));
	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount("Gg"));
	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount("GGG"));
	CPPUNIT_ASSERT_EQUAL(0u, rd.ValuesCount(" 5 "));
}

void TIniReaderTest::testIniReader7()
{
	IniReader rd;
	rd.LoadIniFile(TEST_FILES_FOLDER "testini7.ini");

	CPPUNIT_ASSERT_EQUAL(3u, rd.SectionsCount());
	CPPUNIT_ASSERT_EQUAL(true, rd.HasSection("!@#$%^&*"));
	CPPUNIT_ASSERT_EQUAL(true, rd.HasSection("["));
	CPPUNIT_ASSERT_EQUAL(true, rd.HasSection("[[["));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasSection("[[[["));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasSection(""));
	CPPUNIT_ASSERT_EQUAL(1u, rd.ValuesCount("!@#$%^&*"));
	CPPUNIT_ASSERT_EQUAL(2u, rd.ValuesCount("["));
	CPPUNIT_ASSERT_EQUAL(std::string{ "Level" }, rd.GetValue("!@#$%^&*", "DebugLEVEL"));
	CPPUNIT_ASSERT_EQUAL(std::string{ "11" }, rd.GetValue("[", "level[]"));
	CPPUNIT_ASSERT_EQUAL(std::string{ "22" }, rd.GetValue("[", "level["));
	CPPUNIT_ASSERT_EQUAL(std::string{ "no_value" }, rd.GetValue("[", "level[["));
	CPPUNIT_ASSERT_EQUAL(std::string{ "no_value" }, rd.GetValue("[[[", "["));
}


void TIniReaderTest::testIniReader8()
{

}

void TIniReaderTest::testIniReader9()
{
	IniReader rd;
	rd.LoadIniFile(TEST_FILES_FOLDER "testini9.ini");

	CPPUNIT_ASSERT_EQUAL(1u, rd.SectionsCount());
	CPPUNIT_ASSERT_EQUAL(true, rd.HasSection("Empty Params"));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasSection("empty"));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasSection("params"));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasSection("empty params "));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasSection("empty param"));
	CPPUNIT_ASSERT_EQUAL(2u, rd.ValuesCount("empty params"));
	CPPUNIT_ASSERT_EQUAL(std::string{ "54" }, rd.GetValue("empty params", "", "default", 0));
	CPPUNIT_ASSERT_EQUAL(std::string{ "warning" }, rd.GetValue("empty params", "", "default", 1));
}

void TIniReaderTest::testIniReader10()
{
	IniReader rd;
	rd.LoadIniFile(TEST_FILES_FOLDER "testini10.ini");

	CPPUNIT_ASSERT_EQUAL(1u, rd.SectionsCount());
	CPPUNIT_ASSERT_EQUAL(true, rd.HasSection("===="));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasSection(""));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasSection("="));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasSection("==="));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasSection("====="));
	CPPUNIT_ASSERT_EQUAL(4u, rd.ValuesCount("===="));
	CPPUNIT_ASSERT_EQUAL(std::string{ "===54" }, rd.GetValue("====", "!"));
	CPPUNIT_ASSERT_EQUAL(std::string{ "==warning" }, rd.GetValue("====", "^"));
	CPPUNIT_ASSERT_EQUAL(std::string{ "=====(" }, rd.GetValue("====", "*"));
	CPPUNIT_ASSERT_EQUAL(std::string{ "===)))" }, rd.GetValue("====", "((("));

	CPPUNIT_ASSERT_EQUAL(true, rd.HasValue("====", "!"));
	CPPUNIT_ASSERT_EQUAL(true, rd.HasValue("====", "^"));
	CPPUNIT_ASSERT_EQUAL(true, rd.HasValue("====", "*"));
	CPPUNIT_ASSERT_EQUAL(true, rd.HasValue("====", "((("));

	CPPUNIT_ASSERT_EQUAL(false, rd.HasValue("====", "!!"));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasValue("====", " ^"));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasValue("====", "**"));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasValue("====", "(((("));

	CPPUNIT_ASSERT_EQUAL(false, rd.HasValue(" ====", "!"));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasValue("===", "^"));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasValue("==", "*"));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasValue("==== ", "((("));
}

void TIniReaderTest::testIniReader11()
{
	IniReader rd;
	rd.LoadIniFile(TEST_FILES_FOLDER "testini11.ini");

	CPPUNIT_ASSERT_EQUAL(1u, rd.SectionsCount());
	CPPUNIT_ASSERT_EQUAL(true, rd.HasSection("samesections"));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasSection(" samesections"));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasSection(" samesections "));
	CPPUNIT_ASSERT_EQUAL(false, rd.HasSection("	samesections	"));
	CPPUNIT_ASSERT_EQUAL(4u, rd.ValuesCount("samesections"));
	CPPUNIT_ASSERT_EQUAL(std::string{ "11" }, rd.GetValue("samesections", "1"));
	CPPUNIT_ASSERT_EQUAL(std::string{ "22" }, rd.GetValue("samesections", "2"));
	CPPUNIT_ASSERT_EQUAL(std::string{ "33" }, rd.GetValue("samesections", "3"));
	CPPUNIT_ASSERT_EQUAL(std::string{ "44" }, rd.GetValue("samesections", "4"));

	CPPUNIT_ASSERT_EQUAL(true, rd.HasValue("samesections", "1"));
	CPPUNIT_ASSERT_EQUAL(true, rd.HasValue("samesections", "2"));
	CPPUNIT_ASSERT_EQUAL(true, rd.HasValue("samesections", "3"));
	CPPUNIT_ASSERT_EQUAL(true, rd.HasValue("samesections", "4"));
}

void TIniReaderTest::testIniReader12()
{

}

void TIniReaderTest::testIniReader13()
{

}