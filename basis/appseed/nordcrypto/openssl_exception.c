#include "cryptlib.h"


struct openssl_exception
{

   char * file;
   int line;
   char * assertion;
   char * message;

};

void openssl_throw_exception(struct openssl_exception * pe);

void OpenSSLDie(const char *file,int line,const char *assertion)
{

   struct openssl_exception e;

   e.file = _strdup(file);
   e.line = line;
   e.assertion = _strdup(assertion);

   openssl_throw_exception(&e);

}

void OPENSSL_showfatal(const char *fmta,...)
{
   struct openssl_exception e;
   char sz[2048];
   va_list ap;
   va_start(ap,fmta);
   sprintf(sz,fmta,ap);
   va_end(ap);
   e.message = _strdup(sz);
   openssl_throw_exception(&e);
}