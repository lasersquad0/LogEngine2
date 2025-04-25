#ifndef UTIL_TESTS_HEADER
#define UTIL_TESTS_HEADER

#include "cppunit/extensions/HelperMacros.h"

/* 
 * A test case to test LogEngine utils 
 *
 */

class UtilsTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( UtilsTest );
  CPPUNIT_TEST( testStringReplace1 );
  CPPUNIT_TEST( testStringReplace2 );  
  CPPUNIT_TEST( testStringReplace3 );  
  CPPUNIT_TEST( testStringReplace4 );  
  CPPUNIT_TEST( testStringReplace5 );  
  CPPUNIT_TEST( testStringReplace6 );
  CPPUNIT_TEST( testStringReplace7 );
  CPPUNIT_TEST( testExtractFileName1 );  
  CPPUNIT_TEST( testExtractFileName2 );  
  CPPUNIT_TEST( testExtractFileName3 );  
  CPPUNIT_TEST( testExtractFileName4 );  
  CPPUNIT_TEST( testIntToStr1 );  
  CPPUNIT_TEST( testIntToStr2 );  
  CPPUNIT_TEST(testStripFileExt1);
  CPPUNIT_TEST(testStripFileExt2);
  CPPUNIT_TEST(testStripFileExt3);
  CPPUNIT_TEST(testStripFileExt4);
  CPPUNIT_TEST(testExtractFileDir1);
  CPPUNIT_TEST(testExtractFileDir2);
  CPPUNIT_TEST(testExtractFileDir3);
  CPPUNIT_TEST(testExtractFileDir4);
  CPPUNIT_TEST(testExtractFileExt1);
  CPPUNIT_TEST(testExtractFileExt2);
  CPPUNIT_TEST(testExtractFileExt3);
  CPPUNIT_TEST(testExtractFileExt4);
  CPPUNIT_TEST(testTrimSPCRLF1);
  CPPUNIT_TEST(testTrimSPCRLF2);
  CPPUNIT_TEST(testTrimSPCRLF3);
  CPPUNIT_TEST(testDelCRLF1);
  CPPUNIT_TEST(testDelCRLF2);
  CPPUNIT_TEST(testDelCRLF3);

  CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
    void tearDown();

    void testStringReplace1();
    void testStringReplace2();
    void testStringReplace3();
    void testStringReplace4();
    void testStringReplace5();
    void testStringReplace6();
    void testStringReplace7();
    void testExtractFileName1();
    void testExtractFileName2();
    void testExtractFileName3();
    void testExtractFileName4();
    void testIntToStr1();
    void testIntToStr2();
    void testStripFileExt1();
    void testStripFileExt2();
    void testStripFileExt3();
    void testStripFileExt4();
    void testExtractFileDir1();
    void testExtractFileDir2();
    void testExtractFileDir3();
    void testExtractFileDir4();
    void testExtractFileExt1();
    void testExtractFileExt2();
    void testExtractFileExt3();
    void testExtractFileExt4();
    void testTrimSPCRLF1();
    void testTrimSPCRLF2();
    void testTrimSPCRLF3();
    void testDelCRLF1();
    void testDelCRLF2();
    void testDelCRLF3();
};

#endif // UTIL_TESTS_HEADER
