
#include <string>

#ifdef _WIN32
#include <windows.h>

std::string DisplaySystemVersion()
{
    OSVERSIONINFOEX osvi;
    BOOL bOsVersionInfoEx;

    // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
    // If that fails, try using the OSVERSIONINFO structure.
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if (!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*)&osvi)))
    {
        // If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if (!GetVersionEx((OSVERSIONINFO*)&osvi))
            return "";
    }

    std::string ver;

    switch (osvi.dwPlatformId)
    {
        // Tests for Windows NT product family.
    case VER_PLATFORM_WIN32_NT:
        // Test for the product.
        if (bOsVersionInfoEx)  // Use information from GetVersionEx.
        {
            if (osvi.dwMajorVersion <= 4)
                ver.append("Microsoft Windows NT ");
            if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
                ver.append("Microsoft Windows 2000 ");


            // Test for the workstation type.
            if (osvi.wProductType == VER_NT_WORKSTATION)
            {
                if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
                    ver.append("Microsoft Windows XP ");
                if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
                    ver.append("Microsoft Windows Vista ");
                if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1)
                    ver.append("Microsoft Windows 7 ");
                if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2)
                    ver.append("Microsoft Windows 8 ");
                if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 3)
                    ver.append("Microsoft Windows 8.1 ");
                if (osvi.dwMajorVersion == 10 && osvi.dwMinorVersion == 0)
                    ver.append("Microsoft Windows 10 ");

                if (osvi.wSuiteMask & VER_SUITE_PERSONAL)
                    ver.append("Home Edition ");
                if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
                    ver.append("Enterprise ");
                if (osvi.wSuiteMask & VER_SUITE_TERMINAL)
                    ver.append("Terminal Services ");
                if (osvi.wSuiteMask & VER_SUITE_SMALLBUSINESS)
                    ver.append("Small Business Server ");
                if (osvi.wSuiteMask & VER_SUITE_EMBEDDEDNT)
                    ver.append("Embedded ");
            }

            // Test for the server type.
            else if (osvi.wProductType == VER_NT_SERVER)
            {
                if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
                    ver.append("Microsoft Windows .NET ");
                if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
                    ver.append("Microsoft Windows Home Server ");
                if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
                    ver.append("Microsoft Windows Server 2008 ");
                if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1)
                    ver.append("Microsoft Windows Server 2008 R2 ");
                if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2)
                    ver.append("Microsoft Windows 2012 ");
                if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 3)
                    ver.append("Microsoft Windows Server 2012 R2 ");
                if (osvi.dwMajorVersion == 10 && osvi.dwMinorVersion == 0)
                    ver.append("Microsoft Windows Server 2016 ");

                if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
                    ver.append("DataCenter Server ");
                else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
                    if (osvi.dwMajorVersion == 4)
                        ver.append("Advanced Server ");
                    else
                        ver.append("Enterprise Server ");
                else if (osvi.wSuiteMask == VER_SUITE_BLADE)
                    ver.append("Web Server ");
                //  else
                //     ver.append( "Server " );
            }
        }
        else   // Use the registry on early versions of Windows NT.
        {
            HKEY hKey;
            char szProductType[80];
            DWORD dwBufLen;

            RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\ProductOptions", 0, KEY_QUERY_VALUE, &hKey);
            RegQueryValueEx(hKey, "ProductType", NULL, NULL, (LPBYTE)szProductType, &dwBufLen);
            RegCloseKey(hKey);
            if (lstrcmpi("WINNT", szProductType) == 0)
                ver.append("Professional ");
            if (lstrcmpi("LANMANNT", szProductType) == 0)
                ver.append("Server ");
            if (lstrcmpi("SERVERNT", szProductType) == 0)
                ver.append("Advanced Server ");
        }

        // Display version, service pack (if any), and build number.

        //if (osvi.dwMajorVersion <= 4 )
        //{
        #define BUF_SZ 100
        char buf[BUF_SZ];
        sprintf_s(buf, BUF_SZ, "version %d.%d %s (Build %d)\n", 
                osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.szCSDVersion, osvi.dwBuildNumber & 0xFFFF);
        ver.append(buf);
        /*}
        else
        {
            printf ("%s (Build %d)\n",
                osvi.szCSDVersion,
                osvi.dwBuildNumber & 0xFFFF);
        }*/
        break;

        // Test for the Windows 95 product family.
    case VER_PLATFORM_WIN32_WINDOWS:

        if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
        {
            ver.append("Microsoft Windows 95 ");
            if (osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B')
                ver.append("OSR2 ");
        }

        if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
        {
            ver.append("Microsoft Windows 98 ");
            if (osvi.szCSDVersion[1] == 'A')
                ver.append("SE ");
        }

        if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
        {
            ver.append("Microsoft Windows Millennium Edition ");
        }
        break;
    }
    return ver;
}

#else

std::string DisplaySystemVersion(const char* filename)
{
    system("uname -a");

    std::string ldd_cmd("ldd ");
    ldd_cmd.append(filename);
    system(ldd_cmd.c_str());
}
#endif /* _WIN32 */