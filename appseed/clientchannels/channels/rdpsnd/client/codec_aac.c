#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <winpr/crt.h>
#include <winpr/cmdline.h>

#include <freerdp/types.h>
#include <freerdp/codec/dsp.h>
#include <freerdp/channels/log.h>

#include "rdpsnd_main.h"
#include "codec_aac.h"

void o_debug_string(const char * psz);
#if defined(ANDROID) || defined(__APPLE__)
extern AVCodec ff_aac_decoder;
#endif
#include <assert.h>

void assert_a(int i)
{
   assert(i);
}
void assert_b(int i)
{
   assert(i);
}
void assert_c(int i)
{
   assert(i);
}
void assert_d(int i)
{
   assert(i);
}
void assert_e(int i)
{
   assert(i);
}
void assert_f(int i)
{
   assert(i);
}
void assert_g(int i)
{
   assert(i);
}
void assert_h(int i)
{
   assert(i);
}
void assert_i(int i)
{
   assert(i);
}
void assert_j(int i)
{
   assert(i);
}
void assert_k(int i)
{
   assert(i);
}
void assert_l(int i)
{
   assert(i);
}


int mf_aac_init(AAC_CONTEXT* h264,int rate,int channels,AUDIO_FORMAT * in)
{




   avcodec_register_all();
//   HRESULT hr;
   AAC_CONTEXT_MF* sys;

   sys = (AAC_CONTEXT_MF*)calloc(1,sizeof(AAC_CONTEXT_MF));

   if(!sys)
   {
      assert_a(0);
      goto error;
   }

   h264->pSystemData = (void*)sys;
   sys->c= NULL;
   sys->decoded_frame = NULL;


   //printf("Decode audio file %s to %s\n",filename,outfilename);

   /* find the mpeg audio decoder */
#if defined(__APPLE__)
   sys->codec = avcodec_find_decoder(AV_CODEC_ID_AAC);
//#elif defined(ANDROID)
  // sys->codec = &ff_aac_decoder;
#else
   sys->codec = avcodec_find_decoder(AV_CODEC_ID_AAC);
#endif
   if(!sys->codec)
   {
      assert_b(0);
      fprintf(stderr,"Codec not found\n");
      return FALSE;
   }

   sys->c = avcodec_alloc_context3(sys->codec);
   if(!sys->c) {
      assert_c(0);
      fprintf(stderr,"Could not allocate audio codec context\n");
      return FALSE;
   }


   sys->c->sample_rate = rate;
   sys->c->channels = channels;
   sys->c->channel_layout = channels == 1 ? AV_CH_LAYOUT_MONO : AV_CH_LAYOUT_STEREO;
   //sys->c->sample_fmt = AV_SAMPLE_FMT_S16;
   char string[200];
   sprintf(string,"rate = %d, channels = %d",rate,channels);
   //o_debug_string(string);
   /* open it */
   if(avcodec_open2(sys->c,sys->codec,NULL) < 0) {
      assert_d(0);
      fprintf(stderr,"Could not open codec\n");
      return FALSE;
   }
   av_init_packet(&sys->avpkt);
   //sys->v = swr_alloc_set_opts(NULL,
   //   channels == 1 ? AV_CH_LAYOUT_MONO : AV_CH_LAYOUT_STEREO,
   //   AV_SAMPLE_FMT_S16,
   //   rate,
   //   sys->c->channel_layout,
   //   sys->c->sample_fmt,
   //   sys->c->sample_rate,
   //   0,
   //   NULL
   //   );
   //,sys->c->sample_fmt,sys->c,NULL,0
   //Create an audio sample format converter context. More...

   //void 	swri_audio_convert_free(AudioConvert **ctx)
   //Free audio sample format converter context. More...

   //      Convert between audio sample formats. More...
   //f = fopen(filename,"rb");
   //if(!f) {
   //   fprintf(stderr,"Could not open %s\n",filename);
   //   exit(1);
   //}
   //outfile = fopen(outfilename,"wb");
   //if(!outfile) {
   //   av_free(c);
   //   exit(1);
   //}

   /* decode until eof */
   //avpkt.data = inbuf;
   //avpkt.size = fread(inbuf,1,AUDIO_INBUF_SIZE,f);

   //while(avpkt.size > 0) {
   //   int got_frame = 0;

   //   if(!decoded_frame) {
   //      if(!(decoded_frame = av_frame_alloc())) {
   //         fprintf(stderr,"Could not allocate audio frame\n");
   //         exit(1);
   //      }
   //   }
   //   else
   //      av_frame_unref(decoded_frame);

   //   len = avcodec_decode_audio4(c,decoded_frame,&got_frame,&avpkt);
   //   if(len < 0) {
   //      fprintf(stderr,"Error while decoding\n");
   //      exit(1);
   //   }
   //   if(got_frame) {
   //      /* if a frame has been decoded, output it */
   //      int data_size = av_samples_get_buffer_size(NULL,c->channels,
   //         decoded_frame->nb_samples,
   //         c->sample_fmt,1);
   //      fwrite(decoded_frame->data[0],1,data_size,outfile);
   //   }
   //   avpkt.size -= len;
   //   avpkt.data += len;
   //   avpkt.dts =
   //      avpkt.pts = AV_NOPTS_VALUE;
   //   if(avpkt.size < AUDIO_REFILL_THRESH) {
   //      /* Refill the input buffer, to avoid trying to decode
   //      * incomplete frames. Instead of this, one could also use
   //      * a parser, or use a proper container format through
   //      * libavformat. */
   //      memmove(inbuf,avpkt.data,avpkt.size);
   //      avpkt.data = inbuf;
   //      len = fread(avpkt.data + avpkt.size,1,
   //         AUDIO_INBUF_SIZE - avpkt.size,f);
   //      if(len > 0)
   //         avpkt.size += len;
   //   }
   //}
   return 1;
error:
   return 0;
}

int audio_decode_example2(AAC_CONTEXT* h264,void ** pout,const void * inbuf,int sin)
{
   *pout = NULL;
   int data_size = 0;
   AAC_CONTEXT_MF* sys = h264->pSystemData;
   /* decode until eof */

   if(!sys->decoded_frame)
   {
      if(!(sys->decoded_frame = av_frame_alloc()))
      {
         assert_e(0);
         fprintf(stderr,"Could not allocate audio frame\n");
         return 0;
      }
   }
   else
   {
      av_frame_unref(sys->decoded_frame);
   }
   char * mem = av_malloc(sin);

   memcpy(mem, inbuf, sin);
   int olen = 0;
   sys->avpkt.data = mem;
   sys->avpkt.size = sin;
   int i;

#ifdef LINUX

   sys->len = avcodec_decode_audio4(sys->c,sys->decoded_frame,&sys->got_frame,&sys->avpkt);

   // sys->len = avcodec_decode_audio4(sys->c, sys->decoded_frame, &sys->got_frame, &sys->avpkt);
#else

   int iRet1 = avcodec_send_packet(sys->c, &sys->avpkt);

   sys->len = 0;

   sys->got_frame = 0;

   int iRet2 = -1;

   if (iRet1 >= 0)
   {

      avcodec_receive_frame(sys->c, sys->decoded_frame);

      sys->len = sys->decoded_frame->linesize;

      if (sys->len > 0)
      {

         sys->got_frame = 1;

      }

   }
#endif // LINUX


   av_free(mem);
   if(sys->len < 0)
   {
      char sz[2048];
      av_strerror(sys->len, sz, sizeof(sz));
//      assert_f(0);
      fprintf(stderr,"Error while decoding\n");
      fprintf(stderr,sz);
      fprintf(stderr,"\n");
      return 0;
   }
   if(sys->got_frame)
   {
      /* if a frame has been decoded, output it */
      data_size = av_samples_get_buffer_size(NULL,sys->c->channels,sys->decoded_frame->nb_samples,
         AV_SAMPLE_FMT_S16,1);
      int dst_linesize = 0;
      //int ret = av_samples_alloc_array_and_samples(pout,&dst_linesize,sys->c->channels,
      // sys->decoded_frame->nb_samples * 2,AV_SAMPLE_FMT_S16,0);

#ifdef METROWIN
      *pout = _aligned_malloc(data_size * 2, 4096);
#else
      *pout = malloc(data_size * 2);
#endif
      //int b = ret == EAGAIN;
      if(*pout != NULL)
      {

         short * psh = *pout;
         float * f1 = (float *) sys->decoded_frame->data[0];
         float * f2 = (float *) sys->decoded_frame->data[0];
         int sample;
         for(i = 0; i < sys->decoded_frame->nb_samples; i++)
         {

            sample = (int)(*f1++ * 32768.f);

            if(sample > 32767) sample = 32767;
            if(sample < -32768) sample = -32768;

            *psh++ = (short)sample;

            sample = (int)(*f2++ * 32768.f);

            if(sample > 32767) sample = 32767;
            if(sample < -32768) sample = -32768;

            *psh++ = (short)sample;

         }
         olen =data_size;
         //olen = swr_convert(sys->v,*pout,sys->decoded_frame->nb_samples * 2,sys->decoded_frame->data,sys->decoded_frame->nb_samples);

         /*if(olen < 0)
         {
         olen = 0;
         }
         else
         {
         olen *= 2 * 2;
         }*/
      }
      else
      {
         assert_g(0);
      }
      //memcpy(*pout,,data_size);

      //fwrite(sys->decoded_frame->data[0],1,data_size,outfile);
   }
   sys->avpkt.size -= sys->len;
   sys->avpkt.data += sys->len;
   sys->avpkt.dts = sys->avpkt.pts = AV_NOPTS_VALUE;
   //if(avpkt.size < AUDIO_REFILL_THRESH) {
   //   /* Refill the input buffer, to avoid trying to decode
   //   * incomplete frames. Instead of this, one could also use
   //   * a parser, or use a proper container format through
   //   * libavformat. */
   //   memmove(inbuf,avpkt.data,avpkt.size);
   //   avpkt.data = inbuf;
   //   len = fread(avpkt.data + avpkt.size,1,
   //      AUDIO_INBUF_SIZE - avpkt.size,f);
   //   if(len > 0)
   //      avpkt.size += len;
   //}

   return olen;
}


int mf_aac_uninit(AAC_CONTEXT* h264)
{

   AAC_CONTEXT_MF* sys = h264->pSystemData;

   avcodec_close(sys->c);

   avcodec_free_context(&sys->c);

   free(h264->pSystemData);

   free(h264);

   return 0;

}
