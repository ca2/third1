#include <windows.h>

char * g_pszDirSystem = NULL;

__declspec(dllexport)
void winpr_set_dir_system(char * psystem)
{

   g_pszDirSystem = psystem;

}

__declspec(dllexport)
char * winpr_dir_system()
{

   return g_pszDirSystem;

}