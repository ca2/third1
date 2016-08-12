


#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include <libswresample/swresample.h>


#include <math.h>
#include <float.h>


#define INBUF_SIZE 4096
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096


struct _AAC_CONTEXT
{
   BOOL Compressor;

   UINT32 width;
   UINT32 height;

   //H264_RATECONTROL_MODE RateControlMode;
   UINT32 BitRate;
   FLOAT FrameRate;
   UINT32 QP;
   UINT32 NumberOfThreads;

   int iStride[3];
   BYTE* pYUVData[3];

   void* pSystemData;
   //H264_CONTEXT_SUBSYSTEM* subsystem;
};

typedef struct _AAC_CONTEXT AAC_CONTEXT;
struct _AAC_CONTEXT_MF
{
   AVCodec *codec;
   AVCodecContext *c;
   SwrContext* v;
   int got_frame;
   int len;
   FILE *f,*outfile;
   uint8_t inbuf[AUDIO_INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
   AVPacket avpkt;
   AVFrame *decoded_frame;
};

typedef struct _AAC_CONTEXT_MF AAC_CONTEXT_MF;


int mf_aac_init(AAC_CONTEXT* h264,int rate, int channels,AUDIO_FORMAT * in);
int audio_decode_example2(AAC_CONTEXT* h264,void ** pout,const void * inbuf,int sin);
int mf_aac_uninit(AAC_CONTEXT* h264);
