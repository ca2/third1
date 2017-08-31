#include "StdAfx.h"


extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
   // remove this if you use lpReserved
   UNREFERENCED_PARAMETER(lpReserved);

   if (dwReason == DLL_PROCESS_ATTACH)
   {
      ::OutputDebugString("::ca2:: ca.dll :: initializing!\n");
   }
   else if (dwReason == DLL_PROCESS_DETACH)
   {
      ::OutputDebugString("::ca2:: ca.dll :: terminating!\n");
   }
   return 1;   // ok
}


