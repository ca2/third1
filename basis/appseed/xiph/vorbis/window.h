/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2007             *
 * by the Xiph.Org Foundation http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************

 function: window functions
 last mod: $Id: window.h 13293 2007-07-24 00:09:47Z xiphmont $

 ********************************************************************/

#ifndef _V_WINDOW_
#define _V_WINDOW_





BEGIN_EXTERN_C





extern const float *_vorbis_window_get(int32_t n);
extern void _vorbis_apply_window(float *d,int32_t *winno,long *blocksizes,
                          int32_t lW,int32_t W,int32_t nW);






END_EXTERN_C






#endif
