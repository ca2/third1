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
    last mod: $Id: internal.h 16503 2009-08-22 18:14:02Z giles $

 ********************************************************************/
#if !defined(_internal_H)
# define _internal_H (1)
# include <stdlib.h>
# include <limits.h>
# if defined(HAVE_CONFIG_H)
#  include <config.h>
# endif
# include "theora/codec.h"
# include "theora/theora.h"

# if defined(_MSC_VER)
/*Disable missing EMMS warnings.*/
#  pragma warning(disable:4799)
/*Thank you Microsoft, I know the order of operations.*/
#  pragma warning(disable:4554)
# endif
/*You, too, gcc.*/
# if defined(__GNUC_PREREQ)
#  if __GNUC_PREREQ(4,2)
#   pragma GCC diagnostic ignored "-Wparentheses"
#  endif
# endif

# include "ocintrin.h"
# include "huffman.h"
# include "quant.h"

/*Some assembly constructs require aligned operands.*/
# if defined(OC_X86_ASM)
#  if defined(__GNUC__)
#   define OC_ALIGN8(expr) expr __attribute__((aligned(8)))
#   define OC_ALIGN16(expr) expr __attribute__((aligned(16)))
#  elif defined(_MSC_VER)
#   define OC_ALIGN8(expr) __declspec (align(8)) expr
#   define OC_ALIGN16(expr) __declspec (align(16)) expr
#  endif
# endif
# if !defined(OC_ALIGN8)
#  define OC_ALIGN8(expr) expr
# endif
# if !defined(OC_ALIGN16)
#  define OC_ALIGN16(expr) expr
# endif



typedef struct oc_sb_flags              oc_sb_flags;
typedef struct oc_border_info           oc_border_info;
typedef struct oc_fragment              oc_fragment;
typedef struct oc_fragment_plane        oc_fragment_plane;
typedef struct oc_base_opt_vtable       oc_base_opt_vtable;
typedef struct oc_base_opt_data         oc_base_opt_data;
typedef struct oc_state_dispatch_vtable oc_state_dispatch_vtable;
typedef struct oc_theora_state          oc_theora_state;



/*This library's version.*/
# define OC_VENDOR_STRING "Xiph.Org libtheora 1.1 20090822 (Thusnelda)"

/*Theora bitstream version.*/
# define TH_VERSION_MAJOR (3)
# define TH_VERSION_MINOR (2)
# define TH_VERSION_SUB   (1)
# define TH_VERSION_CHECK(_info,_maj,_min,_sub) \
 ((_info)->version_major>(_maj)||(_info)->version_major==(_maj)&& \
 ((_info)->version_minor>(_min)||(_info)->version_minor==(_min)&& \
 (_info)->version_subminor>=(_sub)))

/*A keyframe.*/
#define OC_INTRA_FRAME (0)
/*A predicted frame.*/
#define OC_INTER_FRAME (1)
/*A frame of unknown type (frame type decision has not yet been made).*/
#define OC_UNKWN_FRAME (-1)

/*The amount of padding to add to the reconstructed frame buffers on all
   sides.
  This is used to allow unrestricted motion vectors without special casing.
  This must be a multiple of 2.*/
#define OC_UMV_PADDING (16)

/*Frame classification indices.*/
/*The previous golden frame.*/
#define OC_FRAME_GOLD (0)
/*The previous frame.*/
#define OC_FRAME_PREV (1)
/*The current frame.*/
#define OC_FRAME_SELF (2)

/*The input or output buffer.*/
#define OC_FRAME_IO   (3)

/*Macroblock modes.*/
/*Macro block is invalid: It is never coded.*/
#define OC_MODE_INVALID        (-1)
/*Encoded difference from the same macro block in the previous frame.*/
#define OC_MODE_INTER_NOMV     (0)
/*Encoded with no motion compensated prediction.*/
#define OC_MODE_INTRA          (1)
/*Encoded difference from the previous frame offset by the given motion 
  vector.*/
#define OC_MODE_INTER_MV       (2)
/*Encoded difference from the previous frame offset by the last coded motion 
  vector.*/
#define OC_MODE_INTER_MV_LAST  (3)
/*Encoded difference from the previous frame offset by the second to last 
  coded motion vector.*/
#define OC_MODE_INTER_MV_LAST2 (4)
/*Encoded difference from the same macro block in the previous golden 
  frame.*/
#define OC_MODE_GOLDEN_NOMV    (5)
/*Encoded difference from the previous golden frame offset by the given motion 
  vector.*/
#define OC_MODE_GOLDEN_MV      (6)
/*Encoded difference from the previous frame offset by the individual motion 
  vectors given for each block.*/
#define OC_MODE_INTER_MV_FOUR  (7)
/*The number of (coded) modes.*/
#define OC_NMODES              (8)

/*Determines the reference frame used for a given MB mode.*/
#define OC_FRAME_FOR_MODE(_x) \
 OC_UNIBBLE_TABLE32(OC_FRAME_PREV,OC_FRAME_SELF,OC_FRAME_PREV,OC_FRAME_PREV, \
  OC_FRAME_PREV,OC_FRAME_GOLD,OC_FRAME_GOLD,OC_FRAME_PREV,(_x))

/*Constants for the packet state machine common between encoder and decoder.*/

/*Next packet to emit/read: Codec info header.*/
#define OC_PACKET_INFO_HDR    (-3)
/*Next packet to emit/read: Comment header.*/
#define OC_PACKET_COMMENT_HDR (-2)
/*Next packet to emit/read: Codec setup header.*/
#define OC_PACKET_SETUP_HDR   (-1)
/*No more packets to emit/read.*/
#define OC_PACKET_DONE        (INT_MAX)



/*Super blocks are 32x32 segments of pixels in a single color plane indexed
   in image order.
  Internally, super blocks are broken up into four quadrants, each of which
   contains a 2x2 pattern of blocks, each of which is an 8x8 block of pixels.
  Quadrants, and the blocks within them, are indexed in a special order called
   a "Hilbert curve" within the super block.

  In order to differentiate between the Hilbert-curve indexing strategy and
   the regular image order indexing strategy, blocks indexed in image order
   are called "fragments".
  Fragments are indexed in image order, left to right, then bottom to top,
   from Y' plane to Cb plane to Cr plane.

  The co-located fragments in all image planes corresponding to the location
   of a single quadrant of a luma plane super block form a macro block.
  Thus there is only a single set of macro blocks for all planes, each of which
   contains between 6 and 12 fragments, depending on the pixel format.
  Therefore macro block information is kept in a separate set of arrays from
   super blocks to avoid unused space in the other planes.
  The lists are indexed in super block order.
  That is, the macro block corresponding to the macro block mbi in (luma plane)
   super block sbi is at index (sbi<<2|mbi).
  Thus the number of macro blocks in each dimension is always twice the number
   of super blocks, even when only an odd number fall inside the coded frame.
  These "extra" macro blocks are just an artifact of our internal data layout,
   and not part of the coded stream; they are flagged with a negative MB mode.*/



/*A single quadrant of the map from a super block to fragment numbers.*/
typedef ptrdiff_t       oc_sb_map_quad[4];
/*A map from a super block to fragment numbers.*/
typedef oc_sb_map_quad  oc_sb_map[4];
/*A single plane of the map from a macro block to fragment numbers.*/
typedef ptrdiff_t       oc_mb_map_plane[4];
/*A map from a macro block to fragment numbers.*/
typedef oc_mb_map_plane oc_mb_map[3];
/*A motion vector.*/
typedef signed char     oc_mv[2];



/*Super block information.*/
struct oc_sb_flags{
  uchar coded_fully:1;
  uchar coded_partially:1;
  uchar quad_valid:4;
};



/*Information about a fragment which intersects the border of the displayable
   region.
  This marks which pixels belong to the displayable region.*/
struct oc_border_info{
  /*A bit mask marking which pixels are in the displayable region.
    Pixel (x,y) corresponds to bit (y<<3|x).*/
  int64_t mask;
  /*The number of pixels in the displayable region.
    This is always positive, and always less than 64.*/
  int32_t         npixels;
};



/*Fragment information.*/
struct oc_fragment{
  /*A flag indicating whether or not this fragment is coded.*/
  uint32_t   coded:1;
  /*A flag indicating that this entire fragment lies outside the displayable
     region of the frame.
    Note the contrast with an invalid macro block, which is outside the coded
     frame, not just the displayable one.
    There are no fragments outside the coded frame by construction.*/
  uint32_t   invalid:1;
  /*The index of the quality index used for this fragment's AC coefficients.*/
  uint32_t   qii:6;
  /*The mode of the macroblock this fragment belongs to.*/
  uint32_t   mb_mode:3;
  /*The index of the associated border information for fragments which lie
     partially outside the displayable region.
    For fragments completely inside or outside this region, this is -1.
    Note that the C standard requires an explicit signed keyword for bitfield
     types, since some compilers may treat them as uint32_t without it.*/
  int32_t borderi:5;
  /*The prediction-corrected DC component.
    Note that the C standard requires an explicit signed keyword for bitfield
     types, since some compilers may treat them as uint32_t without it.*/
  int32_t dc:16;
};



/*A description of each fragment plane.*/
struct oc_fragment_plane{
  /*The number of fragments in the horizontal direction.*/
  int32_t       nhfrags;
  /*The number of fragments in the vertical direction.*/
  int32_t       nvfrags;
  /*The offset of the first fragment in the plane.*/
  ptrdiff_t froffset;
  /*The total number of fragments in the plane.*/
  ptrdiff_t nfrags;
  /*The number of super blocks in the horizontal direction.*/
  uint32_t  nhsbs;
  /*The number of super blocks in the vertical direction.*/
  uint32_t  nvsbs;
  /*The offset of the first super block in the plane.*/
  uint32_t  sboffset;
  /*The total number of super blocks in the plane.*/
  uint32_t  nsbs;
};



/*The shared (encoder and decoder) functions that have accelerated variants.*/
struct oc_base_opt_vtable{
  void (*frag_copy)(uchar *_dst,
   const uchar *_src,int32_t _ystride);
  void (*frag_recon_intra)(uchar *_dst,int32_t _ystride,
   const int16_t _residue[64]);
  void (*frag_recon_inter)(uchar *_dst,
   const uchar *_src,int32_t _ystride,const int16_t _residue[64]);
  void (*frag_recon_inter2)(uchar *_dst,const uchar *_src1,
   const uchar *_src2,int32_t _ystride,const int16_t _residue[64]);
  void (*idct8x8)(int16_t _y[64],int32_t _last_zzi);
  void (*state_frag_recon)(const oc_theora_state *_state,ptrdiff_t _fragi,
   int32_t _pli,int16_t _dct_coeffs[64],int32_t _last_zzi,uint16_t _dc_quant);
  void (*state_frag_copy_list)(const oc_theora_state *_state,
   const ptrdiff_t *_fragis,ptrdiff_t _nfragis,
   int32_t _dst_frame,int32_t _src_frame,int32_t _pli);
  void (*state_loop_filter_frag_rows)(const oc_theora_state *_state,
   int32_t _bv[256],int32_t _refi,int32_t _pli,int32_t _fragy0,int32_t _fragy_end);  
  void (*restore_fpu)();
};

/*The shared (encoder and decoder) tables that vary according to which variants
   of the above functions are used.*/
struct oc_base_opt_data{
  const uchar *dct_fzig_zag;
};


/*State information common to both the encoder and decoder.*/
struct oc_theora_state{
  /*The stream information.*/
  th_info             info;
  /*Table for shared accelerated functions.*/
  oc_base_opt_vtable  opt_vtable;
  /*Table for shared data used by accelerated functions.*/
  oc_base_opt_data    opt_data;
  /*CPU flags to detect the presence of extended instruction sets.*/
  uint32_t        cpu_flags;
  /*The fragment plane descriptions.*/
  oc_fragment_plane   fplanes[3];
  /*The list of fragments, indexed in image order.*/
  oc_fragment        *frags;
  /*The the offset into the reference frame buffer to the upper-left pixel of
     each fragment.*/
  ptrdiff_t          *frag_buf_offs;
  /*The motion vector for each fragment.*/
  oc_mv              *frag_mvs;
  /*The total number of fragments in a single frame.*/
  ptrdiff_t           nfrags;
  /*The list of super block maps, indexed in image order.*/
  oc_sb_map          *sb_maps;
  /*The list of super block flags, indexed in image order.*/
  oc_sb_flags        *sb_flags;
  /*The total number of super blocks in a single frame.*/
  uint32_t            nsbs;
  /*The fragments from each color plane that belong to each macro block.
    Fragments are stored in image order (left to right then top to bottom).
    When chroma components are decimated, the extra fragments have an index of
     -1.*/
  oc_mb_map          *mb_maps;
  /*The list of macro block modes.
    A negative number indicates the macro block lies entirely outside the
     coded frame.*/
  signed char        *mb_modes;
  /*The number of macro blocks in the X direction.*/
  uint32_t            nhmbs;
  /*The number of macro blocks in the Y direction.*/
  uint32_t            nvmbs;
  /*The total number of macro blocks.*/
  size_t              nmbs;
  /*The list of coded fragments, in coded order.
    Uncoded fragments are stored in reverse order from the end of the list.*/
  ptrdiff_t          *coded_fragis;
  /*The number of coded fragments in each plane.*/
  ptrdiff_t           ncoded_fragis[3];
  /*The total number of coded fragments.*/
  ptrdiff_t           ntotal_coded_fragis;
  /*The index of the buffers being used for each OC_FRAME_* reference frame.*/
  int32_t                 ref_frame_idx[4];
  /*The actual buffers used for the previously decoded frames.*/
  th_ycbcr_buffer     ref_frame_bufs[4];
  /*The storage for the reference frame buffers.*/
  uchar      *ref_frame_data[4];
  /*The strides for each plane in the reference frames.*/
  int32_t                 ref_ystride[3];
  /*The number of unique border patterns.*/
  int32_t                 nborders;
  /*The unique border patterns for all border fragments.
    The borderi field of fragments which straddle the border indexes this
     list.*/
  oc_border_info      borders[16];
  /*The frame number of the last keyframe.*/
  int64_t         keyframe_num;
  /*The frame number of the current frame.*/
  int64_t         curframe_num;
  /*The granpos of the current frame.*/
  int64_t         granpos;
  /*The type of the current frame.*/
  uchar       frame_type;
  /*The bias to add to the frame count when computing granule positions.*/
  uchar       granpos_bias;
  /*The number of quality indices used in the current frame.*/
  uchar       nqis;
  /*The quality indices of the current frame.*/
  uchar       qis[3];
  /*The dequantization tables, stored in zig-zag order, and indexed by
     qi, pli, qti, and zzi.*/
  uint16_t       *dequant_tables[64][3][2];
  OC_ALIGN16(oc_quant_table      dequant_table_data[64][3][2]);
  /*Loop filter strength parameters.*/
  uchar       loop_filter_limits[64];
};



/*The function type used to fill in the chroma plane motion vectors for a
   macro block when 4 different motion vectors are specified in the luma
   plane.
  _cbmvs: The chroma block-level motion vectors to fill in.
  _lmbmv: The luma macro-block level motion vector to fill in for use in
           prediction.
  _lbmvs: The luma block-level motion vectors.*/
typedef void (*oc_set_chroma_mvs_func)(oc_mv _cbmvs[4],const oc_mv _lbmvs[4]);



/*A map from the index in the zig zag scan to the coefficient number in a
   block.*/
extern const uchar OC_FZIG_ZAG[128];
/*A map from the coefficient number in a block to its index in the zig zag
   scan.*/
extern const uchar OC_IZIG_ZAG[64];
/*A map from physical macro block ordering to bitstream macro block
   ordering within a super block.*/
extern const uchar OC_MB_MAP[2][2];
/*A list of the indices in the oc_mb_map array that can be valid for each of
   the various chroma decimation types.*/
extern const uchar OC_MB_MAP_IDXS[TH_PF_NFORMATS][12];
/*The number of indices in the oc_mb_map array that can be valid for each of
   the various chroma decimation types.*/
extern const uchar OC_MB_MAP_NIDXS[TH_PF_NFORMATS];
/*A table of functions used to fill in the Cb,Cr plane motion vectors for a
   macro block when 4 different motion vectors are specified in the luma
   plane.*/
extern const oc_set_chroma_mvs_func OC_SET_CHROMA_MVS_TABLE[TH_PF_NFORMATS];



int32_t oc_ilog(uint32_t _v);
void **oc_malloc_2d(size_t _height,size_t _width,size_t _sz);
void **oc_calloc_2d(size_t _height,size_t _width,size_t _sz);
void oc_free_2d(void *_ptr);

void oc_ycbcr_buffer_flip(th_ycbcr_buffer _dst,
 const th_ycbcr_buffer _src);

int32_t oc_state_init(oc_theora_state *_state,const th_info *_info,int32_t _nrefs);
void oc_state_clear(oc_theora_state *_state);
void oc_state_vtable_init_c(oc_theora_state *_state);
void oc_state_borders_fill_rows(oc_theora_state *_state,int32_t _refi,int32_t _pli,
 int32_t _y0,int32_t _yend);
void oc_state_borders_fill_caps(oc_theora_state *_state,int32_t _refi,int32_t _pli);
void oc_state_borders_fill(oc_theora_state *_state,int32_t _refi);
void oc_state_fill_buffer_ptrs(oc_theora_state *_state,int32_t _buf_idx,
 th_ycbcr_buffer _img);
int32_t oc_state_mbi_for_pos(oc_theora_state *_state,int32_t _mbx,int32_t _mby);
int32_t oc_state_get_mv_offsets(const oc_theora_state *_state,int32_t _offsets[2],
 int32_t _pli,int32_t _dx,int32_t _dy);

int32_t oc_state_loop_filter_init(oc_theora_state *_state,int32_t *_bv);
void oc_state_loop_filter(oc_theora_state *_state,int32_t _frame);
#if defined(OC_DUMP_IMAGES)
int32_t oc_state_dump_frame(const oc_theora_state *_state,int32_t _frame,
 const char *_suf);
#endif

/*Shared accelerated functions.*/
void oc_frag_copy(const oc_theora_state *_state,uchar *_dst,
 const uchar *_src,int32_t _ystride);
void oc_frag_recon_intra(const oc_theora_state *_state,
 uchar *_dst,int32_t _dst_ystride,const int16_t _residue[64]);
void oc_frag_recon_inter(const oc_theora_state *_state,uchar *_dst,
 const uchar *_src,int32_t _ystride,const int16_t _residue[64]);
void oc_frag_recon_inter2(const oc_theora_state *_state,
 uchar *_dst,const uchar *_src1,const uchar *_src2,
 int32_t _ystride,const int16_t _residue[64]);
void oc_idct8x8(const oc_theora_state *_state,int16_t _y[64],int32_t _last_zzi);
void oc_state_frag_recon(const oc_theora_state *_state,ptrdiff_t _fragi,
 int32_t _pli,int16_t _dct_coeffs[64],int32_t _last_zzi,uint16_t _dc_quant);
void oc_state_frag_copy_list(const oc_theora_state *_state,
 const ptrdiff_t *_fragis,ptrdiff_t _nfragis,
 int32_t _dst_frame,int32_t _src_frame,int32_t _pli);
void oc_state_loop_filter_frag_rows(const oc_theora_state *_state,
 int32_t _bv[256],int32_t _refi,int32_t _pli,int32_t _fragy0,int32_t _fragy_end);
void oc_restore_fpu(const oc_theora_state *_state);

/*Default pure-C implementations.*/
void oc_frag_copy_c(uchar *_dst,
 const uchar *_src,int32_t _src_ystride);
void oc_frag_recon_intra_c(uchar *_dst,int32_t _dst_ystride,
 const int16_t _residue[64]);
void oc_frag_recon_inter_c(uchar *_dst,
 const uchar *_src,int32_t _ystride,const int16_t _residue[64]);
void oc_frag_recon_inter2_c(uchar *_dst,const uchar *_src1,
 const uchar *_src2,int32_t _ystride,const int16_t _residue[64]);
void oc_idct8x8_c(int16_t _y[64],int32_t _last_zzi);
void oc_state_frag_recon_c(const oc_theora_state *_state,ptrdiff_t _fragi,
 int32_t _pli,int16_t _dct_coeffs[64],int32_t _last_zzi,uint16_t _dc_quant);
void oc_state_frag_copy_list_c(const oc_theora_state *_state,
 const ptrdiff_t *_fragis,ptrdiff_t _nfragis,
 int32_t _dst_frame,int32_t _src_frame,int32_t _pli);
void oc_state_loop_filter_frag_rows_c(const oc_theora_state *_state,
 int32_t _bv[256],int32_t _refi,int32_t _pli,int32_t _fragy0,int32_t _fragy_end);
void oc_restore_fpu_c();

/*We need a way to call a few encoder functions without introducing a link-time
   dependency into the decoder, while still allowing the old alpha API which
   does not distinguish between encoder and decoder objects to be used.
  We do this by placing a function table at the start of the encoder object
   which can dispatch into the encoder library.
  We do a similar thing for the decoder in case we ever decide to split off a
   common base library.*/
typedef void (*oc_state_clear_func)(theora_state *_th);
typedef int32_t (*oc_state_control_func)(theora_state *th,int32_t _req,
 void *_buf,size_t _buf_sz);
typedef int64_t (*oc_state_granule_frame_func)(theora_state *_th,
 int64_t _granulepos);
typedef double (*oc_state_granule_time_func)(theora_state *_th,
 int64_t _granulepos);


struct oc_state_dispatch_vtable{
  oc_state_clear_func         clear;
  oc_state_control_func       control;
  oc_state_granule_frame_func granule_frame;
  oc_state_granule_time_func  granule_time;
};

#endif
