
#include "Shared.h"
#include "Common.h"
#include "testUtils.h"

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( LogEngineUtilsTest, "LogEngineUtilsTest");

void LogEngineUtilsTest::setUp ()
{

}

void LogEngineUtilsTest::tearDown ()
{
    // free memory allocated in setUp, do other things
}

void LogEngineUtilsTest::testStringReplace1()
{
	std::string s = StringReplace("", "", "");
	CPPUNIT_ASSERT_EQUAL((size_t)0, s.size());
}

void LogEngineUtilsTest::testStringReplace2()
{
	std::string s = StringReplace("", "a", "b");
	CPPUNIT_ASSERT_EQUAL((size_t)0, s.size());
}

void LogEngineUtilsTest::testStringReplace3()
{
	std::string s = StringReplace("", "zzz", "bbb");
	CPPUNIT_ASSERT_EQUAL((size_t)0, s.size());
}

void LogEngineUtilsTest::testStringReplace4()
{
	std::string s = StringReplace("", "zzz", "");
	CPPUNIT_ASSERT_EQUAL((size_t)0, s.size());
}

void LogEngineUtilsTest::testStringReplace5()
{
	std::string s = StringReplace("", "", "bbbb");
	CPPUNIT_ASSERT_EQUAL((size_t)0, s.size());
}

void LogEngineUtilsTest::testStringReplace6()
{
	std::string s = StringReplace("gaaah vvvvb aa aaa hghg4aaa54", "aaa", "ZZZZ");
	CPPUNIT_ASSERT(s.compare("gZZZZh vvvvb aa ZZZZ hghg4ZZZZ54") == 0);
}

void LogEngineUtilsTest::testStringReplace7()
{
	std::string s = StringReplace("gaaah vvvvb aa aaa hghg4aaa", "aaa", "");
	CPPUNIT_ASSERT_EQUAL(std::string("gh vvvvb aa  hghg4"), s);
}

void LogEngineUtilsTest::testExtractFileName1()
{
	std::string s = ExtractFileName("c:\\aaa\\bbb\\pp.log");
	CPPUNIT_ASSERT_EQUAL(std::string("pp.log"), s);

	s = ExtractFileName("");
	CPPUNIT_ASSERT_EQUAL(std::string(""), s);

	s = ExtractFileName("/");
	CPPUNIT_ASSERT_EQUAL(std::string("/"), s);

	s = ExtractFileName("\\");
	CPPUNIT_ASSERT_EQUAL(std::string("\\"), s);

	s = ExtractFileName("c:");
	CPPUNIT_ASSERT_EQUAL(std::string("c:"), s);
}

void LogEngineUtilsTest::testExtractFileName2()
{
	std::string s = ExtractFileName("\\sssss.ss\\dsd\\ffffff");
	CPPUNIT_ASSERT_EQUAL(std::string{ "ffffff" }, s);
}

void LogEngineUtilsTest::testExtractFileName3()
{
	std::string s = ExtractFileName("/dfdfd/dfdf/gfg/dffff/");
	CPPUNIT_ASSERT_EQUAL(std::string(""), s);

	s = ExtractFileName("/dfdfd/dfdf/gfg/d4444");
	CPPUNIT_ASSERT_EQUAL(std::string("d4444"), s);
}

void LogEngineUtilsTest::testExtractFileName4()
{
	std::string s = ExtractFileName("app.log.fgh");
    CPPUNIT_ASSERT_EQUAL(std::string("app.log.fgh"), s);
}

void LogEngineUtilsTest::testIntToStr1()
{
	CPPUNIT_ASSERT_EQUAL(std::string("1"), IntToStr(1, 1));
	CPPUNIT_ASSERT_EQUAL(std::string("34"), IntToStr(34, 1));
	CPPUNIT_ASSERT_EQUAL(std::string("34"), IntToStr(34, 2));
}

void LogEngineUtilsTest::testIntToStr2()
{
	CPPUNIT_ASSERT_EQUAL(std::string("123450"), IntToStr(123450, 6));
	CPPUNIT_ASSERT_EQUAL(std::string("123450"), IntToStr(123450, 5));
	CPPUNIT_ASSERT_EQUAL(std::string("123450 "), IntToStr(123450, 7));
	CPPUNIT_ASSERT_EQUAL(std::string("12     "), IntToStr(12, 7));
}

void LogEngineUtilsTest::testStripFileExt1()
{
	std::string s = StripFileExt("c:\\aaa\\bbb\\pp.log");
	CPPUNIT_ASSERT_EQUAL(std::string("c:\\aaa\\bbb\\pp"), s);

	s = StripFileExt("");
	CPPUNIT_ASSERT_EQUAL(std::string(""), s);

	s = StripFileExt("\\");
	CPPUNIT_ASSERT_EQUAL(std::string("\\"), s);

	s = StripFileExt("/");
	CPPUNIT_ASSERT_EQUAL(std::string("/"), s);

	s = StripFileExt("c:");
	CPPUNIT_ASSERT_EQUAL(std::string("c:"), s);
}

void LogEngineUtilsTest::testStripFileExt2()
{
	std::string s = StripFileExt("\\sssss.ss\\dsd\\ffffff");
	CPPUNIT_ASSERT_EQUAL(std::string{ "\\sssss.ss\\dsd\\ffffff" }, s);

	StripFileExt("\\sssss.ss\\dsd\\ffffff.");
	CPPUNIT_ASSERT_EQUAL(std::string{ "\\sssss.ss\\dsd\\ffffff" }, s);

	StripFileExt("\\sssss.ss\\dsd\\ffffff.a");
	CPPUNIT_ASSERT_EQUAL(std::string{ "\\sssss.ss\\dsd\\ffffff" }, s);

	StripFileExt("\\sssss.ss\\dsd\\a. rrffffff ");
	CPPUNIT_ASSERT_EQUAL(std::string{ "\\sssss.ss\\dsd\\a" }, s);
}

void LogEngineUtilsTest::testStripFileExt3()
{
	std::string s = StripFileExt("/dfdfd/dfdf/gfg/dffff/");
	CPPUNIT_ASSERT_EQUAL(std::string("/dfdfd/dfdf/gfg/dffff/"), s);

	s = StripFileExt("/dfdfd/dfdf/gfg/d4444.rrr.ttt.y");
	CPPUNIT_ASSERT_EQUAL(std::string("/dfdfd/dfdf/gfg/d4444.rrr.ttt"), s);

	s = StripFileExt("c:\\aaa\\bbb\\pp.log");
	CPPUNIT_ASSERT_EQUAL(std::string("c:\\aaa\\bbb"), s);
}

void LogEngineUtilsTest::testStripFileExt4()
{
	std::string s = StripFileExt("app.log.fgh");
	CPPUNIT_ASSERT_EQUAL(std::string("app.log"), s);

	s = StripFileExt("app.log.fgh.");
	CPPUNIT_ASSERT_EQUAL(std::string("app.log.fgh"), s);

	s = StripFileExt("a.b");
	CPPUNIT_ASSERT_EQUAL(std::string("a"), s);

	s = StripFileExt("a");
	CPPUNIT_ASSERT_EQUAL(std::string("a"), s);
}

void LogEngineUtilsTest::testExtractFileDir1()
{
	std::string s = ExtractFileDir("\\sssss.ss\\dsd\\ffffff");
	CPPUNIT_ASSERT_EQUAL(std::string{ "\\sssss.ss\\dsd" }, s);

	s = ExtractFileDir("\\sssss.ss\\dsd\\ffffff.");
	CPPUNIT_ASSERT_EQUAL(std::string{ "\\sssss.ss\\dsd" }, s);

	s = ExtractFileDir("\\sssss.ss\\dsd\\ffffff.a");
	CPPUNIT_ASSERT_EQUAL(std::string{ "\\sssss.ss\\dsd" }, s);

	s = ExtractFileDir("\\sssss.ss\\dsd\\a. rrffffff ");
	CPPUNIT_ASSERT_EQUAL(std::string{ "\\sssss.ss\\dsd" }, s);
}

void LogEngineUtilsTest::testExtractFileDir2()
{
	std::string s = ExtractFileDir("/dfdfd/dfdf/gfg/dffff/");
	CPPUNIT_ASSERT_EQUAL(std::string("/dfdfd/dfdf/gfg/dffff/"), s);

	s = ExtractFileDir("c:/dfdfd/dfdf/gfg/d4444.rrr.ttt.y");
	CPPUNIT_ASSERT_EQUAL(std::string("c:/dfdfd/dfdf/gfg"), s);
}

void LogEngineUtilsTest::testExtractFileDir3()
{
	std::string s = ExtractFileDir("app.log.fgh");
	CPPUNIT_ASSERT_EQUAL(std::string(""), s);

	s = ExtractFileDir("/app.log.fgh.");
	CPPUNIT_ASSERT_EQUAL(std::string(""), s);

	s = ExtractFileDir("a.b/");
	CPPUNIT_ASSERT_EQUAL(std::string("a.b/"), s);

}

void LogEngineUtilsTest::testExtractFileDir4()
{
	std::string s = ExtractFileDir("");
	CPPUNIT_ASSERT_EQUAL(std::string{ "" }, s);

	s = ExtractFileDir("a");
	CPPUNIT_ASSERT_EQUAL(std::string("a"), s);

	s = ExtractFileDir("/");
	CPPUNIT_ASSERT_EQUAL(std::string(""), s);

	s = ExtractFileDir("\\");
	CPPUNIT_ASSERT_EQUAL(std::string("\\"), s);
}
