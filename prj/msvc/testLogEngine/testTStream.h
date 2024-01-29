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
};



#endif //TEST_TSTREAM_H
