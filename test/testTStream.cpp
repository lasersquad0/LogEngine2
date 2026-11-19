
#include "Shared.h"
#include "testTStream.h"



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
	//typedef TMemoryStream::pos_type pos_t;
	TMemoryStream stream;

	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(0, TSeekMode::smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(0, TSeekMode::smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(0, TSeekMode::smFromCurrent));

	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(1, TSeekMode::smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(1, TSeekMode::smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(1, TSeekMode::smFromCurrent));

	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(100, TSeekMode::smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(55, TSeekMode::smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(-1, TSeekMode::smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(-10, TSeekMode::smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(500, TSeekMode::smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(-500, TSeekMode::smFromCurrent));
}

void TStreamTest::testMemoryStream2()
{
	TMemoryStream stream;
	const char* str = "1234567890"; // 10 symbols

	stream.SetBuffer((uint8_t*)str, 10);

	using enum TSeekMode;

	CPPUNIT_ASSERT_EQUAL((pos_t)5, stream.SeekR(5, smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)6, stream.SeekR(6, smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(0, smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekR(99, smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(-1, smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(-10, smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(-0, smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(-99, smFromBegin));

	CPPUNIT_ASSERT_EQUAL((pos_t)5, stream.SeekR(5, smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)4, stream.SeekR(6, smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekR(0, smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(99, smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekR(-1, smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekR(-10, smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekR(-0, smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekR(-99, smFromEnd));

	//set position to beginning
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(0, smFromBegin));

	CPPUNIT_ASSERT_EQUAL((pos_t)5, stream.SeekR(5, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekR(6, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekR(0, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekR(99, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)9, stream.SeekR(-1, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)7, stream.SeekR(-2, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)7, stream.SeekR(-0, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(-99, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekR(-99, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekR(99, smFromCurrent));
}


void TStreamTest::testMemoryStream3()
{
	using enum TSeekMode;
	TMemoryStream stream;

	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(0, smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(0, smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(0, smFromCurrent));

	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(1, smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(1, smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(1, smFromCurrent));

	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(100, smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(55, smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(-1, smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(-10, smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(500, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(-500, smFromCurrent));
}

void TStreamTest::testMemoryStream4()
{
	using enum TSeekMode;
	TMemoryStream stream;
	const char* str = "1234567890"; // 10 symbols

	stream.SetBuffer((uint8_t*)str, 10);

	CPPUNIT_ASSERT_EQUAL((pos_t)5, stream.SeekW(5, smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)6, stream.SeekW(6, smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(0, smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekW(99, smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(-1, smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(-10, smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(-0, smFromBegin));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(-99, smFromBegin));

	CPPUNIT_ASSERT_EQUAL((pos_t)5, stream.SeekW(5, smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)4, stream.SeekW(6, smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekW(0, smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(99, smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekW(-1, smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekW(-10, smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekW(-0, smFromEnd));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekW(-99, smFromEnd));

	//set position to beginning
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(0, smFromBegin));

	CPPUNIT_ASSERT_EQUAL((pos_t)5, stream.SeekW(5, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekW(6, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekW(0, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekW(99, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)9, stream.SeekW(-1, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)7, stream.SeekW(-2, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)7, stream.SeekW(-0, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(-99, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)0, stream.SeekW(-99, smFromCurrent));
	CPPUNIT_ASSERT_EQUAL((pos_t)10, stream.SeekW(99, smFromCurrent));
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
	CPPUNIT_ASSERT_EQUAL('3', buf[0]);

	CPPUNIT_ASSERT_EQUAL(1, stream1.Read(buf, 1));
	CPPUNIT_ASSERT_EQUAL('4', buf[0]);

	CPPUNIT_ASSERT_EQUAL(2, stream1.Read(buf, 2));
	CPPUNIT_ASSERT_EQUAL(std::string("56"), std::string(buf)); 

	CPPUNIT_ASSERT_EQUAL(4, stream1.Read(buf, 10));
	CPPUNIT_ASSERT_EQUAL(std::string("7890"), std::string(buf)); 

	CPPUNIT_ASSERT_EQUAL(0, stream1.Read(buf, 5)); // nothing read, EOF reached
	CPPUNIT_ASSERT_EQUAL(std::string("7890"), std::string(buf)); 
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
	CPPUNIT_ASSERT_EQUAL((char*)"abcdefghijklmnopqrstuvwxyz", (char*)buf);

	CPPUNIT_ASSERT_EQUAL(0, stream1.Read(buf, BUF_SZ));
	CPPUNIT_ASSERT_EQUAL((char*)"abcdefghijklmnopqrstuvwxyz", (char*)buf);

	memset(buf, 0, BUF_SZ + 1);
	stream1.SeekR(0, TSeekMode::smFromBegin);
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
	stream1.SeekR(0, TSeekMode::smFromBegin);
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
	
	stream1.SeekR(0, TSeekMode::smFromBegin);
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

	stream1.SeekR(-5, TSeekMode::smFromCurrent);
	CPPUNIT_ASSERT_EQUAL(5, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"abcde", (const char*)buf);

	stream1.SeekR(-6, TSeekMode::smFromCurrent);
	CPPUNIT_ASSERT_EQUAL(5, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"abcde", (const char*)buf);

	stream1.SeekR(-4, TSeekMode::smFromCurrent);
	CPPUNIT_ASSERT_EQUAL(5, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"bcdef", (const char*)buf);

	stream1.SeekR(-2, TSeekMode::smFromBegin);
	CPPUNIT_ASSERT_EQUAL(5, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"abcde", (const char*)buf);

	stream1.SeekR(1, TSeekMode::smFromBegin);
	CPPUNIT_ASSERT_EQUAL(5, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"bcdef", (const char*)buf);

	stream1.SeekR(0, TSeekMode::smFromBegin);
	CPPUNIT_ASSERT_EQUAL(5, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"abcde", (const char*)buf);

	stream1.SeekR(0, TSeekMode::smFromEnd);
	memset(buf, 0, BUF_SZ + 1);
	CPPUNIT_ASSERT_EQUAL(0, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"", (const char*)buf);

	stream1.SeekR(1, TSeekMode::smFromEnd);
	CPPUNIT_ASSERT_EQUAL(1, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"z", (const char*)buf);

	stream1.SeekR(5, TSeekMode::smFromEnd);
	CPPUNIT_ASSERT_EQUAL(5, stream1.Read(buf, 5));
	CPPUNIT_ASSERT_EQUAL((const char*)"vwxyz", (const char*)buf);

	stream1.SeekR(-5, TSeekMode::smFromEnd);
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
	stream2 << s << '\n'; // \n this is end-of-line marker, otherwise we will not be able to read this string properly
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

// check ownership of buffer. for external buffer it should not try to free it. 
// in case of need of reallocate, exception is thrown and errono is set to EINVAL.
void TStreamTest::testMemoryStream12()
{
	TMemoryStream stream1;
	const char* str = "abcdefghijklmnopqrstuvwxyz"; // 26 symbols
	stream1.SetBuffer((uint8_t*)str, strlen(str));

	// attempt to write into stream more than 26 symbols will generate an exception
	// because mem stream does not own this buffer and cannot relocate it
	CPPUNIT_ASSERT_THROW(stream1 << "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM", IOException);

}

void TStreamTest::testMemoryStream13()
{
	TMemoryStream stream1(0); // allocate initial buffer zero bytes size
	stream1 << 'F';
	stream1 << 42; // since mem stream owns the buffer, it will be atomatically relocated

	TMemoryStream stream2(1); // allocate initial buffer 1 byte size
	stream2 << "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM";

	std::string s;
	stream2.SetBuffer((uint8_t*)s.data(), s.capacity()); // default capacity is 15
	stream2 << "ZZZ";
	//stream2 << "HHHHHHHHHHHH"; // try to write 15 symbols
    std::string ss(s.capacity() - 3, 'H'); // try to write s.capacity() symbols
    stream2 << ss;
	CPPUNIT_ASSERT_THROW(stream2 << '!', IOException); // 16th symbol overflows buffer and generates an exception because we cannot extend external buffer

	stream2.SeekW(0, TSeekMode::smFromBegin); // rewind buffer, make it available for writing again
	stream2 << "XXX"; // no exception
	//stream2 << "MMMMMMMMMMMM"; // trying to write 15 symbols, still no exception
    ss.assign(s.capacity() - 3, 'M');
    stream2 << ss; // trying to write 15 symbols, still no exception

	CPPUNIT_ASSERT_THROW(stream2 << '!', IOException); // 16th symbol overflows buffer and generates an exception

	stream2.SeekW(1, TSeekMode::smFromBegin); // rewind buffer and offset it 1
	stream2 << "YYY"; // no exception
	//stream2 << "123456789AB"; // now trying to write 14 symbols, still no exception
    ss.assign(s.capacity() - 4, '1');
    stream2 << ss;
	CPPUNIT_ASSERT_THROW(stream2 << 'F', IOException); // 16th symbol overflows buffer and generates an exception 

	stream2.SeekW(0, TSeekMode::smFromEnd);
	CPPUNIT_ASSERT_THROW(stream2 << '1', IOException); // 16th symbol overflows buffer

	stream2.SeekW(23, TSeekMode::smFromBegin); // actually puts position to the end of buffer
	CPPUNIT_ASSERT_THROW(stream2 << '1', IOException); // 16th symbol overflows buffer

	stream2.SeekW(1, TSeekMode::smFromEnd);
	stream2 << "Y"; // no exception
	CPPUNIT_ASSERT_THROW(stream2 << '1', IOException); // 16th symbol overflows buffer

	stream2.SeekW(15, TSeekMode::smFromEnd); // actually puts current position to the beginning of buffer
	stream2 << "123"; // no exception
	stream2 << "456789ABCDEF"; // now trying to write 14 symbols, still no exception
	CPPUNIT_ASSERT_THROW(stream2 << '0', IOException); // 16th symbol overflows buffer

	stream2.SeekW(23, TSeekMode::smFromEnd); // actually puts current position to the beginning of buffer
	stream2 << "123"; // no exception
	//stream2 << "456789ABCDEF"; // now trying to write 14 symbols, still no exception
    ss.assign(s.capacity() - 3, 'F');
    stream2 << ss;

	CPPUNIT_ASSERT_THROW(stream2 << 'F', IOException); // 16th symbol overflows buffer

}

// creating non-standard kind of string for testing
template <class CH>
struct tci_char_traits : public std::char_traits<CH>
{
private:
	static inline char mytoupper(char c) { return (char)::toupper((int)c); }
	static inline wint_t mytoupper(wchar_t w) { return ::towupper((wint_t)w); }

public:
	static bool eq(CH c1, CH c2) { return mytoupper(c1) == mytoupper(c2); }
	static bool ne(CH c1, CH c2) { return mytoupper(c1) != mytoupper(c2); }
	static bool lt(CH c1, CH c2) { return mytoupper(c1) < mytoupper(c2); }

	static int compare(const CH* s1, const CH* s2, size_t n)
	{
		CH c1, c2;
		while (n-- != 0)
		{
			c1 = mytoupper(*s1);
			c2 = mytoupper(*s2);

			if (c1 < c2) return -1;
			if (c1 > c2) return 1;
			++s1; ++s2;
		}
		return 0;
	}

	static const CH* find(const CH* s, size_t n, CH a)
	{
		CH A = (CH)mytoupper(a);
		for (size_t i = 0; i < n; i++)
		{
			if (mytoupper(*s) == A) return s;
			s++;
		}

		return nullptr;
	}
};

typedef std::basic_string<char, tci_char_traits<char>> tci_astring; // ANSI string
typedef std::basic_string<wchar_t, tci_char_traits<wchar_t>> tci_wstring; // Wide Char string
typedef std::basic_string<uint32_t, std::char_traits<uint32_t>> t32_string; // uint32_t string

//tests for f.ReadString() method for special strings: tci_astring and tci_wstring
void TStreamTest::testMemoryStreamTCIString()
{
	TMemoryStream stream1;
	stream1 << "automotovelofoto";
	stream1 << '\n';
	stream1 << '5';
	stream1 << '4';
	stream1 << '\n'; // \n this is end-of-line marker, otherwise we will not be able to read this string properly

	tci_astring readCiStr = stream1.ReadString<tci_astring>();
	CPPUNIT_ASSERT_EQUAL((const char*)"automotovelofoto", readCiStr.c_str());

	readCiStr = stream1.ReadString<tci_astring>();
	CPPUNIT_ASSERT_EQUAL((const char*)"54", readCiStr.c_str());

	readCiStr = stream1.ReadString<tci_astring>();
	CPPUNIT_ASSERT_EQUAL((const char*)"", readCiStr.c_str());
	CPPUNIT_ASSERT(stream1.Eof());

	TMemoryStream stream2;
	stream2 << L"automotovelofoto";
	stream2 << L'\n';
	stream2 << L'5';
	stream2 << L'4';
	stream2 << L'\n'; // \n this is end-of-line marker, otherwise we will not be able to read this string properly

	tci_wstring readCiWStr = stream2.ReadString<tci_wstring>();
	CPPUNIT_ASSERT(tci_wstring(L"automotovelofoto") == readCiWStr);

	readCiWStr = stream2.ReadString<tci_wstring>();
	CPPUNIT_ASSERT(tci_wstring(L"54") == readCiWStr);

	readCiWStr = stream2.ReadString<tci_wstring>();
	CPPUNIT_ASSERT(tci_wstring(L"") == readCiWStr);
	CPPUNIT_ASSERT(stream2.Eof());

}

//tests for f.ReadString() method for different kinds of strings
void TStreamTest::testMemoryStream14()
{
	TMemoryStream stream1;
	const char* str = "abcde\nf\n"; // 26 symbols

	stream1.SetBuffer((uint8_t*)str, strlen(str));

	CPPUNIT_ASSERT_EQUAL(8ull, strlen(str));
	
	std::string readStr = stream1.ReadString<std::string>();
	CPPUNIT_ASSERT_EQUAL(std::string( "abcde" ), readStr);
	CPPUNIT_ASSERT(!stream1.Eof());

	readStr = stream1.ReadString<std::string>();
	CPPUNIT_ASSERT_EQUAL(std::string{ "f" }, readStr);
	CPPUNIT_ASSERT(stream1.Eof());

	readStr = stream1.ReadString<std::string>();
	CPPUNIT_ASSERT_EQUAL(std::string{ "" }, readStr);
	CPPUNIT_ASSERT(stream1.Eof());


	TMemoryStream stream3;
	stream3 << L"\nAA\nBB\n\n";

	std::wstring readWStr = stream3.ReadString<std::wstring>();
	CPPUNIT_ASSERT_EQUAL(0ull, readWStr.size()); // empty string

	readWStr = stream3.ReadString<std::wstring>();
	CPPUNIT_ASSERT(std::wstring{ L"AA" } == readWStr); // for wstring CPPUNIT_ASSERT_EQUAL does not compile

	readWStr = stream3.ReadString<std::wstring>();
	CPPUNIT_ASSERT(std::wstring{ L"BB" } == readWStr);

	readWStr = stream3.ReadString<std::wstring>();
	CPPUNIT_ASSERT(std::wstring{ L"" } == readWStr);
	CPPUNIT_ASSERT(stream3.Eof());

	TMemoryStream stream4;
	stream4 << "\r\nAA\nBB\r\nCC\rDD\n";

	readStr = stream4.ReadString<std::string>();
	CPPUNIT_ASSERT_EQUAL(0ull, readStr.size()); // empty string

	readStr = stream4.ReadString<std::string>();
	CPPUNIT_ASSERT_EQUAL(std::string{ "AA" }, readStr); 

	readStr = stream4.ReadString<std::string>();
	CPPUNIT_ASSERT_EQUAL(std::string{ "BB" }, readStr);

	readStr = stream4.ReadString<std::string>();
	CPPUNIT_ASSERT_EQUAL(std::string{ "CC\rDD" }, readStr);
	CPPUNIT_ASSERT(stream4.Eof());

	readStr = stream4.ReadString<std::string>();
	CPPUNIT_ASSERT_EQUAL(std::string{ "" }, readStr);
	readStr = stream4.ReadString<std::string>();
	CPPUNIT_ASSERT_EQUAL(std::string{ "" }, readStr);

	CPPUNIT_ASSERT(stream4.Eof()); 

	TMemoryStream stream5;
	stream4 << "DD5";

	readStr = stream4.ReadString<std::string>();
	CPPUNIT_ASSERT_EQUAL(std::string{ "DD5" }, readStr); 
	CPPUNIT_ASSERT(stream4.Eof());

	readStr = stream4.ReadString<std::string>();
	CPPUNIT_ASSERT_EQUAL(std::string{ "" }, readStr);
	CPPUNIT_ASSERT(stream4.Eof());

}

// testing f.ReadString() method when reading from a file
void TStreamTest::testFileStream1()
{
	TFileStream stream1(TEST_FILES_FOLDER "5lines.txt", TFileMode::fmRead);
	
	std::string readStr = stream1.ReadString<std::string>();
	CPPUNIT_ASSERT_EQUAL(std::string("aaa"), readStr);
	CPPUNIT_ASSERT(!stream1.Eof());

	readStr = stream1.ReadString<std::string>();
	CPPUNIT_ASSERT_EQUAL(std::string{ "bbb" }, readStr);
	CPPUNIT_ASSERT(!stream1.Eof());

	readStr = stream1.ReadString<std::string>();
	CPPUNIT_ASSERT_EQUAL(std::string{ "" }, readStr);
	CPPUNIT_ASSERT(!stream1.Eof());

	readStr = stream1.ReadString<std::string>();
	CPPUNIT_ASSERT_EQUAL(std::string{ "ccc" }, readStr);
	CPPUNIT_ASSERT(!stream1.Eof());

	readStr = stream1.ReadString<std::string>();
	CPPUNIT_ASSERT_EQUAL(std::string{ "" }, readStr);
	CPPUNIT_ASSERT(stream1.Eof());

	readStr = stream1.ReadString<std::string>();
	CPPUNIT_ASSERT_EQUAL(std::string{ "" }, readStr);
	CPPUNIT_ASSERT(stream1.Eof());
}