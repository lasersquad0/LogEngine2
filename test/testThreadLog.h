#ifndef _TEST_THREAD_LOGGER_H_
#define _TEST_THREAD_LOGGER_H_

#include <cppunit/extensions/HelperMacros.h>

/* 
 * A test case to test LogEngine Logger class in multithreading environment
 *
 */

class ThreadLogTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( ThreadLogTest );
  CPPUNIT_TEST( testCallLogFromManyThreads );
  CPPUNIT_TEST(testAsyncLog1);
  //CPPUNIT_TEST(testThreadLogMeasureTime1);
  //CPPUNIT_TEST(testNONThreadLogMeasureTime1);
  CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
    void tearDown();

    void testCallLogFromManyThreads();
    void testAsyncLog1();
    void testThreadLogMeasureTime1();
    void testNONThreadLogMeasureTime1();
};

#endif //_TEST_THREAD_LOGGER_H_
