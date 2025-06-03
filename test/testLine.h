#if !defined(TEST_LINE)
#define TEST_LINE

#include "cppunit/extensions/HelperMacros.h"

/* 
 * A test case to test LogEngine Line class
 *
 */

class LineTest : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( LineTest );
  CPPUNIT_TEST( testLine1 );
  CPPUNIT_TEST( testLine2 );
  CPPUNIT_TEST( testLine3 );
  CPPUNIT_TEST( testLine4 );
  CPPUNIT_TEST( testLine5 );
  CPPUNIT_TEST( testLine6 );
  CPPUNIT_TEST(testLine6_1);
  CPPUNIT_TEST( testLine7 );
  CPPUNIT_TEST( testLine8 );
  CPPUNIT_TEST( testLine9 );
  CPPUNIT_TEST( testLine10 );
  CPPUNIT_TEST( testLine11 );
  CPPUNIT_TEST( testLine12 );
  CPPUNIT_TEST( testLine13 );
  CPPUNIT_TEST( testLine14 );
  CPPUNIT_TEST( testLine15 );
  CPPUNIT_TEST(testLine16);
  CPPUNIT_TEST(testLine17);
  CPPUNIT_TEST(testLine18);
  CPPUNIT_TEST(testLine19);
  CPPUNIT_TEST(testLine20);
  CPPUNIT_TEST(testLine21);
  CPPUNIT_TEST(testLine22);
  CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
    void tearDown();

    void testLine1();
    void testLine2();
    void testLine3();
    void testLine4();
    void testLine5();
    void testLine6();
    void testLine6_1();
    void testLine7();
    void testLine8();
    void testLine9();
    void testLine10();
    void testLine11();
    void testLine12();
    void testLine13();
    void testLine14();
    void testLine15();
    void testLine16();
    void testLine17();
    void testLine18();
    void testLine19();
    void testLine20();
    void testLine21();
    void testLine22();
};



#endif //TEST_LINE
