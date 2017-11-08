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

 function: libvorbis backend and mapping structures; needed for
           static mode headers
 last mod: $Id: backends.h 16962 2010-03-11 07:30:34Z xiphmont $

 ********************************************************************/

/* this is exposed up here because we need it for static modes.
   Lookups for each backend aren't exposed because there's no reason
   to do so */

#ifndef _vorbis_backend_h_
#define _vorbis_backend_h_

#include "codec_internal.h"

BEGIN_EXTERN_C

/* this would all be simpler/shorter with templates, but.... */
/* Floor backend generic *****************************************/
typedef struct{
  void                   (*pack)  (vorbis_info_floor *,oggpack_buffer *);
  vorbis_info_floor     *(*unpack)(vorbis_info *,oggpack_buffer *);
  vorbis_look_floor     *(*look)  (vorbis_dsp_state *,vorbis_info_floor *);
  void (*free_info) (vorbis_info_floor *);
  void (*free_look) (vorbis_look_floor *);
  void *(*inverse1)  (struct vorbis_block *,vorbis_look_floor *);
  int32_t   (*inverse2)  (struct vorbis_block *,vorbis_look_floor *,
                     void *buffer,float *);
} vorbis_func_floor;

typedef struct{
  int32_t   order;
  long  rate;
  long  barkmap;

  int32_t   ampbits;
  int32_t   ampdB;

  int32_t   numbooks; /* <= 16 */
  int32_t   books[16];

  float lessthan;     /* encode-only config setting hacks for libvorbis */
  float greaterthan;  /* encode-only config setting hacks for libvorbis */

} vorbis_info_floor0;


#define VIF_POSIT 63
#define VIF_CLASS 16
#define VIF_PARTS 31
typedef struct{
  int32_t   partitions;                /* 0 to 31 */
  int32_t   partitionclass[VIF_PARTS]; /* 0 to 15 */

  int32_t   class_dim[VIF_CLASS];        /* 1 to 8 */
  int32_t   class_subs[VIF_CLASS];       /* 0,1,2,3 (bits: 1<<n poss) */
  int32_t   class_book[VIF_CLASS];       /* subs ^ dim entries */
  int32_t   class_subbook[VIF_CLASS][8]; /* [VIF_CLASS][subs] */


  int32_t   mult;                      /* 1 2 3 or 4 */
  int32_t   postlist[VIF_POSIT+2];    /* first two implicit */


  /* encode side analysis parameters */
  float maxover;
  float maxunder;
  float maxerr;

  float twofitweight;
  float twofitatten;

  int32_t   n;

} vorbis_info_floor1;

/* Residue backend generic *****************************************/
typedef struct{
  void                 (*pack)  (vorbis_info_residue *,oggpack_buffer *);
  vorbis_info_residue *(*unpack)(vorbis_info *,oggpack_buffer *);
  vorbis_look_residue *(*look)  (vorbis_dsp_state *,
                                 vorbis_info_residue *);
  void (*free_info)    (vorbis_info_residue *);
  void (*free_look)    (vorbis_look_residue *);
  long **(*m_class)      (struct vorbis_block *,vorbis_look_residue *,
                        int32_t **,int32_t *,int32_t);
  int32_t  (*forward)      (oggpack_buffer *,struct vorbis_block *,
                        vorbis_look_residue *,
                        int32_t **,int32_t *,int32_t,long **,int32_t);
  int32_t  (*inverse)      (struct vorbis_block *,vorbis_look_residue *,
                        float **,int32_t *,int32_t);
} vorbis_func_residue;

typedef struct vorbis_info_residue0{
/* block-partitioned VQ coded straight residue */
  long  begin;
  long  end;

  /* first stage (lossless partitioning) */
  int32_t    grouping;         /* group n vectors per partition */
  int32_t    partitions;       /* possible codebooks for a partition */
  int32_t    partvals;         /* partitions ^ groupbook dim */
  int32_t    groupbook;        /* huffbook for partitioning */
  int32_t    secondstages[64]; /* expanded out to pointers in lookup */
  int32_t    booklist[512];    /* list of second stage books */

  const int32_t classmetric1[64];
  const int32_t classmetric2[64];
} vorbis_info_residue0;

/* Mapping backend generic *****************************************/
typedef struct{
  void                 (*pack)  (vorbis_info *,vorbis_info_mapping *,
                                 oggpack_buffer *);
  vorbis_info_mapping *(*unpack)(vorbis_info *,oggpack_buffer *);
  void (*free_info)    (vorbis_info_mapping *);
  int32_t  (*forward)      (struct vorbis_block *vb);
  int32_t  (*inverse)      (struct vorbis_block *vb,vorbis_info_mapping *);
} vorbis_func_mapping;

typedef struct vorbis_info_mapping0{
  int32_t   submaps;  /* <= 16 */
  int32_t   chmuxlist[256];   /* up to 256 channels in a Vorbis stream */

  int32_t   floorsubmap[16];   /* [mux] submap to floors */
  int32_t   residuesubmap[16]; /* [mux] submap to residue */

  int32_t   coupling_steps;
  int32_t   coupling_mag[256];
  int32_t   coupling_ang[256];

} vorbis_info_mapping0;

END_EXTERN_C

#endif
