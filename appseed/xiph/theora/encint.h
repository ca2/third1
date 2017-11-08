/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggTheora SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE Theora SOURCE CODE IS COPYRIGHT (C) 2002-2009                *
 * by the Xiph.Org Foundation http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************

  function:
  last mod: $Id: encint.h 16503 2009-08-22 18:14:02Z giles $

 ********************************************************************/
#if !defined(_encint_H)
# define _encint_H (1)
# if defined(HAVE_CONFIG_H)
#  include "config.h"
# endif
# include "theora/theoraenc.h"
# include "internal.h"
# include "ocintrin.h"
# include "mathops.h"
# include "enquant.h"
# include "huffenc.h"
/*# define OC_COLLECT_METRICS*/



typedef oc_mv                         oc_mv2[2];

typedef struct oc_enc_opt_vtable      oc_enc_opt_vtable;
typedef struct oc_mb_enc_info         oc_mb_enc_info;
typedef struct oc_mode_scheme_chooser oc_mode_scheme_chooser;
typedef struct oc_iir_filter          oc_iir_filter;
typedef struct oc_frame_metrics       oc_frame_metrics;
typedef struct oc_rc_state            oc_rc_state;
typedef struct th_enc_ctx             oc_enc_ctx;
typedef struct oc_token_checkpoint    oc_token_checkpoint;



/*Constants for the packet-out state machine specific to the encoder.*/

/*Next packet to emit: Data packet, but none are ready yet.*/
#define OC_PACKET_EMPTY (0)
/*Next packet to emit: Data packet, and one is ready.*/
#define OC_PACKET_READY (1)

/*All features enabled.*/
#define OC_SP_LEVEL_SLOW       (0)
/*Enable early skip.*/
#define OC_SP_LEVEL_EARLY_SKIP (1)
/*Disable motion compensation.*/
#define OC_SP_LEVEL_NOMC       (2)
/*Maximum valid speed level.*/
#define OC_SP_LEVEL_MAX        (2)


/*The bits used for each of the MB mode codebooks.*/
extern const uchar OC_MODE_BITS[2][OC_NMODES];

/*The bits used for each of the MV codebooks.*/
extern const uchar OC_MV_BITS[2][64];

/*The minimum value that can be stored in a SB run for each codeword.
  The last entry is the upper bound on the length of a single SB run.*/
extern const uint16_t  OC_SB_RUN_VAL_MIN[8];
/*The bits used for each SB run codeword.*/
extern const uchar OC_SB_RUN_CODE_NBITS[7];

/*The bits used for each block run length (starting with 1).*/
extern const uchar OC_BLOCK_RUN_CODE_NBITS[30];



/*Encoder specific functions with accelerated variants.*/
struct oc_enc_opt_vtable{
  uint32_t (*frag_sad)(const uchar *_src,
   const uchar *_ref,int32_t _ystride);
  uint32_t (*frag_sad_thresh)(const uchar *_src,
   const uchar *_ref,int32_t _ystride,uint32_t _thresh);
  uint32_t (*frag_sad2_thresh)(const uchar *_src,
   const uchar *_ref1,const uchar *_ref2,int32_t _ystride,
   uint32_t _thresh);
  uint32_t (*frag_satd_thresh)(const uchar *_src,
   const uchar *_ref,int32_t _ystride,uint32_t _thresh);
  uint32_t (*frag_satd2_thresh)(const uchar *_src,
   const uchar *_ref1,const uchar *_ref2,int32_t _ystride,
   uint32_t _thresh);
  uint32_t (*frag_intra_satd)(const uchar *_src,int32_t _ystride);
  void     (*frag_sub)(int16_t _diff[64],const uchar *_src,
   const uchar *_ref,int32_t _ystride);
  void     (*frag_sub_128)(int16_t _diff[64],
   const uchar *_src,int32_t _ystride);
  void     (*frag_copy2)(uchar *_dst,
   const uchar *_src1,const uchar *_src2,int32_t _ystride);
  void     (*frag_recon_intra)(uchar *_dst,int32_t _ystride,
   const int16_t _residue[64]);
  void     (*frag_recon_inter)(uchar *_dst,
   const uchar *_src,int32_t _ystride,const int16_t _residue[64]);
  void     (*fdct8x8)(int16_t _y[64],const int16_t _x[64]);
};


void oc_enc_vtable_init(oc_enc_ctx *_enc);



/*Encoder-specific macroblock information.*/
struct oc_mb_enc_info{
  /*Neighboring macro blocks that have MVs available from the current frame.*/
  uint32_t      cneighbors[4];
  /*Neighboring macro blocks to use for MVs from the previous frame.*/
  uint32_t      pneighbors[4];
  /*The number of current-frame neighbors.*/
  uchar ncneighbors;
  /*The number of previous-frame neighbors.*/
  uchar npneighbors;
  /*Flags indicating which MB modes have been refined.*/
  uchar refined;
  /*Motion vectors for a macro block for the current frame and the
     previous two frames.
    Each is a set of 2 vectors against OC_FRAME_GOLD and OC_FRAME_PREV, which
     can be used to estimate constant velocity and constant acceleration
     predictors.
    Uninitialized MVs are (0,0).*/
  oc_mv2        analysis_mv[3];
  /*Current unrefined analysis MVs.*/
  oc_mv         unref_mv[2];
  /*Unrefined block MVs.*/
  oc_mv         block_mv[4];
  /*Refined block MVs.*/
  oc_mv         ref_mv[4];
  /*Minimum motion estimation error from the analysis stage.*/
  uint16_t  error[2];
  /*MB error for half-pel refinement for each frame type.*/
  uint32_t      satd[2];
  /*Block error for half-pel refinement.*/
  uint32_t      block_satd[4];
};



/*State machine to estimate the opportunity cost of coding a MB mode.*/
struct oc_mode_scheme_chooser{
  /*Pointers to the a list containing the index of each mode in the mode
     alphabet used by each scheme.
    The first entry points to the dynamic scheme0_ranks, while the remaining 7
     point to the constant entries stored in OC_MODE_SCHEMES.*/
  const uchar *mode_ranks[8];
  /*The ranks for each mode when coded with scheme 0.
    These are optimized so that the more frequent modes have lower ranks.*/
  uchar        scheme0_ranks[OC_NMODES];
  /*The list of modes, sorted in descending order of frequency, that
    corresponds to the ranks above.*/
  uchar        scheme0_list[OC_NMODES];
  /*The number of times each mode has been chosen so far.*/
  int32_t                  mode_counts[OC_NMODES];
  /*The list of mode coding schemes, sorted in ascending order of bit cost.*/
  uchar        scheme_list[8];
  /*The number of bits used by each mode coding scheme.*/
  ptrdiff_t            scheme_bits[8];
};


void oc_mode_scheme_chooser_init(oc_mode_scheme_chooser *_chooser);



/*A 2nd order low-pass Bessel follower.
  We use this for rate control because it has fast reaction time, but is
   critically damped.*/
struct oc_iir_filter{
  int32_t c[2];
  int64_t g;
  int32_t x[2];
  int32_t y[2];
};



/*The 2-pass metrics associated with a single frame.*/
struct oc_frame_metrics{
  /*The log base 2 of the scale factor for this frame in Q24 format.*/
  int32_t   log_scale;
  /*The number of application-requested duplicates of this frame.*/
  uint32_t      dup_count:31;
  /*The frame type from pass 1.*/
  uint32_t      frame_type:1;
};



/*Rate control state information.*/
struct oc_rc_state{
  /*The target average bits per frame.*/
  int64_t        bits_per_frame;
  /*The current buffer fullness (bits available to be used).*/
  int64_t        fullness;
  /*The target buffer fullness.
    This is where we'd like to be by the last keyframe the appears in the next
     buf_delay frames.*/
  int64_t        target;
  /*The maximum buffer fullness (total size of the buffer).*/
  int64_t        max;
  /*The log of the number of pixels in a frame in Q57 format.*/
  int64_t        log_npixels;
  /*The exponent used in the rate model in Q8 format.*/
  uint32_t           exp[2];
  /*The number of frames to distribute the buffer usage over.*/
  int32_t                buf_delay;
  /*The total drop count from the previous frame.
    This includes duplicates explicitly requested via the
     TH_ENCCTL_SET_DUP_COUNT API as well as frames we chose to drop ourselves.*/
  uint32_t       prev_drop_count;
  /*The log of an estimated scale factor used to obtain the real framerate, for
     VFR sources or, e.g., 12 fps content doubled to 24 fps, etc.*/
  int64_t        log_drop_scale;
  /*The log of estimated scale factor for the rate model in Q57 format.*/
  int64_t        log_scale[2];
  /*The log of the target quantizer level in Q57 format.*/
  int64_t        log_qtarget;
  /*Will we drop frames to meet bitrate target?*/
  uchar      drop_frames;
  /*Do we respect the maximum buffer fullness?*/
  uchar      cap_overflow;
  /*Can the reservoir go negative?*/
  uchar      cap_underflow;
  /*Second-order lowpass filters to track scale and VFR.*/
  oc_iir_filter      scalefilter[2];
  int32_t                inter_count;
  int32_t                inter_delay;
  int32_t                inter_delay_target;
  oc_iir_filter      vfrfilter;
  /*Two-pass mode state.
    0 => 1-pass encoding.
    1 => 1st pass of 2-pass encoding.
    2 => 2nd pass of 2-pass encoding.*/
  int32_t                twopass;
  /*Buffer for current frame metrics.*/
  uchar      twopass_buffer[48];
  /*The number of bytes in the frame metrics buffer.
    When 2-pass encoding is enabled, this is set to 0 after each frame is
     submitted, and must be non-zero before the next frame will be accepted.*/
  int32_t                twopass_buffer_bytes;
  int32_t                twopass_buffer_fill;
  /*Whether or not to force the next frame to be a keyframe.*/
  uchar      twopass_force_kf;
  /*The metrics for the previous frame.*/
  oc_frame_metrics   prev_metrics;
  /*The metrics for the current frame.*/
  oc_frame_metrics   cur_metrics;
  /*The buffered metrics for future frames.*/
  oc_frame_metrics  *frame_metrics;
  int32_t                nframe_metrics;
  int32_t                cframe_metrics;
  /*The index of the current frame in the circular metric buffer.*/
  int32_t                frame_metrics_head;
  /*The frame count of each type (keyframes, delta frames, and dup frames);
     32 bits limits us to 2.268 years at 60 fps.*/
  uint32_t       frames_total[3];
  /*The number of frames of each type yet to be processed.*/
  uint32_t       frames_left[3];
  /*The sum of the scale values for each frame type.*/
  int64_t        scale_sum[2];
  /*The start of the window over which the current scale sums are taken.*/
  int32_t                scale_window0;
  /*The end of the window over which the current scale sums are taken.*/
  int32_t                scale_window_end;
  /*The frame count of each type in the current 2-pass window; this does not
     include dup frames.*/
  int32_t                nframes[3];
  /*The total accumulated estimation bias.*/
  int64_t        rate_bias;
};


void oc_rc_state_init(oc_rc_state *_rc,oc_enc_ctx *_enc);
void oc_rc_state_clear(oc_rc_state *_rc);

void oc_enc_rc_resize(oc_enc_ctx *_enc);
int32_t oc_enc_select_qi(oc_enc_ctx *_enc,int32_t _qti,int32_t _clamp);
void oc_enc_calc_lambda(oc_enc_ctx *_enc,int32_t _frame_type);
int32_t oc_enc_update_rc_state(oc_enc_ctx *_enc,
 long _bits,int32_t _qti,int32_t _qi,int32_t _trial,int32_t _droppable);
int32_t oc_enc_rc_2pass_out(oc_enc_ctx *_enc,uchar **_buf);
int32_t oc_enc_rc_2pass_in(oc_enc_ctx *_enc,uchar *_buf,size_t _bytes);



/*The internal encoder state.*/
struct th_enc_ctx{
  /*Shared encoder/decoder state.*/
  oc_theora_state          state;
  /*Buffer in which to assemble packets.*/
  oggpack_buffer           opb;
  /*Encoder-specific macroblock information.*/
  oc_mb_enc_info          *mb_info;
  /*DC coefficients after prediction.*/
  int16_t             *frag_dc;
  /*The list of coded macro blocks, in coded order.*/
  uint32_t                *coded_mbis;
  /*The number of coded macro blocks.*/
  size_t                   ncoded_mbis;
  /*Whether or not packets are ready to be emitted.
    This takes on negative values while there are remaining header packets to
     be emitted, reaches 0 when the codec is ready for input, and becomes
     positive when a frame has been processed and data packets are ready.*/
  int32_t                      packet_state;
  /*The maximum distance between keyframes.*/
  uint32_t             keyframe_frequency_force;
  /*The number of duplicates to produce for the next frame.*/
  uint32_t             dup_count;
  /*The number of duplicates remaining to be emitted for the current frame.*/
  uint32_t             nqueued_dups;
  /*The number of duplicates emitted for the last frame.*/
  uint32_t             prev_dup_count;
  /*The current speed level.*/
  int32_t                      sp_level;
  /*Whether or not VP3 compatibility mode has been enabled.*/
  uchar            vp3_compatible;
  /*Whether or not any INTER frames have been coded.*/
  uchar            coded_inter_frame;
  /*Whether or not previous frame was dropped.*/
  uchar            prevframe_dropped;
  /*Stores most recently chosen Huffman tables for each frame type, DC and AC
     coefficients, and luma and chroma tokens.
    The actual Huffman table used for a given coefficient depends not only on
     the choice made here, but also its index in the zig-zag ordering.*/
  uchar            huff_idxs[2][2][2];
  /*Current count of bits used by each MV coding mode.*/
  size_t                   mv_bits[2];
  /*The mode scheme chooser for estimating mode coding costs.*/
  oc_mode_scheme_chooser   chooser;
  /*The number of vertical super blocks in an MCU.*/
  int32_t                      mcu_nvsbs;
  /*The SSD error for skipping each fragment in the current MCU.*/
  uint32_t                *mcu_skip_ssd;
  /*The DCT token lists for each coefficient and each plane.*/
  uchar          **dct_tokens[3];
  /*The extra bits associated with each DCT token.*/
  uint16_t           **extra_bits[3];
  /*The number of DCT tokens for each coefficient for each plane.*/
  ptrdiff_t                ndct_tokens[3][64];
  /*Pending EOB runs for each coefficient for each plane.*/
  uint16_t             eob_run[3][64];
  /*The offset of the first DCT token for each coefficient for each plane.*/
  uchar            dct_token_offs[3][64];
  /*The last DC coefficient for each plane and reference frame.*/
  int32_t                      dc_pred_last[3][3];
#if defined(OC_COLLECT_METRICS)
  /*Fragment SATD statistics for MB mode estimation metrics.*/
  uint32_t                *frag_satd;
  /*Fragment SSD statistics for MB mode estimation metrics.*/
  uint32_t                *frag_ssd;
#endif
  /*The R-D optimization parameter.*/
  int32_t                      lambda;
  /*The huffman tables in use.*/
  th_huff_code             huff_codes[TH_NHUFFMAN_TABLES][TH_NDCT_TOKENS];
  /*The quantization parameters in use.*/
  th_quant_info            qinfo;
  oc_iquant               *enquant_tables[64][3][2];
  oc_iquant_table          enquant_table_data[64][3][2];
  /*An "average" quantizer for each quantizer type (INTRA or INTER) and qi
     value.
    This is used to paramterize the rate control decisions.
    They are kept in the log domain to simplify later processing.
    Keep in mind these are DCT domain quantizers, and so are scaled by an
     additional factor of 4 from the pixel domain.*/
  int64_t              log_qavg[2][64];
  /*The buffer state used to drive rate control.*/
  oc_rc_state              rc;
  /*Table for encoder acceleration functions.*/
  oc_enc_opt_vtable        opt_vtable;
};


void oc_enc_analyze_intra(oc_enc_ctx *_enc,int32_t _recode);
int32_t oc_enc_analyze_inter(oc_enc_ctx *_enc,int32_t _allow_keyframe,int32_t _recode);
#if defined(OC_COLLECT_METRICS)
void oc_enc_mode_metrics_collect(oc_enc_ctx *_enc);
void oc_enc_mode_metrics_dump(oc_enc_ctx *_enc);
#endif



/*Perform fullpel motion search for a single MB against both reference frames.*/
void oc_mcenc_search(oc_enc_ctx *_enc,int32_t _mbi);
/*Refine a MB MV for one frame.*/
void oc_mcenc_refine1mv(oc_enc_ctx *_enc,int32_t _mbi,int32_t _frame);
/*Refine the block MVs.*/
void oc_mcenc_refine4mv(oc_enc_ctx *_enc,int32_t _mbi);



/*Used to rollback a tokenlog transaction when we retroactively decide to skip
   a fragment.
  A checkpoint is taken right before each token is added.*/
struct oc_token_checkpoint{
  /*The color plane the token was added to.*/
  uchar pli;
  /*The zig-zag index the token was added to.*/
  uchar zzi;
  /*The outstanding EOB run count before the token was added.*/
  uint16_t  eob_run;
  /*The token count before the token was added.*/
  ptrdiff_t     ndct_tokens;
};



void oc_enc_tokenize_start(oc_enc_ctx *_enc);
int32_t oc_enc_tokenize_ac(oc_enc_ctx *_enc,int32_t _pli,ptrdiff_t _fragi,
 int16_t *_qdct,const uint16_t *_dequant,const int16_t *_dct,
 int32_t _zzi,oc_token_checkpoint **_stack,int32_t _acmin);
void oc_enc_tokenlog_rollback(oc_enc_ctx *_enc,
 const oc_token_checkpoint *_stack,int32_t _n);
void oc_enc_pred_dc_frag_rows(oc_enc_ctx *_enc,
 int32_t _pli,int32_t _fragy0,int32_t _frag_yend);
void oc_enc_tokenize_dc_frag_list(oc_enc_ctx *_enc,int32_t _pli,
 const ptrdiff_t *_coded_fragis,ptrdiff_t _ncoded_fragis,
 int32_t _prev_ndct_tokens1,int32_t _prev_eob_run1);
void oc_enc_tokenize_finish(oc_enc_ctx *_enc);



/*Utility routine to encode one of the header packets.*/
int32_t oc_state_flushheader(oc_theora_state *_state,int32_t *_packet_state,
 oggpack_buffer *_opb,const th_quant_info *_qinfo,
 const th_huff_code _codes[TH_NHUFFMAN_TABLES][TH_NDCT_TOKENS],
 const char *_vendor,th_comment *_tc,ogg_packet *_op);



/*Encoder-specific accelerated functions.*/
void oc_enc_frag_sub(const oc_enc_ctx *_enc,int16_t _diff[64],
 const uchar *_src,const uchar *_ref,int32_t _ystride);
void oc_enc_frag_sub_128(const oc_enc_ctx *_enc,int16_t _diff[64],
 const uchar *_src,int32_t _ystride);
uint32_t oc_enc_frag_sad(const oc_enc_ctx *_enc,const uchar *_src,
 const uchar *_ref,int32_t _ystride);
uint32_t oc_enc_frag_sad_thresh(const oc_enc_ctx *_enc,
 const uchar *_src,const uchar *_ref,int32_t _ystride,
 uint32_t _thresh);
uint32_t oc_enc_frag_sad2_thresh(const oc_enc_ctx *_enc,
 const uchar *_src,const uchar *_ref1,
 const uchar *_ref2,int32_t _ystride,uint32_t _thresh);
uint32_t oc_enc_frag_satd_thresh(const oc_enc_ctx *_enc,
 const uchar *_src,const uchar *_ref,int32_t _ystride,
 uint32_t _thresh);
uint32_t oc_enc_frag_satd2_thresh(const oc_enc_ctx *_enc,
 const uchar *_src,const uchar *_ref1,
 const uchar *_ref2,int32_t _ystride,uint32_t _thresh);
uint32_t oc_enc_frag_intra_satd(const oc_enc_ctx *_enc,
 const uchar *_src,int32_t _ystride);
void oc_enc_frag_copy2(const oc_enc_ctx *_enc,uchar *_dst,
 const uchar *_src1,const uchar *_src2,int32_t _ystride);
void oc_enc_frag_recon_intra(const oc_enc_ctx *_enc,
 uchar *_dst,int32_t _ystride,const int16_t _residue[64]);
void oc_enc_frag_recon_inter(const oc_enc_ctx *_enc,uchar *_dst,
 const uchar *_src,int32_t _ystride,const int16_t _residue[64]);
void oc_enc_fdct8x8(const oc_enc_ctx *_enc,int16_t _y[64],
 const int16_t _x[64]);

/*Default pure-C implementations.*/
void oc_enc_vtable_init_c(oc_enc_ctx *_enc);

void oc_enc_frag_sub_c(int16_t _diff[64],
 const uchar *_src,const uchar *_ref,int32_t _ystride);
void oc_enc_frag_sub_128_c(int16_t _diff[64],
 const uchar *_src,int32_t _ystride);
void oc_enc_frag_copy2_c(uchar *_dst,
 const uchar *_src1,const uchar *_src2,int32_t _ystride);
uint32_t oc_enc_frag_sad_c(const uchar *_src,
 const uchar *_ref,int32_t _ystride);
uint32_t oc_enc_frag_sad_thresh_c(const uchar *_src,
 const uchar *_ref,int32_t _ystride,uint32_t _thresh);
uint32_t oc_enc_frag_sad2_thresh_c(const uchar *_src,
 const uchar *_ref1,const uchar *_ref2,int32_t _ystride,
 uint32_t _thresh);
uint32_t oc_enc_frag_satd_thresh_c(const uchar *_src,
 const uchar *_ref,int32_t _ystride,uint32_t _thresh);
uint32_t oc_enc_frag_satd2_thresh_c(const uchar *_src,
 const uchar *_ref1,const uchar *_ref2,int32_t _ystride,
 uint32_t _thresh);
uint32_t oc_enc_frag_intra_satd_c(const uchar *_src,int32_t _ystride);
void oc_enc_fdct8x8_c(int16_t _y[64],const int16_t _x[64]);

#endif
