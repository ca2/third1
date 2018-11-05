/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2009             *
 * by the Xiph.Org Foundation http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************

 function: libvorbis codec headers
 last mod: $Id: codec_internal.h 16227 2009-07-08 06:58:46Z xiphmont $

 ********************************************************************/

#ifndef _V_CODECI_H_
#define _V_CODECI_H_

#include "envelope.h"
#include "codebook.h"

BEGIN_EXTERN_C

#define BLOCKTYPE_IMPULSE    0
#define BLOCKTYPE_PADDING    1
#define BLOCKTYPE_TRANSITION 0
#define BLOCKTYPE_LONG       1

#define PACKETBLOBS 15

typedef struct vorbis_block_internal{
  float  **pcmdelay;  /* this is a pointer into local storage */
  float  ampmax;
  int32_t    blocktype;

  oggpack_buffer *packetblob[PACKETBLOBS]; /* initialized, must be freed;
                                              blob [PACKETBLOBS/2] points to
                                              the oggpack_buffer in the
                                              main vorbis_block */
} vorbis_block_internal;

typedef void vorbis_look_floor;
typedef void vorbis_look_residue;
typedef void vorbis_look_transform;

/* mode ************************************************************/
typedef struct {
  int32_t blockflag;
  int32_t windowtype;
  int32_t transformtype;
  int32_t mapping;
} vorbis_info_mode;

typedef void vorbis_info_floor;
typedef void vorbis_info_residue;
typedef void vorbis_info_mapping;

#include "psy.h"
#include "bitrate.h"

typedef struct private_state {
  /* local lookup storage */
  envelope_lookup        *ve; /* envelope lookup */
  int32_t                     window[2];
  vorbis_look_transform **transform[2];    /* block, type */
  drft_lookup             fft_look[2];

  int32_t                     modebits;
  vorbis_look_floor     **flr;
  vorbis_look_residue   **residue;
  vorbis_look_psy        *psy;
  vorbis_look_psy_global *psy_g_look;

  /* local storage, only used on the encoding side.  This way the
     application does not need to worry about freeing some packets'
     memory and not others'; packet storage is always tracked.
     Cleared next call to a _dsp_ function */
  uchar *header;
  uchar *header1;
  uchar *header2;

  bitrate_manager_state bms;

  int64_t sample_count;
} private_state;

/* codec_setup_info contains all the setup information specific to the
   specific compression/decompression mode in progress (eg,
   psychoacoustic settings, channel setup, options, codebook
   etc).
*********************************************************************/

#include "highlevel.h"
typedef struct codec_setup_info {

  /* Vorbis supports only int16_t and long blocks, but allows the
     encoder to choose the sizes */

  long blocksizes[2];

  /* modes are the primary means of supporting on-the-fly different
     blocksizes, different channel mappings (LR or M/A),
     different residue backends, etc.  Each mode consists of a
     blocksize flag and a mapping (along with the mapping setup */

  int32_t        modes;
  int32_t        maps;
  int32_t        floors;
  int32_t        residues;
  int32_t        books;
  int32_t        psys;     /* encode only */

  vorbis_info_mode       *mode_param[64];
  int32_t                     map_type[64];
  vorbis_info_mapping    *map_param[64];
  int32_t                     floor_type[64];
  vorbis_info_floor      *floor_param[64];
  int32_t                     residue_type[64];
  vorbis_info_residue    *residue_param[64];
  static_codebook        *book_param[256];
  codebook               *fullbooks;

  vorbis_info_psy        *psy_param[4]; /* encode only */
  vorbis_info_psy_global psy_g_param;

  bitrate_manager_info   bi;
  highlevel_encode_setup hi; /* used only by vorbisenc.c.  It's a
                                highly redundant structure, but
                                improves clarity of program flow. */
  int32_t         halfrate_flag; /* painless downsample for decode */
} codec_setup_info;

extern vorbis_look_psy_global *_vp_global_look(vorbis_info *vi);
extern void _vp_global_free(vorbis_look_psy_global *look);



typedef struct {
  int32_t sorted_index[VIF_POSIT+2];
  int32_t forward_index[VIF_POSIT+2];
  int32_t reverse_index[VIF_POSIT+2];

  int32_t hineighbor[VIF_POSIT];
  int32_t loneighbor[VIF_POSIT];
  int32_t posts;

  int32_t n;
  int32_t quant_q;
  vorbis_info_floor1 *vi;

  long phrasebits;
  long postbits;
  long frames;
} vorbis_look_floor1;



extern int32_t *floor1_fit(vorbis_block *vb,vorbis_look_floor1 *look,
                          const float *logmdct,   /* in */
                          const float *logmask);
extern int32_t *floor1_interpolate_fit(vorbis_block *vb,vorbis_look_floor1 *look,
                          int32_t *A,int32_t *B,
                          int32_t del);
extern int32_t floor1_encode(oggpack_buffer *opb,vorbis_block *vb,
                  vorbis_look_floor1 *look,
                  int32_t *post,int32_t *ilogmask);


END_EXTERN_C
#endif
