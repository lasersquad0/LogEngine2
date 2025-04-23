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

    CPPUNIT_TEST(testConfigFile1);
    CPPUNIT_TEST(testConfigFile2);
    CPPUNIT_TEST(testConfigFile3);
    CPPUNIT_TEST(testConfigFile4);
    CPPUNIT_TEST(testConfigFile5);
    CPPUNIT_TEST(testConfigFile6);
    CPPUNIT_TEST(testConfigFile7);
    CPPUNIT_TEST(testConfigFile8);
    CPPUNIT_TEST(testConfigFile9);
    CPPUNIT_TEST(testConfigFile20);
    CPPUNIT_TEST(testConfigFile21);

    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();

    void testConfigFile1();
    void testConfigFile2();
    void testConfigFile3();
    void testConfigFile4();
    void testConfigFile5(); 
    void testConfigFile6(); 
    void testConfigFile7();
    void testConfigFile8();
    void testConfigFile9();
    void testConfigFile20();
    void testConfigFile21();
};

#endif //TEST_CONFIGFILE
