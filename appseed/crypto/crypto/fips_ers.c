#include <openssl/opensslconf.h>

#ifdef OPENSSL_FIPS
# include "fips_err.h"
#else
#ifndef DUMMY
#define DUMMY
static void *dummy=&dummy;
#endif
#endif
