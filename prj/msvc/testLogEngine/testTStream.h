#if !defined(TEST_TSTREAM_H)
#define TEST_TSTREAM_H

#include <cppunit/extensions/HelperMacros.h>

/*
 * A test case to test TMemoryStream and TFileStream classes
 *
 */

class TStreamTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(TStreamTest);
    CPPUNIT_TEST(testMemoryStream1);
    CPPUNIT_TEST(testMemoryStream2);
    CPPUNIT_TEST(testMemoryStream3);
    CPPUNIT_TEST(testMemoryStream4);
    CPPUNIT_TEST(testMemoryStream5);
    CPPUNIT_TEST(testMemoryStream6);
    CPPUNIT_TEST(testMemoryStream7);
    CPPUNIT_TEST(testMemoryStream8);
    CPPUNIT_TEST(testMemoryStream9);
    CPPUNIT_TEST(testMemoryStream10);
    CPPUNIT_TEST(testMemoryStream11);
    CPPUNIT_TEST(testMemoryStream12);
    CPPUNIT_TEST(testMemoryStream13);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();

    void testMemoryStream1();
    void testMemoryStream2();
    void testMemoryStream3();
    void testMemoryStream4();
    void testMemoryStream5();
    void testMemoryStream6();
    void testMemoryStream7();
    void testMemoryStream8();
    void testMemoryStream9();
    void testMemoryStream10();
    void testMemoryStream11();
    void testMemoryStream12();
    void testMemoryStream13();
};



#endif //TEST_TSTREAM_H
