
#include <cppunit/TestCase.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TextTestProgressListener.h> 
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include "MyTestProgressListener.h"

// We use Console test runner here because cppunit mftUi test runner declared as deprecated and removed from cppunit package.

int main(int argc, char* argv[])
{
    CppUnit::TestResult controller;
    CppUnit::TestResultCollector result;
    controller.addListener( &result );

    //CppUnit::BriefTestProgressListener progressListener;
    MyTestProgressListener progressListener;
    controller.addListener( &progressListener );

    CppUnit::TestRunner runner; // creates a test executor

    // use the suite factory, then execute all tests
    runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());

    // it is possible to pass cmd argument with name of Test Suite, only specified Test Suite will run in this case
    std::string testPath = (argc > 1) ? argv[1] : "";

    try 
    {
        runner.run(controller, testPath);
        
        bool wasSuccessful = result.wasSuccessful();
        return wasSuccessful ? 0 : 1;
    }
    catch (std::exception& e) 
    {
        std::cerr << "ERROR caught: " << e.what() << std::endl;
        return -1;
    }
    catch (...)
    {
        std::cerr << "UNKNOWN ERROR has been caught" << std::endl;
        return -1;
    }
 
}
