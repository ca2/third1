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
    last mod: $Id: quant.h 16503 2009-08-22 18:14:02Z giles $

 ********************************************************************/

#if !defined(_quant_H)
# define _quant_H (1)
# include "theora/codec.h"
# include "ocintrin.h"

typedef uint16_t   oc_quant_table[64];


/*Maximum scaled quantizer value.*/
#define OC_QUANT_MAX          (1024<<2)


void oc_dequant_tables_init(uint16_t *_dequant[64][3][2],
 int32_t _pp_dc_scale[64],const th_quant_info *_qinfo);

#endif
