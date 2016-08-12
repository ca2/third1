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

 function: PCM data envelope analysis and manipulation
 last mod: $Id: envelope.h 16227 2009-07-08 06:58:46Z xiphmont $

 ********************************************************************/

#ifndef _V_ENVELOPE_
#define _V_ENVELOPE_

#include "mdct.h"

BEGIN_EXTERN_C

#define VE_PRE    16
#define VE_WIN    4
#define VE_POST   2
#define VE_AMP    (VE_PRE+VE_POST-1)

#define VE_BANDS  7
#define VE_NEARDC 15

#define VE_MINSTRETCH 2   /* a bit less than int16_t block */
#define VE_MAXSTRETCH 12  /* one-third full block */

typedef struct {
  float ampbuf[VE_AMP];
  int32_t   ampptr;

  float nearDC[VE_NEARDC];
  float nearDC_acc;
  float nearDC_partialacc;
  int32_t   nearptr;

} envelope_filter_state;

typedef struct {
  int32_t begin;
  int32_t end;
  float *window;
  float total;
} envelope_band;

typedef struct {
  int32_t ch;
  int32_t winlength;
  int32_t searchstep;
  float minenergy;

  mdct_lookup  mdct;
  float       *mdct_win;

  envelope_band          band[VE_BANDS];
  envelope_filter_state *filter;
  int32_t   stretch;

  int32_t                   *mark;

  long storage;
  long current;
  long curmark;
  long cursor;
} envelope_lookup;

extern void _ve_envelope_init(envelope_lookup *e,vorbis_info *vi);
extern void _ve_envelope_clear(envelope_lookup *e);
extern long _ve_envelope_search(vorbis_dsp_state *v);
extern void _ve_envelope_shift(envelope_lookup *e,long shift);
extern int32_t  _ve_envelope_mark(vorbis_dsp_state *v);

END_EXTERN_C

#endif
