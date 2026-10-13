#ifndef _TEST_LOGGER_H_
#define _TEST_LOGGER_H_

#include <string>
#include <cppunit/extensions/HelperMacros.h>


/* 
 * A test case to test LogEngine Logger class
 *
 */

class LoggerTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( LoggerTest );
  CPPUNIT_TEST( testLog0 );
  CPPUNIT_TEST( testLog1 );
  CPPUNIT_TEST( testLog2 );
  CPPUNIT_TEST( testLog3 );
  CPPUNIT_TEST( testLog4 );
  CPPUNIT_TEST( testLogTwoSinksOneFileLock );
  CPPUNIT_TEST( testLogTwoSinksOneFile );
  CPPUNIT_TEST( testLogDuplicateSinks1 );
  CPPUNIT_TEST( testLogDuplicateSinks2 );
  CPPUNIT_TEST( testLogDuplicateSinks3 );
  CPPUNIT_TEST( testLogStrategyNone );
  CPPUNIT_TEST( testLogStrategySingle );
  CPPUNIT_TEST( testLogStrategyTimeStamp );
  CPPUNIT_TEST( testLogStrategyBakNumber );
  CPPUNIT_TEST( testLogStatistic );
  CPPUNIT_TEST( testGetFileLogger );
  CPPUNIT_TEST( testGetStdoutLogger );
  CPPUNIT_TEST( testGetStderrLogger );
  CPPUNIT_TEST( testLogPerfromanceST_ASyncFalse );
  CPPUNIT_TEST( testLogPerfromanceST_ASyncTrue );
  CPPUNIT_TEST( testLogPerfromanceMT_ASyncFalse );
  CPPUNIT_TEST( testLogPerfromanceLock1 );
  /* CPPUNIT_TEST( testLogRotation1 );*/
  CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
    void tearDown();

    void testLog0();
    void testLog1();
    void testLog2();
    void testLog3();
    void testLog4();
    void testLogTwoSinksOneFile();
    void testLogTwoSinksOneFileLock();
    void testLogDuplicateSinks1();
    void testLogDuplicateSinks2(); 
    void testLogDuplicateSinks3(); 
    void testLogStrategyNone();
    void testLogStrategySingle();
    void testLogStrategyTimeStamp();
    void testLogStrategyBakNumber();
    void testLogStatistic();
    void testGetFileLogger();
    void testGetStdoutLogger();
    void testGetStderrLogger();
    void testLogPerfromanceST_ASyncFalse();
    void testLogPerfromanceST_ASyncTrue();
    void testLogPerfromanceMT_ASyncFalse();
    void testLogPerfromanceLock1();
    // void testLogRotation1();
};



#endif // _TEST_LOGGER_H_
