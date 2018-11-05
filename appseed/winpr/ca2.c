#pragma once

#if defined(_WIN32) && !defined(_UWP)

#include <windows.h>

BOOL get_version_ex_a(LPOSVERSIONINFOA lpVersionInformation)
{

   //#ifdef _UWP

   /* Windows 10 Version Info */
   if ((lpVersionInformation->dwOSVersionInfoSize == sizeof(OSVERSIONINFOA)) ||
      (lpVersionInformation->dwOSVersionInfoSize == sizeof(OSVERSIONINFOEXA)))
   {
      lpVersionInformation->dwMajorVersion = 10;
      lpVersionInformation->dwMinorVersion = 0;
      lpVersionInformation->dwBuildNumber = 0;
      lpVersionInformation->dwPlatformId = VER_PLATFORM_WIN32_NT;
      ZeroMemory(lpVersionInformation->szCSDVersion, sizeof(lpVersionInformation->szCSDVersion));

      if (lpVersionInformation->dwOSVersionInfoSize == sizeof(OSVERSIONINFOEXA))
      {
         LPOSVERSIONINFOEXA lpVersionInformationEx = (LPOSVERSIONINFOEXA)lpVersionInformation;
         lpVersionInformationEx->wServicePackMajor = 0;
         lpVersionInformationEx->wServicePackMinor = 0;
         lpVersionInformationEx->wSuiteMask = 0;
         lpVersionInformationEx->wProductType = VER_NT_WORKSTATION;
         lpVersionInformationEx->wReserved = 0;
      }

      return TRUE;
   }

   return FALSE;

}

#endif



