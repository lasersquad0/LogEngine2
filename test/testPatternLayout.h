#ifndef _TEST_PATTERNLAYOUT_H_
#define _TEST_PATTERNLAYOUT_H_

#include <string>
#include <cppunit/extensions/HelperMacros.h>
#include "Common.h"

/* 
 * A test case to test LogEngine PatternLayout class
 *
 */

class PatternTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( PatternTest );
  CPPUNIT_TEST( testLog0 );
  CPPUNIT_TEST( testLog1 );
  CPPUNIT_TEST( testLog2 );
  CPPUNIT_TEST( testLog3 );

  CPPUNIT_TEST_SUITE_END();
public:
    struct tm FixedTime;

	void setUp();
    void tearDown();

    void testLog0();
    void testLog1();
    void testLog2();
    void testLog3();


};


#endif // _TEST_PATTERNLAYOUT_H_
