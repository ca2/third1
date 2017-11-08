#pragma once


#include "core/core/core.h"


#include <stdlib.h>
#include <ctype.h>
#include "mathops.h"
#include <limits.h>
#include <string.h>
#include <ogg/ogg.h>
#include "encint.h"
#include "internal.h"
#include "dct.h"
#include "huffenc.h"
#include "huffdec.h"
#include "decint.h"
#include "dequant.h"
#include "enquant.h"
#include "theora/theoraenc.h"
#include "theora/theoradec.h"
#include "cpu.h"
#include "bitpack.h"

#include "modedec.h"

#if defined(OC_X86_ASM)
#if defined(_MSC_VER)
# include "x86_vc/x86int.h"
#else
# include "x86/x86int.h"
#endif
# include "x86/x86enc.h"
#endif
#if defined(OC_DUMP_IMAGES)
# include "png.h"
#endif


#include "quant.h"
#include "apiwrapper.h"


#if defined(HAVE_CAIRO)
# include <cairo.h>
#endif
