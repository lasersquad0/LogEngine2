
#include "Shared.h"
#include "LogEngine.h"
#include "RotatingFileSink.h"
#include "testConfigFile.h"


CPPUNIT_TEST_SUITE_REGISTRATION(ConfigFileTest);


using namespace LogEngine;

void ConfigFileTest::setUp()
{
}

void ConfigFileTest::tearDown()
{
    ClearLoggers();
}

void ConfigFileTest::testConfigFile1()
{
    InitFromFile(TEST_FILES_FOLDER "test1.lfg");

    CPPUNIT_ASSERT_EQUAL(2u, GetLoggersCount());

    Logger& logger1 = GetLogger("1");
    CPPUNIT_ASSERT_EQUAL(0u, logger1.SinkCount());
    CPPUNIT_ASSERT_EQUAL(Levels::llDebug, logger1.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger1.GetAsyncMode());

    Logger& logger2 = GetLogger("a");
    CPPUNIT_ASSERT_EQUAL(0u, logger2.SinkCount());
    CPPUNIT_ASSERT_EQUAL(LL_DEFAULT, logger2.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger2.GetAsyncMode());

}

void ConfigFileTest::testConfigFile2()
{
    InitFromFile(TEST_FILES_FOLDER "test2.lfg");
    
    CPPUNIT_ASSERT_EQUAL(2u, GetLoggersCount());
    
    Logger& logger1 = LogEngine::GetLogger("!");
    CPPUNIT_ASSERT_EQUAL(LL_DEFAULT, logger1.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger1.GetAsyncMode());
    CPPUNIT_ASSERT_EQUAL(0u, logger1.SinkCount());

    Logger& logger2 = LogEngine::GetLogger("!!");
    CPPUNIT_ASSERT_EQUAL(0u, logger2.SinkCount());
    CPPUNIT_ASSERT_EQUAL(LL_DEFAULT, logger2.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger2.GetAsyncMode());

}

void ConfigFileTest::testConfigFile3()
{
    InitFromFile(TEST_FILES_FOLDER "test3.lfg");

    CPPUNIT_ASSERT_EQUAL(1u, GetLoggersCount());

    Logger& logger = GetLogger("..");
    CPPUNIT_ASSERT_EQUAL(Levels::llTrace, logger.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(true, logger.GetAsyncMode());
    CPPUNIT_ASSERT_EQUAL(0u, logger.SinkCount());

}

void ConfigFileTest::testConfigFile4()
{
    InitFromFile(TEST_FILES_FOLDER "test4.lfg");

    CPPUNIT_ASSERT_EQUAL(2u, GetLoggersCount());

    Logger& logger1 = GetLogger("ABC");
    CPPUNIT_ASSERT_EQUAL(Levels::llDebug, logger1.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger1.GetAsyncMode());
    CPPUNIT_ASSERT_EQUAL(0u, logger1.SinkCount());

    Logger& logger2 = GetLogger("CdE");
    CPPUNIT_ASSERT_EQUAL(Levels::llCritical, logger2.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger2.GetAsyncMode());
    CPPUNIT_ASSERT_EQUAL(0u, logger2.SinkCount());

}

void ConfigFileTest::testConfigFile5()
{
    InitFromFile(TEST_FILES_FOLDER "test5.lfg");

    CPPUNIT_ASSERT_EQUAL(0u, GetLoggersCount());
}

void ConfigFileTest::testConfigFile6()
{
    InitFromFile(TEST_FILES_FOLDER "test6.lfg");

    CPPUNIT_ASSERT_EQUAL(1u, GetLoggersCount());

    Logger& logger1 = GetLogger("RotLog");
    CPPUNIT_ASSERT_EQUAL(LL_DEFAULT, logger1.GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(false, logger1.GetAsyncMode());
    CPPUNIT_ASSERT_EQUAL(1u, logger1.SinkCount());
    Sink* sink = logger1.GetSink("Siii");
    
    CPPUNIT_ASSERT_EQUAL(Levels::llError, sink->GetLogLevel());
    CPPUNIT_ASSERT_EQUAL(true, sink == dynamic_cast<RotatingFileSink*>(sink)); // check sink type
  
}

void ConfigFileTest::testConfigFile20()
{
    InitFromFile(TEST_FILES_FOLDER "example2.lfg");
}




/*
#ifdef _WIN32
#include <windows.h>

BOOL DisplaySystemVersion(const char *filename)
{
   OSVERSIONINFOEX osvi;
   BOOL bOsVersionInfoEx;

   // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
   // If that fails, try using the OSVERSIONINFO structure.

   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
   {
      // If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
      osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
      if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
         return FALSE;
   }

   switch (osvi.dwPlatformId)
   {
      // Tests for Windows NT product family.
      case VER_PLATFORM_WIN32_NT:

      // Test for the product.

         if ( osvi.dwMajorVersion <= 4 )
            printf("Microsoft Windows NT ");

         if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
            printf ("Microsoft Windows 2000 ");


         if( bOsVersionInfoEx )  // Use information from GetVersionEx.
         { 
         // Test for the workstation type.
         /*   if ( osvi.wProductType == VER_NT_WORKSTATION )
            {
               if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
                  printf ("Microsoft Windows XP ");

               if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
                  printf ( "Home Edition " );
               else
                  printf ( "Professional " );
            }

         // Test for the server type.
            else if ( osvi.wProductType == VER_NT_SERVER )
            {
               if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
                  printf ("Microsoft Windows .NET ");

               if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                  printf ( "DataCenter Server " );
               else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                  if( osvi.dwMajorVersion == 4 )
                     printf ("Advanced Server " );
                  else
                     printf ( "Enterprise Server " );
               else if ( osvi.wSuiteMask == VER_SUITE_BLADE )
                  printf ( "Web Server " );
               else
                  printf ( "Server " );
            }*/
  /*       }
         else   // Use the registry on early versions of Windows NT.
         {
            HKEY hKey;
            char szProductType[80];
            DWORD dwBufLen;

            RegOpenKeyEx( HKEY_LOCAL_MACHINE,
               "SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
               0, KEY_QUERY_VALUE, &hKey );
            RegQueryValueEx( hKey, "ProductType", NULL, NULL,
               (LPBYTE) szProductType, &dwBufLen);
            RegCloseKey( hKey );
            if ( lstrcmpi( "WINNT", szProductType) == 0 )
               printf( "Professional " );
            if ( lstrcmpi( "LANMANNT", szProductType) == 0 )
               printf( "Server " );
            if ( lstrcmpi( "SERVERNT", szProductType) == 0 )
				printf( "Advanced Server " );
         }
		 
		 // Display version, service pack (if any), and build number.
		 
         if ( osvi.dwMajorVersion <= 4 )
         {
			 printf ("version %d.%d %s (Build %d)\n",
				 osvi.dwMajorVersion,
				 osvi.dwMinorVersion,
				 osvi.szCSDVersion,
				 osvi.dwBuildNumber & 0xFFFF);
         }
         else
         { 
			 printf ("%s (Build %d)\n",
				 osvi.szCSDVersion,
				 osvi.dwBuildNumber & 0xFFFF);
         }
         break;
		 
		 // Test for the Windows 95 product family.
      case VER_PLATFORM_WIN32_WINDOWS:
		  
		  if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		  {
			  printf ("Microsoft Windows 95 ");
			  if ( osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B' )
				  printf("OSR2 " );
		  } 
		  
		  if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		  {
			  printf ("Microsoft Windows 98 ");
			  if ( osvi.szCSDVersion[1] == 'A' )
				  printf("SE " );
		  } 
		  
		  if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
		  {
			  printf ("Microsoft Windows Millennium Edition ");
		  } 
		  break;
   }
   return TRUE; 
}
#else
int DisplaySystemVersion(const char *filename)
{
    system("uname -a");

    std::string ldd_cmd("ldd ");
    ldd_cmd.append(filename);
    system(ldd_cmd.c_str());
}
#endif /* _WIN32 */
