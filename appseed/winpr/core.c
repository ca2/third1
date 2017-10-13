
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <winpr/crt.h>
#include <winpr/print.h>
#include <winpr/sysinfo.h>


#define VER_PLATFORM_WIN32s                     0
#define VER_PLATFORM_WIN32_WINDOWS              1
#define VER_PLATFORM_WIN32_NT                   2

#ifndef _WIN32

#define  VER_NT_WORKSTATION         1
#define  VER_NT_DOMAIN_CONTROLLER      2
#define  VER_NT_SERVER           3

#endif



int core_get_version_ex_a(LPOSVERSIONINFOA lpVersionInformation)
{

#if defined(_WIN32) || defined(_UWP)

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

#else

   /* Windows 7 SP1 Version Info */
   if ((lpVersionInformation->dwOSVersionInfoSize == sizeof(OSVERSIONINFOA)) ||
         (lpVersionInformation->dwOSVersionInfoSize == sizeof(OSVERSIONINFOEXA)))
   {
      lpVersionInformation->dwMajorVersion = 6;
      lpVersionInformation->dwMinorVersion = 1;
      lpVersionInformation->dwBuildNumber = 7601;
      lpVersionInformation->dwPlatformId = VER_PLATFORM_WIN32_NT;
      memset(lpVersionInformation->szCSDVersion, 0, sizeof(lpVersionInformation->szCSDVersion));

      if (lpVersionInformation->dwOSVersionInfoSize == sizeof(OSVERSIONINFOEXA))
      {
         LPOSVERSIONINFOEXA lpVersionInformationEx = (LPOSVERSIONINFOEXA)lpVersionInformation;
         lpVersionInformationEx->wServicePackMajor = 1;
         lpVersionInformationEx->wServicePackMinor = 0;
         lpVersionInformationEx->wSuiteMask = 0;
         lpVersionInformationEx->wProductType = VER_NT_WORKSTATION;
         lpVersionInformationEx->wReserved = 0;
      }

      return TRUE;
   }


#endif

   return FALSE;

}

