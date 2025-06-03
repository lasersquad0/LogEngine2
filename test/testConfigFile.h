#if !defined(TEST_CONFIGFILE)
#define TEST_CONFIGFILE

#include <cppunit/extensions/HelperMacros.h>

/*
 * A test case to test LogEngine Properties class
 *
 */

class ConfigFileTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(ConfigFileTest);

    CPPUNIT_TEST(testConfigFile01);
    CPPUNIT_TEST(testConfigFile02);
    CPPUNIT_TEST(testConfigFile03);
    CPPUNIT_TEST(testConfigFile1);
    CPPUNIT_TEST(testConfigFile2);
    CPPUNIT_TEST(testConfigFile3);
    CPPUNIT_TEST(testConfigFile4);
    CPPUNIT_TEST(testConfigFile5);
    CPPUNIT_TEST(testConfigFile6);
    CPPUNIT_TEST(testConfigFile7);
    CPPUNIT_TEST(testConfigFile8);
    CPPUNIT_TEST(testConfigFile9);
    CPPUNIT_TEST(testConfigFile10);
    CPPUNIT_TEST(testConfigFile11);
    CPPUNIT_TEST(testConfigFile12);
    CPPUNIT_TEST(testConfigFile13);
    CPPUNIT_TEST(testConfigFile14);
    CPPUNIT_TEST(testConfigFile20);
    CPPUNIT_TEST(testConfigFile21);

    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();

    void testConfigFile01();
    void testConfigFile02();
    void testConfigFile03();
    void testConfigFile1();
    void testConfigFile2();
    void testConfigFile3();
    void testConfigFile4();
    void testConfigFile5(); 
    void testConfigFile6(); 
    void testConfigFile7();
    void testConfigFile8();
    void testConfigFile9();
    void testConfigFile10();
    void testConfigFile11();
    void testConfigFile12();
    void testConfigFile13();
    void testConfigFile14();
    void testConfigFile20();
    void testConfigFile21();
};

#endif //TEST_CONFIGFILE
