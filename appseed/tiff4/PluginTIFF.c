#include "tiffiop.h"
#include "tif_predict.h"
#include <stdlib.h>


// ----------------------------------------------------------
//   TIFF library FreeImage-specific routines.
// ----------------------------------------------------------

void*
_TIFFmalloc(tmsize_t s)
{
   return malloc(s);
}

void
_TIFFfree(void *p)
{
   free(p);
}

void*
_TIFFrealloc(void* p, tmsize_t s)
{
   return realloc(p, s);
}

void
_TIFFmemset(void* p, int v, tmsize_t c)
{
   memset(p, v, (size_t)c);
}

void
_TIFFmemcpy(void* d, const void* s, tmsize_t c)
{
   memcpy(d, s, (size_t)c);
}

int
_TIFFmemcmp(const void* p1, const void* p2, tmsize_t c)
{
   return (memcmp(p1, p2, (size_t)c));
}

// ----------------------------------------------------------
//   in FreeImage warnings and errors are disabled
// ----------------------------------------------------------

static void
msdosWarningHandler(const char* module, const char* fmt, va_list ap)
{
}

TIFFErrorHandler _TIFFwarningHandler = msdosWarningHandler;


static void
msdosErrorHandler(const char* module, const char* fmt, va_list ap)
{

   // use this for diagnostic only (do not use otherwise, even in DEBUG mode)
   /*
   if (module != NULL) {
   char msg[1024];
   vsprintf(msg, fmt, ap);
   FreeImage_OutputMessageProc(s_format_id, "%s: %s", module, msg);
   }
   */
}

TIFFErrorHandler _TIFFerrorHandler = msdosErrorHandler;

