#ifndef _LOG_TESTS_HEADER_
#define _LOG_TESTS_HEADER_

#include <string>
#include <cppunit/extensions/HelperMacros.h>
#include "Common.h"

/* 
 * A test case to test LogEngine Log class
 *
 */

class LoggerTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( LoggerTest );
  CPPUNIT_TEST( testLog0 );
  CPPUNIT_TEST( testLog1 );
  CPPUNIT_TEST( testLog2 );
  CPPUNIT_TEST(testLog3);
  CPPUNIT_TEST( testLog4 );
  CPPUNIT_TEST( testLog5 );
  CPPUNIT_TEST( testLogStrategyNone );
  CPPUNIT_TEST(testLogStrategySingle);
  CPPUNIT_TEST(testLogStrategyTimeStamp);
  CPPUNIT_TEST(testLogStrategyBakNumber);
  CPPUNIT_TEST( testLogStatistic );
  CPPUNIT_TEST(testGetFileLogger);
  CPPUNIT_TEST(testGetStdoutLogger);
  CPPUNIT_TEST(testGetStderrLogger);
 /* CPPUNIT_TEST( testWrong_LFG_File );
  CPPUNIT_TEST( testBadLFGFile );
  CPPUNIT_TEST( testBadLFGFile2 );
  CPPUNIT_TEST( testLogRotation1 );*/
  CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
    void tearDown();

    void testLog0();
    void testLog1();
    void testLog2();
    void testLog3();
    void testLog4();
    void testLog5();
    void testLogStrategyNone();
    void testLogStrategySingle();
    void testLogStrategyTimeStamp();
    void testLogStrategyBakNumber();
    void testLogStatistic();
    void testGetFileLogger();
    void testGetStdoutLogger();
    void testGetStderrLogger();
  /*  void testWrong_LFG_File();
    void testBadLFGFile();
    void testBadLFGFile2();
    void testLogRotation1();*/
};

inline std::string cutLog(std::string str) // cut off time and thread information (which is not same time from time)
{ 
    size_t pos = str.find_first_of('#', 1);
    pos = str.find_first_of(':', pos +  1);

    if (pos == std::string::npos) return str;

    return DelCRLF(str.erase((size_t)1, pos+1));
}

#endif // _LOG_TESTS_HEADER_
