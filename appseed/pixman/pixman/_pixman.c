#ifdef HAVE_CONFIG_H
#include<config.h>
#endif

#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<math.h>

#include"pixman-accessor.h"
#include"pixman-private.h"
#include"pixman-combine32.h"
#include"pixman-inlines.h"

//O volume na unidade C n∆o tem nome.
 //O N£mero de SÇrie do Volume Ç 72AC-5E14

 //Pasta de C:\nordroid\app\axis\pixman\pixman

//#include "pixman-access-accessors.c"
//#include "pixman-access.c"
#ifdef USE_ARM_NEON
#include "pixman-arm-neon.c"
#endif
#ifdef USE_ARM_SIMD
#include "pixman-arm-simd.c"
#endif
#if defined(USE_ARM_NEON) || defined(USE_ARM_SIMD)
//#include "pixman-arm.c"
#endif
#include "pixman-bits-image.c"
//#include "pixman-combine-float.c"
//#include "pixman-combine32.c"
#include "pixman-conical-gradient.c"
// #include "pixman-edge-accessors.c"
// #include "pixman-edge.c"
#include "pixman-fast-path.c"
#include "pixman-filter.c"
#include "pixman-general.c"
#include "pixman-glyph.c"
#include "pixman-gradient-walker.c"
#include "pixman-image.c"
#include "pixman-implementation.c"
#include "pixman-linear-gradient.c"
#include "pixman-matrix.c"
//#ifdef USE_MIPS
//#include "pixman-mips-dspr2.c"
#include "pixman-mips.c"
//#endif
#include "pixman-mmx.c"
#include "pixman-noop.c"
#include "pixman-ppc.c"
#include "pixman-radial-gradient.c"
//#include "pixman-region.c"
//#include "pixman-region16.c"
//#include "pixman-region32.c"
//#include "pixman-solid-fill.c"
//#include "pixman-sse2.c"
//#include "pixman-ssse3.c"
#include "pixman-timer.c"
#include "pixman-trap.c"
#include "pixman-utils.c"
#ifdef ALTIVEC
#include "pixman-vmx.c"
#endif
#include "pixman-x86.c"
#include "pixman.c"
//2015-01-04  19:55                 0 _pixman.c"
//              39 arquivo(s)        993.802 bytes
//               0 pasta(s)   19.290.767.360 bytes dispon°veis
