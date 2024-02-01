#pragma once

#include <cppunit/extensions/HelperMacros.h>

/*
 * A test case to test IniReader  class
 *
 */

class TIniReaderTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(TIniReaderTest);
    CPPUNIT_TEST(testIniReader1);
    CPPUNIT_TEST(testIniReader2);
    CPPUNIT_TEST(testIniReader3);
    CPPUNIT_TEST(testIniReader4);
    CPPUNIT_TEST(testIniReader5);
    CPPUNIT_TEST(testIniReader6);
    CPPUNIT_TEST(testIniReader7);
    CPPUNIT_TEST(testIniReader8);
    CPPUNIT_TEST(testIniReader9);
    CPPUNIT_TEST(testIniReader10);
    CPPUNIT_TEST(testIniReader11);
    CPPUNIT_TEST(testIniReader12);
    CPPUNIT_TEST(testIniReader13);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();

    void testIniReader1();
    void testIniReader2();
    void testIniReader3();
    void testIniReader4();
    void testIniReader5();
    void testIniReader6();
    void testIniReader7();
    void testIniReader8();
    void testIniReader9();
    void testIniReader10();
    void testIniReader11();
    void testIniReader12();
    void testIniReader13();
};

