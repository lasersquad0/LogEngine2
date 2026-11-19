#ifndef _TEST_TSTREAM_H_
#define _TEST_TSTREAM_H_

#include "cppunit/extensions/HelperMacros.h"
#include "FileStream.h"

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
    CPPUNIT_TEST(testMemoryStream11W);
    CPPUNIT_TEST(testMemoryStream12);
    CPPUNIT_TEST(testMemoryStream13);
    CPPUNIT_TEST(testMemoryStream14);
    CPPUNIT_TEST(testFileStream1);
    CPPUNIT_TEST(testMemoryStreamTCIString);
    CPPUNIT_TEST_SUITE_END();
public:
    typedef LogEngine::TMemoryStream::pos_type pos_t;

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
    void testMemoryStream11W();
    void testMemoryStream12();
    void testMemoryStream13();
    void testMemoryStream14();
    void testMemoryStreamTCIString();
    void testFileStream1();
};



#endif //_TEST_TSTREAM_H_
