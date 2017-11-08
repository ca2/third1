#pragma once


#include "core/core/core.h"


#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ogg/ogg.h>
#include "vorbis/codec.h"
#include "codec_internal.h"
#include "registry.h"
#include "scales.h"
#include "os.h"
#include "misc.h"
#include "codec_internal.h"
#include "os.h"
#include "misc.h"
#include "bitrate.h"


#include <errno.h>
#include <string.h>
#include <math.h>


/* we don't need or want the static callback symbols here */
#define OV_EXCLUDE_STATIC_CALLBACKS
#include "vorbis/vorbisfile.h"

#include "vorbis/vorbisenc.h"

#include "codec_internal.h"

#include "os.h"
#include "misc.h"


#include <ogg/ogg.h>
#include "codec_internal.h"
#include "registry.h"


#include "smallft.h"



#include "codebook.h"
#include "scales.h"



#include "masking.h"
#include "psy.h"
#include "lpc.h"


#include "window.h"
#include "registry.h"
#include "psy.h"
#include "misc.h"


#include "lsp.h"
#include "lookup.h"



#include "lookup_data.h"


#include "envelope.h"
#include "mdct.h"
#include "misc.h"



BEGIN_EXTERN_C
extern float * vorbis_window(vorbis_dsp_state *v,int32_t W);
END_EXTERN_C
