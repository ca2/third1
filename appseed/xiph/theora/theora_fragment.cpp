/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggTheora SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE Theora SOURCE CODE IS COPYRIGHT (C) 2002-2009                *
 * by the Xiph.Org Foundation and contributors http://www.xiph.org/ *
 *                                                                  *
 ********************************************************************

  function:
    last mod: $Id: fragment.c 16503 2009-08-22 18:14:02Z giles $

 ********************************************************************/
#include "framework.h"



void oc_frag_copy(const oc_theora_state *_state,uchar *_dst,
 const uchar *_src,int32_t _ystride){
  (*_state->opt_vtable.frag_copy)(_dst,_src,_ystride);
}

void oc_frag_copy_c(uchar *_dst,const uchar *_src,int32_t _ystride){
  int32_t i;
  for(i=8;i-->0;){
    memcpy(_dst,_src,8*sizeof(*_dst));
    _dst+=_ystride;
    _src+=_ystride;
  }
}

void oc_frag_recon_intra(const oc_theora_state *_state,uchar *_dst,
 int32_t _ystride,const int16_t _residue[64]){
  _state->opt_vtable.frag_recon_intra(_dst,_ystride,_residue);
}

void oc_frag_recon_intra_c(uchar *_dst,int32_t _ystride,
 const int16_t _residue[64]){
  int32_t i;
  for(i=0;i<8;i++){
    int32_t j;
    for(j=0;j<8;j++)_dst[j]=OC_CLAMP255(_residue[i*8+j]+128);
    _dst+=_ystride;
  }
}

void oc_frag_recon_inter(const oc_theora_state *_state,uchar *_dst,
 const uchar *_src,int32_t _ystride,const int16_t _residue[64]){
  _state->opt_vtable.frag_recon_inter(_dst,_src,_ystride,_residue);
}

void oc_frag_recon_inter_c(uchar *_dst,
 const uchar *_src,int32_t _ystride,const int16_t _residue[64]){
  int32_t i;
  for(i=0;i<8;i++){
    int32_t j;
    for(j=0;j<8;j++)_dst[j]=OC_CLAMP255(_residue[i*8+j]+_src[j]);
    _dst+=_ystride;
    _src+=_ystride;
  }
}

void oc_frag_recon_inter2(const oc_theora_state *_state,uchar *_dst,
 const uchar *_src1,const uchar *_src2,int32_t _ystride,
 const int16_t _residue[64]){
  _state->opt_vtable.frag_recon_inter2(_dst,_src1,_src2,_ystride,_residue);
}

void oc_frag_recon_inter2_c(uchar *_dst,const uchar *_src1,
 const uchar *_src2,int32_t _ystride,const int16_t _residue[64]){
  int32_t i;
  for(i=0;i<8;i++){
    int32_t j;
    for(j=0;j<8;j++)_dst[j]=OC_CLAMP255(_residue[i*8+j]+(_src1[j]+_src2[j]>>1));
    _dst+=_ystride;
    _src1+=_ystride;
    _src2+=_ystride;
  }
}

void oc_restore_fpu(const oc_theora_state *_state){
  _state->opt_vtable.restore_fpu();
}

void oc_restore_fpu_c(){}
