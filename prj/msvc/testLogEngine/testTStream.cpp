
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
	TMemoryStream stream;

	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(0, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(0, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(0, smFromCurrent));

	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(1, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(1, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(1, smFromCurrent));

	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(100, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(55, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(-1, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(-10, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(500, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(-500, smFromCurrent));
}

void TStreamTest::testMemoryStream4()
{
	TMemoryStream stream;
	const char* str = "1234567890"; // 10 symbols

	stream.SetBuffer((uint8_t*)str, 10);

	CPPUNIT_ASSERT_EQUAL(5l, stream.SeekW(5, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(6l, stream.SeekW(6, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(0, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekW(99, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(-1, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(-10, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(-0, smFromBegin));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(-99, smFromBegin));

	CPPUNIT_ASSERT_EQUAL(5l, stream.SeekW(5, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(4l, stream.SeekW(6, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekW(0, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(99, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekW(-1, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekW(-10, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekW(-0, smFromEnd));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekW(-99, smFromEnd));

	//set position to beginning
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(0, smFromBegin));

	CPPUNIT_ASSERT_EQUAL(5l, stream.SeekW(5, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekW(6, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekW(0, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekW(99, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(9l, stream.SeekW(-1, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(7l, stream.SeekW(-2, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(7l, stream.SeekW(-0, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(-99, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(0l, stream.SeekW(-99, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL(10l, stream.SeekW(99, smFromCurrent));
}

void TStreamTest::testMemoryStream5()
{
	TMemoryStream stream1;

	CPPUNIT_ASSERT_EQUAL(-1, stream1.ReadChar());
	CPPUNIT_ASSERT_EQUAL(-1, stream1.ReadChar());

	CPPUNIT_ASSERT_THROW(stream1.ReadPString(), IOException);

	TMemoryStream stream2;
	char buf[100];
	
	CPPUNIT_ASSERT_EQUAL(-1, stream2.Read(nullptr, 0));
	CPPUNIT_ASSERT_EQUAL(-1, stream2.Read(nullptr, 1));
	CPPUNIT_ASSERT_EQUAL(-1, stream2.Read(nullptr, 99));

	CPPUNIT_ASSERT_EQUAL(-1, stream2.Read(buf, 0));
	CPPUNIT_ASSERT_EQUAL(0, stream2.Read(buf, 1));
	CPPUNIT_ASSERT_EQUAL(0, stream2.Read(buf, 99));

}

void TStreamTest::testMemoryStream6()
{
	TMemoryStream stream1;
	const char* str = "1234567890"; // 10 symbols
	stream1.SetBuffer((uint8_t*)str, 10);

	CPPUNIT_ASSERT_EQUAL((int)'1', stream1.ReadChar());
	CPPUNIT_ASSERT_EQUAL((int)'2', stream1.ReadChar());

	CPPUNIT_ASSERT_EQUAL(-1, stream1.Read(nullptr, 0));
	CPPUNIT_ASSERT_EQUAL(-1, stream1.Read(nullptr, 1));
	CPPUNIT_ASSERT_EQUAL(-1, stream1.Read(nullptr, 99));

	char buf[15]{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	CPPUNIT_ASSERT_EQUAL(-1, stream1.Read(buf, 0));
	CPPUNIT_ASSERT_EQUAL(1, stream1.Read(buf, 1));
	CPPUNIT_ASSERT_EQUAL((char*)"3", (char*)buf);

	CPPUNIT_ASSERT_EQUAL(1, stream1.Read(buf, 1));
	CPPUNIT_ASSERT_EQUAL((char*)"4", (char*)buf);

	CPPUNIT_ASSERT_EQUAL(2, stream1.Read(buf, 2));
	CPPUNIT_ASSERT_EQUAL((char*)"56", (char*)buf); 

	CPPUNIT_ASSERT_EQUAL(4, stream1.Read(buf, 10));
	CPPUNIT_ASSERT_EQUAL((char*)"7890", (char*)buf); 

	CPPUNIT_ASSERT_EQUAL(0, stream1.Read(buf, 5)); // nothing read, EOF reached
	CPPUNIT_ASSERT_EQUAL((char*)"7890", (char*)buf); 
}

void TStreamTest::testMemoryStream7()
{
	TMemoryStream stream1;
	const char* str = "abcdefghijklmnopqrstuvwxyz"; // 26 symbols
	stream1.SetBuffer((uint8_t*)str, strlen(str));

	const int BUF_SZ = 26;
	char buf[BUF_SZ + 1]; // extra byte for terminating zero
	memset(buf, 0, BUF_SZ + 1);

	CPPUNIT_ASSERT_EQUAL(BUF_SZ, stream1.Read(buf, BUF_SZ));
	CPPUNIT_ASSERT_EQUAL((const char*)"abcdefghijklmnopqrstuvwxyz", (const char*)buf);

	CPPUNIT_ASSERT_EQUAL(0, stream1.Read(buf, BUF_SZ));
	CPPUNIT_ASSERT_EQUAL((char*)"abcdefghijklmnopqrstuvwxyz", (char*)buf);

	memset(buf, 0, BUF_SZ + 1);
	stream1.SeekR(0, smFromBegin);
	CPPUNIT_ASSERT_EQUAL(BUF_SZ, stream1.Read(buf, BUF_SZ + 1));
	CPPUNIT_ASSERT_EQUAL((char*)"abcdefghijklmnopqrstuvwxyz", (char*)buf);

	CPPUNIT_ASSERT_EQUAL(0, stream1.Read(buf, 20));
	CPPUNIT_ASSERT_EQUAL((char*)"abcdefghijklmnopqrstuvwxyz", (char*)buf);
}


void TStreamTest::testMemoryStream8()
{
	TMemoryStream stream1;
	const char* str = "abcdefghijklmnopqrstuvwxyz"; // 26 symbols
	stream1.SetBuffer((uint8_t*)str, strlen(str));

	const int BUF_SZ = 35;
	char buf[BUF_SZ];
	memset(buf, 0, BUF_SZ);

	CPPUNIT_ASSERT_EQUAL(26, stream1.Read(buf, BUF_SZ));
	CPPUNIT_ASSERT_EQUAL((char*)"abcdefghijklmnopqrstuvwxyz", (char*)buf);

	CPPUNIT_ASSERT_EQUAL(0, stream1.Read(buf, BUF_SZ));
	CPPUNIT_ASSERT_EQUAL((char*)"abcdefghijklmnopqrstuvwxyz", (char*)buf);

	memset(buf, 0, BUF_SZ);
	stream1.SeekR(0, smFromBegin);
	CPPUNIT_ASSERT_EQUAL(20, stream1.Read(buf, 20));
	CPPUNIT_ASSERT_EQUAL((char*)"abcdefghijklmnopqrst", (char*)buf);

	memset(buf, 0, BUF_SZ); // reset buf after previous Read()
	CPPUNIT_ASSERT_EQUAL(6, stream1.Read(buf, 20));
	CPPUNIT_ASSERT_EQUAL((const char*)"uvwxyz", (const char*)buf);

	CPPUNIT_ASSERT_EQUAL(0, stream1.Read(buf, 20));
	CPPUNIT_ASSERT_EQUAL((char*)"uvwxyz", (char*)buf);
}

void TStreamTest::testMemoryStream9()
{
	TMemoryStream stream1;
	const char* str = "abcdefghijklmnopqrstuvwxyz"; // 26 symbols
	stream1.SetBuffer((uint8_t*)str, strlen(str));

	const int BUF_SZ = 26;
	char buf[BUF_SZ + 1]; // extra byte for terminating zero
	memset(buf, 0, BUF_SZ + 1);

	CPPUNIT_ASSERT_EQUAL(BUF_SZ - 1, stream1.Read(buf, BUF_SZ - 1));
	CPPUNIT_ASSERT_EQUAL((const char*)"abcdefghijklmnopqrstuvwxy", (const char*)buf);

	CPPUNIT_ASSERT_EQUAL(1, stream1.Read(buf, 1));
	CPPUNIT_ASSERT_EQUAL((char*)"zbcdefghijklmnopqrstuvwxy", (char*)buf);

	CPPUNIT_ASSERT_EQUAL(0, stream1.Read(buf, 1));
	CPPUNIT_ASSERT_EQUAL((char*)"zbcdefghijklmnopqrstuvwxy", (char*)buf);
	
	stream1.SeekR(0, smFromBegin);
	memset(buf, 0, BUF_SZ + 1); 
	CPPUNIT_ASSERT_EQUAL(BUF_SZ - 1, stream1.Read(buf, BUF_SZ - 1));
	CPPUNIT_ASSERT_EQUAL((char*)"abcdefghijklmnopqrstuvwxy", (char*)buf);

	CPPUNIT_ASSERT_EQUAL(1, stream1.Read(buf, 2));
	CPPUNIT_ASSERT_EQUAL((char*)"zbcdefghijklmnopqrstuvwxy", (char*)buf);
	CPPUNIT_ASSERT_EQUAL(0, stream1.Read(buf, 2));
	CPPUNIT_ASSERT_EQUAL((char*)"zbcdefghijklmnopqrstuvwxy", (char*)buf);

}

void TStreamTest::testMemoryStream10()
{
	TMemoryStream stream1;
	const char* str = "abcdefghijklmnopqrstuvwxyz"; // 26 symbols
	stream1.SetBuffer((uint8_t*)str, strlen(str));

	const int BUF_SZ = 26;
	char buf[BUF_SZ + 1]; // extra byte for terminating zero
	memset(buf, 0, BUF_SZ + 1);

	CPPUNIT_ASSERT_EQUAL(5, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"abcde", (const char*)buf);

	stream1.SeekR(-5, smFromCurrent);
	CPPUNIT_ASSERT_EQUAL(5, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"abcde", (const char*)buf);

	stream1.SeekR(-6, smFromCurrent);
	CPPUNIT_ASSERT_EQUAL(5, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"abcde", (const char*)buf);

	stream1.SeekR(-4, smFromCurrent);
	CPPUNIT_ASSERT_EQUAL(5, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"bcdef", (const char*)buf);

	stream1.SeekR(-2, smFromBegin);
	CPPUNIT_ASSERT_EQUAL(5, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"abcde", (const char*)buf);

	stream1.SeekR(1, smFromBegin);
	CPPUNIT_ASSERT_EQUAL(5, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"bcdef", (const char*)buf);

	stream1.SeekR(0, smFromBegin);
	CPPUNIT_ASSERT_EQUAL(5, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"abcde", (const char*)buf);

	stream1.SeekR(0, smFromEnd);
	memset(buf, 0, BUF_SZ + 1);
	CPPUNIT_ASSERT_EQUAL(0, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"", (const char*)buf);

	stream1.SeekR(1, smFromEnd);
	CPPUNIT_ASSERT_EQUAL(1, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"z", (const char*)buf);

	stream1.SeekR(5, smFromEnd);
	CPPUNIT_ASSERT_EQUAL(5, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"vwxyz", (const char*)buf);

	stream1.SeekR(-5, smFromEnd);
	CPPUNIT_ASSERT_EQUAL(0, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"vwxyz", (const char*)buf); // buf is not changed since previous call

}

void TStreamTest::testMemoryStream11()
{
	TMemoryStream stream1;
	const char* str = "abcdefghijklmnopqrstuvwxyz"; // 26 symbols
	stream1.SetBuffer((uint8_t*)str, strlen(str));

	const int BUF_SZ = 26;
	char buf[BUF_SZ + 1]; // extra byte for terminating zero
	memset(buf, 0, BUF_SZ + 1);
	char c;

	stream1 >> c;
	CPPUNIT_ASSERT_EQUAL('a', c);
	stream1 >> c;
	CPPUNIT_ASSERT_EQUAL('b', c);

	std::string s;
	stream1 >> s;
	CPPUNIT_ASSERT_EQUAL(std::string{ "cdefghijklmnopqrstuvwxyz" }, s);

	TMemoryStream stream2;
	s = "automotovelofoto";
	stream2 << 5;
	stream2 << '5';
	stream2 << 3.1415265859;
	stream2 << s;
	stream2 << 1000000000ll;

	//stream2.SeekR(stream2.Length(), smFromEnd);
	int i;
	double d;
	long long ll;

	stream2 >> i;
	CPPUNIT_ASSERT_EQUAL(5, i);

	stream2 >> c;
	CPPUNIT_ASSERT_EQUAL('5', c);

	stream2 >> d;
	CPPUNIT_ASSERT_EQUAL(3.1415265859, d);

	s = "";
	stream2 >> s;
	CPPUNIT_ASSERT_EQUAL(std::string{"automotovelofoto"}, s);

	stream2 >> ll;
	CPPUNIT_ASSERT_EQUAL(1000000000ll, ll);

	s = "4444";
	stream2 << s;

	s = "5555";
	stream2 >> s;
	CPPUNIT_ASSERT_EQUAL(std::string{ "4444" }, s);

	CPPUNIT_ASSERT_THROW(stream2 >> ll, IOException);
}

// check ovnership of buffer. for external buffer it should not try to free it. 
// in case of need of reallocate error (-1) is returned and errono is set to EINVAL.
void TStreamTest::testMemoryStream12()
{
	TMemoryStream stream1;
	const char* str = "abcdefghijklmnopqrstuvwxyz"; // 26 symbols
	stream1.SetBuffer((uint8_t*)str, strlen(str));

	CPPUNIT_ASSERT_NO_THROW(stream1 << "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM");

}

// check ovnership of buffer. for external buffer it should not try to free it. 
// in case of need of reallocate error (-1) is returned and errono is set to EINVAL.
void TStreamTest::testMemoryStream13()
{
	TMemoryStream stream1(0); // allocate buffer zero bytes size
	stream1 << 'F';
	stream1 << 42;

	TMemoryStream stream2(1); // allocate buffer 1 byte size
	stream2 << "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM";

	std::string s;
	stream2.SetBuffer((uint8_t*)s.data(), s.capacity()); // default capacity is 15
	stream2 << "ZZZ";

	stream2 << "HHHHHHHHHHHH"; // try to write 15 symbols

	CPPUNIT_ASSERT_THROW(stream2 << '!', IOException); // 16th symbol overflows buffer and generates an exception because we cannot extend external buffer
}