
#include "Shared.h"
#include "testTStream.h"
#include "FileStream.h"


using namespace LogEngine;

CPPUNIT_TEST_SUITE_REGISTRATION(TStreamTest);
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TStreamTest, "TStreamTest");

void TStreamTest::setUp()
{

}

void TStreamTest::tearDown()
{
	// free memory allocated in setUp, do other things
}

void TStreamTest::testMemoryStream1()
{
	TMemoryStream stream;

	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(0, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(0, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(0, smFromCurrent));

	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(1, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(1, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(1, smFromCurrent));

	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(100, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(55, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(-1, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(-10, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(500, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(-500, smFromCurrent));
}

void TStreamTest::testMemoryStream2()
{
	TMemoryStream stream;
	const char* str = "1234567890"; // 10 symbols

	stream.SetBuffer((uint8_t*)str, 10);

	CPPUNIT_ASSERT_EQUAL(5l, stream.SeekR(5, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(6l, stream.SeekR(6, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(0, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekR(99, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(-1, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(-10, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(-0, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(-99, smFromBegin));

	CPPUNIT_ASSERT_EQUAL(5l, stream.SeekR(5, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(4l, stream.SeekR(6, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekR(0, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(99, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekR(-1, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekR(-10, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekR(-0, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekR(-99, smFromEnd));

	//set position to beginning
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(0, smFromBegin));

	CPPUNIT_ASSERT_EQUAL(5l, stream.SeekR(5, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekR(6, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekR(0, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekR(99, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(9l, stream.SeekR(-1, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(7l, stream.SeekR(-2, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(7l, stream.SeekR(-0, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(-99, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekR(-99, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekR(99, smFromCurrent));
}

void TStreamTest::testMemoryStream3()
{
}

void TStreamTest::testMemoryStream4()
{
}

void TStreamTest::testMemoryStream5()
{
}

void TStreamTest::testMemoryStream6()
{
}
