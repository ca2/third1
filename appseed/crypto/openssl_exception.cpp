#include "cryptlib.h"


class openssl_exception
{
public:

   char * file;
   int line;
   char * assertion;
   char * message;

};

extern "C"
void OpenSSLDie(const char *file,int line,const char *assertion)
{

   openssl_exception e;

   e.file = _strdup(file);
   e.line = line;
   e.assertion = _strdup(assertion);

   throw e;

}

extern "C"
void OPENSSL_showfatal(const char *fmta,...)
{
   openssl_exception e;
   char sz[2048];
   va_list ap;
   va_start(ap,fmta);
   sprintf(sz,fmta,ap);
   va_end(ap);
   e.message = _strdup(sz);
   throw e;
}