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
    last mod: $Id: dequant.c 16503 2009-08-22 18:14:02Z giles $

 ********************************************************************/
#include "framework.h"



int32_t oc_quant_params_unpack(oc_pack_buf *_opb,th_quant_info *_qinfo){
  th_quant_base *base_mats;
  long           val;
  int32_t            nbase_mats;
  int32_t            sizes[64];
  int32_t            indices[64];
  int32_t            nbits;
  int32_t            bmi;
  int32_t            ci;
  int32_t            qti;
  int32_t            pli;
  int32_t            qri;
  int32_t            qi;
  int32_t            i;
  val=oc_pack_read(_opb,3);
  nbits=(int32_t)val;
  for(qi=0;qi<64;qi++){
    val=oc_pack_read(_opb,nbits);
    _qinfo->loop_filter_limits[qi]=(uchar)val;
  }
  val=oc_pack_read(_opb,4);
  nbits=(int32_t)val+1;
  for(qi=0;qi<64;qi++){
    val=oc_pack_read(_opb,nbits);
    _qinfo->ac_scale[qi]=(uint16_t)val;
  }
  val=oc_pack_read(_opb,4);
  nbits=(int32_t)val+1;
  for(qi=0;qi<64;qi++){
    val=oc_pack_read(_opb,nbits);
    _qinfo->dc_scale[qi]=(uint16_t)val;
  }
  val=oc_pack_read(_opb,9);
  nbase_mats=(int32_t)val+1;
  base_mats= (th_quant_base *) _ogg_malloc(nbase_mats*sizeof(base_mats[0]));
  if(base_mats==NULL)return TH_EFAULT;
  for(bmi=0;bmi<nbase_mats;bmi++){
    for(ci=0;ci<64;ci++){
      val=oc_pack_read(_opb,8);
      base_mats[bmi][ci]=(uchar)val;
    }
  }
  nbits=oc_ilog(nbase_mats-1);
  for(i=0;i<6;i++){
    th_quant_ranges *qranges;
    th_quant_base   *qrbms;
    int32_t             *qrsizes;
    qti=i/3;
    pli=i%3;
    qranges=_qinfo->qi_ranges[qti]+pli;
    if(i>0){
      val=oc_pack_read1(_opb);
      if(!val){
        int32_t qtj;
        int32_t plj;
        if(qti>0){
          val=oc_pack_read1(_opb);
          if(val){
            qtj=qti-1;
            plj=pli;
          }
          else{
            qtj=(i-1)/3;
            plj=(i-1)%3;
          }
        }
        else{
          qtj=(i-1)/3;
          plj=(i-1)%3;
        }
        *qranges=*(_qinfo->qi_ranges[qtj]+plj);
        continue;
      }
    }
    val=oc_pack_read(_opb,nbits);
    indices[0]=(int32_t)val;
    for(qi=qri=0;qi<63;){
      val=oc_pack_read(_opb,oc_ilog(62-qi));
      sizes[qri]=(int32_t)val+1;
      qi+=(int32_t)val+1;
      val=oc_pack_read(_opb,nbits);
      indices[++qri]=(int32_t)val;
    }
    /*Note: The caller is responsible for cleaning up any partially
       constructed qinfo.*/
    if(qi>63){
      _ogg_free(base_mats);
      return TH_EBADHEADER;
    }
    qranges->nranges=qri;
    qranges->sizes=qrsizes=(int32_t *)_ogg_malloc(qri*sizeof(qrsizes[0]));
    if(qranges->sizes==NULL){
      /*Note: The caller is responsible for cleaning up any partially
         constructed qinfo.*/
      _ogg_free(base_mats);
      return TH_EFAULT;
    }
    memcpy(qrsizes,sizes,qri*sizeof(qrsizes[0]));
    qrbms=(th_quant_base *)_ogg_malloc((qri+1)*sizeof(qrbms[0]));
    if(qrbms==NULL){
      /*Note: The caller is responsible for cleaning up any partially
         constructed qinfo.*/
      _ogg_free(base_mats);
      return TH_EFAULT;
    }
    qranges->base_matrices=(const th_quant_base *)qrbms;
    do{
      bmi=indices[qri];
      /*Note: The caller is responsible for cleaning up any partially
         constructed qinfo.*/
      if(bmi>=nbase_mats){
        _ogg_free(base_mats);
        return TH_EBADHEADER;
      }
      memcpy(qrbms[qri],base_mats[bmi],sizeof(qrbms[qri]));
    }
    while(qri-->0);
  }
  _ogg_free(base_mats);
  return 0;
}

void oc_quant_params_clear(th_quant_info *_qinfo){
  int32_t i;
  for(i=6;i-->0;){
    int32_t qti;
    int32_t pli;
    qti=i/3;
    pli=i%3;
    /*Clear any duplicate pointer references.*/
    if(i>0){
      int32_t qtj;
      int32_t plj;
      qtj=(i-1)/3;
      plj=(i-1)%3;
      if(_qinfo->qi_ranges[qti][pli].sizes==
       _qinfo->qi_ranges[qtj][plj].sizes){
        _qinfo->qi_ranges[qti][pli].sizes=NULL;
      }
      if(_qinfo->qi_ranges[qti][pli].base_matrices==
       _qinfo->qi_ranges[qtj][plj].base_matrices){
        _qinfo->qi_ranges[qti][pli].base_matrices=NULL;
      }
    }
    if(qti>0){
      if(_qinfo->qi_ranges[1][pli].sizes==
       _qinfo->qi_ranges[0][pli].sizes){
        _qinfo->qi_ranges[1][pli].sizes=NULL;
      }
      if(_qinfo->qi_ranges[1][pli].base_matrices==
       _qinfo->qi_ranges[0][pli].base_matrices){
        _qinfo->qi_ranges[1][pli].base_matrices=NULL;
      }
    }
    /*Now free all the non-duplicate storage.*/
    _ogg_free((void *)_qinfo->qi_ranges[qti][pli].sizes);
    _ogg_free((void *)_qinfo->qi_ranges[qti][pli].base_matrices);
  }
}
