#ifndef THREAD_LOG_TESTS_HEADER
#define THREAD_LOG_TESTS_HEADER

#include <cppunit/extensions/HelperMacros.h>

/* 
 * A test case to test LogEngine ThreadLog class
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

#endif //THREAD_LOG_TESTS_HEADER
