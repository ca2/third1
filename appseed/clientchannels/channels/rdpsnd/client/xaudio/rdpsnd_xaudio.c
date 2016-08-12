/**
 * FreeRDP: A Remote Desktop Protocol Implementation
 * Audio Output Virtual Channel
 *
 * Copyright 2009-2012 Jay Sorg
 * Copyright 2010-2012 Vic Lee
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//#define WINVER _WIN32_WINNT_WIN8
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


__declspec(dllimport) unsigned int  get_tick_count();


//// -  -  - BA7C - 066C40B5E2B9
//#include <initguid.h>
//#include <Windows.h>
//#include <MMSystem.h>
//#include <Mfapi.h>
//#include <strmif.h>
//#include <Mftransform.h>
//#include <wmcodecdsp.h>
//#include <Codecapi.h>
//EXTERN_C const CLSID CLSID_CMSAACDecMFT;

//#ifdef __cplusplus
//
//class DECLSPEC_UUID("32d186a7-218f-4c75-8876-dd77273a8999")
//   CMSAACDecMFT;
//#endif
//DEFINE_GUID(CLSID_CMSAACDecMFT,0x32d186a7,0x218f,0x4c75,0x88,0x76,0xdd,0x77,0x27,0x3a,0x89,0x99);

#include <winpr/crt.h>
#include <winpr/cmdline.h>

#include <freerdp/types.h>
#include <freerdp/codec/dsp.h>
#include <freerdp/channels/log.h>

#include <mmsystem.h>

#include "../rdpsnd_main.h"
#include "../codec_aac.h"


/* wave data block header */
typedef struct wavehdr_tag {
   LPSTR       lpData;                 /* pointer to locked data buffer */
   DWORD       dwBufferLength;         /* length of data buffer */
   DWORD       dwBytesRecorded;        /* used for input only */
   DWORD_PTR   dwUser;                 /* for client's use */
   DWORD       dwFlags;                /* assorted flags (see defines) */
   DWORD       dwLoops;                /* loop control counter */
   struct wavehdr_tag FAR *lpNext;     /* reserved for driver */
   DWORD_PTR   reserved;               /* reserved for driver */
} WAVEHDR,*PWAVEHDR,NEAR *NPWAVEHDR,FAR *LPWAVEHDR;



typedef struct rdpsnd_xaudio_plugin rdpsndWinmmPlugin;

struct xaudio_wave_out;


typedef struct xaudio_wave_out * HWAVEOUT;
typedef HWAVEOUT * LPHWAVEOUT;
#define MM_WOM_OPEN         0x3BB           /* waveform output */
#define MM_WOM_CLOSE        0x3BC
#define MM_WOM_DONE         0x3BD
#define MMRESULT int

#define WAVE_MAPPER     ((UINT)-1)
#define CALLBACK_FUNCTION   0x00030000l    /* dwCallback is a FARPROC */

/* general error return values */
#define MMSYSERR_BASE          0
#define MMSYSERR_NOERROR      0                    /* no error */
#define MMSYSERR_ERROR        (MMSYSERR_BASE + 1)  /* unspecified error */
#define MMSYSERR_BADDEVICEID  (MMSYSERR_BASE + 2)  /* device ID out of range */
#define MMSYSERR_NOTENABLED   (MMSYSERR_BASE + 3)  /* driver failed enable */
#define MMSYSERR_ALLOCATED    (MMSYSERR_BASE + 4)  /* device already allocated */
#define MMSYSERR_INVALHANDLE  (MMSYSERR_BASE + 5)  /* device handle is invalid */
#define MMSYSERR_NODRIVER     (MMSYSERR_BASE + 6)  /* no device driver present */
#define MMSYSERR_NOMEM        (MMSYSERR_BASE + 7)  /* memory allocation error */
#define MMSYSERR_NOTSUPPORTED (MMSYSERR_BASE + 8)  /* function isn't supported */
#define MMSYSERR_BADERRNUM    (MMSYSERR_BASE + 9)  /* error value out of range */
#define MMSYSERR_INVALFLAG    (MMSYSERR_BASE + 10) /* invalid flag passed */
#define MMSYSERR_INVALPARAM   (MMSYSERR_BASE + 11) /* invalid parameter passed */
#define MMSYSERR_HANDLEBUSY   (MMSYSERR_BASE + 12) /* handle being used */
/* simultaneously on another */
/* thread (eg callback) */
#define MMSYSERR_INVALIDALIAS (MMSYSERR_BASE + 13) /* specified alias not found */
#define MMSYSERR_BADDB        (MMSYSERR_BASE + 14) /* bad registry database */
#define MMSYSERR_KEYNOTFOUND  (MMSYSERR_BASE + 15) /* registry key not found */
#define MMSYSERR_READERROR    (MMSYSERR_BASE + 16) /* registry read error */
#define MMSYSERR_WRITEERROR   (MMSYSERR_BASE + 17) /* registry write error */
#define MMSYSERR_DELETEERROR  (MMSYSERR_BASE + 18) /* registry delete error */
#define MMSYSERR_VALNOTFOUND  (MMSYSERR_BASE + 19) /* registry value not found */
#define MMSYSERR_NODRIVERCB   (MMSYSERR_BASE + 20) /* driver does not call DriverCallback */
#define MMSYSERR_MOREDATA     (MMSYSERR_BASE + 21) /* more data to be returned */
#define MMSYSERR_LASTERROR    (MMSYSERR_BASE + 21) /* last error in range */


MMRESULT waveOutOpen(
   LPHWAVEOUT     phwo,
   UINT_PTR       uDeviceID,
   LPWAVEFORMATEX pwfx,
   DWORD_PTR      dwCallback,
   DWORD_PTR      dwCallbackInstance,
   DWORD          fdwOpen
   );

MMRESULT waveOutWrite(
   HWAVEOUT  hwo,
   LPWAVEHDR pwh,
   UINT      cbwh
   );

MMRESULT waveOutReset(HWAVEOUT h);
MMRESULT waveOutClose(HWAVEOUT h);


struct rdpsnd_xaudio_plugin
{
   rdpsndDevicePlugin device;

   HWAVEOUT hWaveOut;
   WAVEFORMATEX format;
   int wformat;
   int block_size;
   FREERDP_DSP_CONTEXT* dsp_context;
   AAC_CONTEXT * aac_context;
   AAC_CONTEXT_MF * aac_context_mf;
};



//
//
//
//static void audio_decode_example3(const char *outfilename,const char *filename)
//{
//
//   av_init_packet(&avpkt);
//
//   printf("Decode audio file %s to %s\n",filename,outfilename);
//
//   /* find the mpeg audio decoder */
//   codec = avcodec_find_decoder(AV_CODEC_ID_AAC);
//   if(!codec) {
//      fprintf(stderr,"Codec not found\n");
//      exit(1);
//   }
//
//   c = avcodec_alloc_context3(codec);
//   if(!c) {
//      fprintf(stderr,"Could not allocate audio codec context\n");
//      exit(1);
//   }
//
//   /* open it */
//   if(avcodec_open2(c,codec,NULL) < 0) {
//      fprintf(stderr,"Could not open codec\n");
//      exit(1);
//   }
//
//   f = fopen(filename,"rb");
//   if(!f) {
//      fprintf(stderr,"Could not open %s\n",filename);
//      exit(1);
//   }
//   outfile = fopen(outfilename,"wb");
//   if(!outfile) {
//      av_free(c);
//      exit(1);
//   }
//
//   /* decode until eof */
//   avpkt.data = inbuf;
//   avpkt.size = fread(inbuf,1,AUDIO_INBUF_SIZE,f);
//
//   while(avpkt.size > 0) {
//      int got_frame = 0;
//
//      if(!decoded_frame) {
//         if(!(decoded_frame = av_frame_alloc())) {
//            fprintf(stderr,"Could not allocate audio frame\n");
//            exit(1);
//         }
//      }
//      else
//         av_frame_unref(decoded_frame);
//
//      len = avcodec_decode_audio4(c,decoded_frame,&got_frame,&avpkt);
//      if(len < 0) {
//         fprintf(stderr,"Error while decoding\n");
//         exit(1);
//      }
//      if(got_frame) {
//         /* if a frame has been decoded, output it */
//         int data_size = av_samples_get_buffer_size(NULL,c->channels,
//            decoded_frame->nb_samples,
//            c->sample_fmt,1);
//         fwrite(decoded_frame->data[0],1,data_size,outfile);
//      }
//      avpkt.size -= len;
//      avpkt.data += len;
//      avpkt.dts =
//         avpkt.pts = AV_NOPTS_VALUE;
//      if(avpkt.size < AUDIO_REFILL_THRESH) {
//         /* Refill the input buffer, to avoid trying to decode
//         * incomplete frames. Instead of this, one could also use
//         * a parser, or use a proper container format through
//         * libavformat. */
//         memmove(inbuf,avpkt.data,avpkt.size);
//         avpkt.data = inbuf;
//         len = fread(avpkt.data + avpkt.size,1,
//            AUDIO_INBUF_SIZE - avpkt.size,f);
//         if(len > 0)
//            avpkt.size += len;
//      }
//   }
//
//   fclose(outfile);
//   fclose(f);
//
//   avcodec_close(c);
//   av_free(c);
//   av_frame_free(&decoded_frame);
//}

//static HRESULT mf_aac_find_output_type(AAC_CONTEXT_MF* sys,const GUID* guid,IMFMediaType** ppMediaType)
//{
//   DWORD idx = 0;
//   GUID mediaGuid;
//   HRESULT hr = S_OK;
//   IMFMediaType* pMediaType = NULL;
//
//   while(1)
//   {
//      hr = sys->transform->lpVtbl->GetOutputAvailableType(sys->transform,0,idx,&pMediaType);
//
//      if(FAILED(hr))
//         break;
//
//      pMediaType->lpVtbl->GetGUID(pMediaType,&MF_MT_SUBTYPE,&mediaGuid);
//
//      if(IsEqualGUID(&mediaGuid,guid))
//      {
//         *ppMediaType = pMediaType;
//         return S_OK;
//      }
//
//      pMediaType->lpVtbl->Release(pMediaType);
//
//      idx++;
//   }
//
//   return hr;
//}
//
//static HRESULT mf_aac_create_output_sample(AAC_CONTEXT_MF* sys)
//{
//   HRESULT hr = S_OK;
//   MFT_OUTPUT_STREAM_INFO streamInfo;
//
//   if(sys->outputSample)
//   {
//      sys->outputSample->lpVtbl->Release(sys->outputSample);
//      sys->outputSample = NULL;
//   }
//
//   hr = sys->MFCreateSample(&sys->outputSample);
//
//   if(FAILED(hr))
//   {
//      WLog_ERR(TAG,"MFCreateSample failure: 0x%04X",hr);
//      goto error;
//   }
//
//   hr = sys->transform->lpVtbl->GetOutputStreamInfo(sys->transform,0,&streamInfo);
//
//   if(FAILED(hr))
//   {
//      WLog_ERR(TAG,"GetOutputStreamInfo failure: 0x%04X",hr);
//      goto error;
//   }
//
//   hr = sys->MFCreateMemoryBuffer(streamInfo.cbSize,&sys->outputBuffer);
//
//   if(FAILED(hr))
//   {
//      WLog_ERR(TAG,"MFCreateMemoryBuffer failure: 0x%04X",hr);
//      goto error;
//   }
//
//   sys->outputSample->lpVtbl->AddBuffer(sys->outputSample,sys->outputBuffer);
//
//   if(FAILED(hr))
//   {
//      WLog_ERR(TAG,"AddBuffer failure: 0x%04X",hr);
//      goto error;
//   }
//
//   sys->outputBuffer->lpVtbl->Release(sys->outputBuffer);
//
//error:
//   return hr;
//}
//
//static void mf_aac_uninit(AAC_CONTEXT* h264)
//{
//   AAC_CONTEXT_MF* sys = (AAC_CONTEXT_MF*)h264->pSystemData;
//
//   if(sys)
//   {
//      if(sys->transform)
//      {
//         sys->transform->lpVtbl->Release(sys->transform);
//         sys->transform = NULL;
//      }
//
//      if(sys->codecApi)
//      {
//         sys->codecApi->lpVtbl->Release(sys->codecApi);
//         sys->codecApi = NULL;
//      }
//
//      if(sys->inputType)
//      {
//         sys->inputType->lpVtbl->Release(sys->inputType);
//         sys->inputType = NULL;
//      }
//
//      if(sys->outputType)
//      {
//         sys->outputType->lpVtbl->Release(sys->outputType);
//         sys->outputType = NULL;
//      }
//
//      if(sys->outputSample)
//      {
//         sys->outputSample->lpVtbl->Release(sys->outputSample);
//         sys->outputSample = NULL;
//      }
//
//      if(sys->mfplat)
//      {
//         FreeLibrary(sys->mfplat);
//         sys->mfplat = NULL;
//      }
//
//      free(h264->pYUVData[0]);
//      h264->pYUVData[0] = h264->pYUVData[1] = h264->pYUVData[2] = NULL;
//      h264->iStride[0] = h264->iStride[1] = h264->iStride[2] = 0;
//
//      sys->MFShutdown();
//
//      CoUninitialize();
//
//      free(sys);
//      h264->pSystemData = NULL;
//   }
//}
//
//static BOOL mf_aac_init(AAC_CONTEXT* h264,WAVEFORMATEX * format,AUDIO_FORMAT * in)
//{
//   HRESULT hr;
//   AAC_CONTEXT_MF* sys;
//
//   sys = (AAC_CONTEXT_MF*)calloc(1,sizeof(AAC_CONTEXT_MF));
//
//   if(!sys)
//      goto error;
//
//   h264->pSystemData = (void*)sys;
//
//   /* http://decklink-sdk-delphi.googlecode.com/svn/trunk/Blackmagic%20DeckLink%20SDK%209.7/Win/Samples/Streaming/StreamingPreview/DecoderMF.cpp */
//
//   sys->mfplat = LoadLibraryA("mfplat.dll");
//
//   if(!sys->mfplat)
//      goto error;
//
//   sys->MFStartup = (pfnMFStartup)GetProcAddress(sys->mfplat,"MFStartup");
//   sys->MFShutdown = (pfnMFShutdown)GetProcAddress(sys->mfplat,"MFShutdown");
//   sys->MFCreateSample = (pfnMFCreateSample)GetProcAddress(sys->mfplat,"MFCreateSample");
//   sys->MFCreateMemoryBuffer = (pfnMFCreateMemoryBuffer)GetProcAddress(sys->mfplat,"MFCreateMemoryBuffer");
//   sys->MFCreateMediaType = (pfnMFCreateMediaType)GetProcAddress(sys->mfplat,"MFCreateMediaType");
//   sys->MFCreateDXGIDeviceManager = (pfnMFCreateDXGIDeviceManager)GetProcAddress(sys->mfplat,"MFCreateDXGIDeviceManager");
//
//   if(!sys->MFStartup || !sys->MFShutdown || !sys->MFCreateSample || !sys->MFCreateMemoryBuffer ||
//      !sys->MFCreateMediaType || !sys->MFCreateDXGIDeviceManager)
//      goto error;
//
//   CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
//
//   if(h264->Compressor)
//   {
//
//   }
//   else
//   {
//      VARIANT var ={0};
//
//      //hr = sys->MFStartup(MF_VERSION,MFSTARTUP_FULL);
//      hr = MFStartup(MF_VERSION,MFSTARTUP_FULL);
//
//      if(FAILED(hr))
//      {
//         WLog_ERR(TAG,"MFStartup failure: 0x%04X",hr);
//         goto error;
//      }
//
//      //HRESULT hr = S_OK;
//      //UINT32 count = 0;
//      //IMFActivate ** activate = NULL;
//      //MFT_REGISTER_TYPE_INFO info ={0};
//
//      //info.guidMajorType = MFMediaType_Audio;
//      //info.guidSubtype = MFAudioFormat_AAC;
//      //UINT32 flags = MFT_ENUM_FLAG_SYNCMFT | MFT_ENUM_FLAG_LOCALMFT | MFT_ENUM_FLAG_SORTANDFILTER;
//
//      //hr = MFTEnumEx(MFT_CATEGORY_AUDIO_DECODER,
//      //   flags,
//      //   NULL,
//      //   &info,
//      //   &activate,
//      //   &count);
//      //if(FAILED(hr)) {
//      //   printf("Fail at MFTEnumEx. AAC transform encoder could not be initialized");
//      //   goto error;
//      //}
//      //printf("MFTEnumEx succeeded");
//
//      //if(count == 0) {
//      //   printf("Fail at getting the AAC encoder. AAC transform encoder could not be initialized");
//      //   goto error;
//      //}
//
//      //hr = activate[0]->lpVtbl->ActivateObject(activate[0],&IID_IMFTransform, &sys->transform);
//      //if(FAILED(hr)) {
//      //   printf("Fail at activating AAC encoder. AAC transform encoder could not be initialized");
//      //   for(UINT32 idx = 0; idx < count; idx++) {
//      //      activate[idx]->lpVtbl->Release(activate[idx]);
//      //   }
//      //   CoTaskMemFree(activate);
//      //}
//      //else
//      //{
//      //   for(UINT32 idx = 1; idx < count; idx++) {
//      //      activate[idx]->lpVtbl->Release(activate[idx]);
//      //   }
//      //}
//      //printf("AAC MFT encoder successfully initialized.");
//
//
//      //hr = CoCreateInstance(&CLSID_CMSAACDecMFT,NULL,CLSCTX_INPROC_SERVER,&IID_IMFTransform,(void**)&sys->transform);
//
//      //if(FAILED(hr))
//      //{
//      //   WLog_ERR(TAG,"CoCreateInstance(CLSID_CMSH264DecoderMFT) failure: 0x%04X",hr);
//      //   goto error;
//      //}
//
//      //hr = sys->transform->lpVtbl->QueryInterface(sys->transform,&IID_ICodecAPI,(void**)&sys->codecApi);
//
//      //if(FAILED(hr))
//      //{
//      //   WLog_ERR(TAG,"QueryInterface(IID_ICodecAPI) failure: 0x%04X",hr);
//      //   goto error;
//      //}
//
//      //var.vt = VT_UI4;
//      //var.ulVal = 1;
//
//      //hr = sys->codecApi->lpVtbl->SetValue(sys->codecApi,&CODECAPI_AVLowLatencyMode,&var);
//
//      //if(FAILED(hr))
//      //{
//      //   WLog_ERR(TAG,"SetValue(CODECAPI_AVLowLatencyMode) failure: 0x%04X",hr);
//      //   goto error;
//      //}
//
//      hr = sys->MFCreateMediaType(&sys->inputType);
//
//      if(FAILED(hr))
//      {
//         WLog_ERR(TAG,"MFCreateMediaType failure: 0x%04X",hr);
//         goto error;
//      }
//
//      hr = sys->inputType->lpVtbl->SetGUID(sys->inputType,&MF_MT_MAJOR_TYPE,&MFMediaType_Audio);
//
//      if(FAILED(hr))
//      {
//         WLog_ERR(TAG,"SetGUID(MF_MT_MAJOR_TYPE) failure: 0x%04X",hr);
//         goto error;
//      }
//
//      hr = sys->inputType->lpVtbl->SetGUID(sys->inputType,&MF_MT_SUBTYPE,&MFAudioFormat_AAC);
//
//      if(FAILED(hr))
//      {
//         WLog_ERR(TAG,"SetGUID(MF_MT_SUBTYPE) failure: 0x%04X",hr);
//         goto error;
//      }
//
//      hr = sys->inputType->lpVtbl->SetUINT32(sys->inputType,&MF_MT_AAC_AUDIO_PROFILE_LEVEL_INDICATION,0) ;
//      if(FAILED(hr))
//      {
//         WLog_ERR(TAG,"SetGUID(MF_MT_SUBTYPE) failure: 0x%04X",hr);
//         goto error;
//      }
//
//      hr = sys->inputType->lpVtbl->SetUINT32(sys->inputType,&MF_MT_AAC_PAYLOAD_TYPE,1) ;		// payload 1 = ADTS header
//      if(FAILED(hr))
//      {
//         WLog_ERR(TAG,"SetGUID(MF_MT_SUBTYPE) failure: 0x%04X",hr);
//         goto error;
//      }
//      hr = sys->inputType->lpVtbl->SetUINT32(sys->inputType,&MF_MT_AUDIO_SAMPLES_PER_SECOND,in->nSamplesPerSec);
//      if(FAILED(hr))
//      {
//         WLog_ERR(TAG,"SetGUID(MF_MT_SUBTYPE) failure: 0x%04X",hr);
//         goto error;
//      }
//      hr = sys->inputType->lpVtbl->SetUINT32(sys->inputType,&MF_MT_AUDIO_BITS_PER_SAMPLE,in->wBitsPerSample);
//      if(FAILED(hr))
//      {
//         WLog_ERR(TAG,"SetGUID(MF_MT_SUBTYPE) failure: 0x%04X",hr);
//         goto error;
//      }
//      hr = sys->inputType->lpVtbl->SetUINT32(sys->inputType,&MF_MT_AUDIO_NUM_CHANNELS,in->nChannels);
//      if(FAILED(hr))
//      {
//         WLog_ERR(TAG,"SetGUID(MF_MT_SUBTYPE) failure: 0x%04X",hr);
//         goto error;
//      }
//      hr = sys->inputType->lpVtbl->SetUINT32(sys->inputType, &MF_MT_AUDIO_CHANNEL_MASK,(in->nChannels == 2 ? 0x03 : 0x3F)) ;
//      if(FAILED(hr))
//      {
//         WLog_ERR(TAG,"SetGUID(MF_MT_SUBTYPE) failure: 0x%04X",hr);
//         goto error;
//      }
//      if(in->cbSize > 0)
//      {
//         hr = sys->inputType->lpVtbl->SetBlob(sys->inputType,&MF_MT_USER_DATA,in->data,in->cbSize);
//         if(FAILED(hr))
//         {
//            WLog_ERR(TAG,"SetGUID(MF_MT_SUBTYPE) failure: 0x%04X",hr);
//            goto error;
//         }
//      }
//      else
//      {
//         //byte arrUser[] ={0x01, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x90} ;	// hard wired for 2 channel, 48000 kHz
//         //hr = sys->inputType->lpVtbl->SetBlob(sys->inputType,&MF_MT_USER_DATA,arrUser,sizeof(arrUser));
//         //if(FAILED(hr))
//         //{
//         //   WLog_ERR(TAG,"SetGUID(MF_MT_SUBTYPE) failure: 0x%04X",hr);
//         //   goto error;
//         //}
//
//      }
//      //CHECK_HR(outMediaType->SetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND,12000));
//      //if(FAILED(hr))
//      //{
//      //   WLog_ERR(TAG,"SetGUID(MF_MT_SUBTYPE) failure: 0x%04X",hr);
//      //   goto error;
//      //}
//      //CHECK_HR(hr = outMediaType->SetUINT32(MF_MT_AUDIO_BLOCK_ALIGNMENT,outblockAlign));
//      //if(FAILED(hr))
//      //{
//      //   WLog_ERR(TAG,"SetGUID(MF_MT_SUBTYPE) failure: 0x%04X",hr);
//      //   goto error;
//      //}
//      CLSID *clsids = NULL;
//      UINT32 clsidCount=0;
//      MFT_REGISTER_TYPE_INFO inputType  ={MFMediaType_Audio, in->wFormatTag}; // WAVE_FORMAT_MPEG_HEAAC can't be searched for
//      //MFT_REGISTER_TYPE_INFO outputType ={MFMediaType_Audio, WAVE_FORMAT_PCM};         // MEDIASUBTYPE_PCM doesn't exist (but can be search for if created)
//
//      hr = MFTEnum(MFT_CATEGORY_AUDIO_DECODER,0,
//         &inputType,
//         NULL, /* &outputType, */ // search fails if outputType is not NULL
//         NULL,&clsids,&clsidCount);
//      if(FAILED(hr))
//      {
//         WLog_ERR(TAG,"SetInputType failure: 0x%04X",hr);
//         goto error;
//      }
//
//      //ListTranscoders(clsids,clsidCount);
//
//      hr = CoCreateInstance(&clsids[0],NULL,CLSCTX_ALL,&IID_IMFTransform, &sys->transform);
//      if(FAILED(hr))
//      {
//         WLog_ERR(TAG,"SetInputType failure: 0x%04X",hr);
//         goto error;
//      }
//
//      hr = sys->transform->lpVtbl->SetInputType(sys->transform,0,sys->inputType,0);
//
//      if(FAILED(hr))
//      {
//         WLog_ERR(TAG,"SetInputType failure: 0x%04X",hr);
//         goto error;
//      }
//
//      hr = mf_aac_find_output_type(sys,&MFAudioFormat_PCM,&sys->outputType);
//
//      if(FAILED(hr))
//      {
//         WLog_ERR(TAG,"mf_find_output_type failure: 0x%04X",hr);
//         goto error;
//      }
//
//      hr = sys->transform->lpVtbl->SetOutputType(sys->transform,0,sys->outputType,0);
//
//      if(FAILED(hr))
//      {
//         WLog_ERR(TAG,"SetOutputType failure: 0x%04X",hr);
//         goto error;
//      }
//
//      hr = mf_aac_create_output_sample(sys);
//
//      if(FAILED(hr))
//      {
//         WLog_ERR(TAG,"mf_create_output_sample failure: 0x%04X",hr);
//         goto error;
//      }
//   }
//   return TRUE;
//
//error:
//   WLog_ERR(TAG,"mf_init failure");
//   mf_aac_uninit(h264);
//   return FALSE;
//}


static BOOL rdpsnd_xaudio_convert_format(const AUDIO_FORMAT* in, WAVEFORMATEX* out)
{
	BOOL result = FALSE;

	ZeroMemory(out, sizeof(WAVEFORMATEX));
	out->wFormatTag = WAVE_FORMAT_PCM;
	out->nChannels = in->nChannels;
	out->nSamplesPerSec = in->nSamplesPerSec;

	switch (in->wFormatTag)
	{
		case WAVE_FORMAT_PCM:
			out->wBitsPerSample = in->wBitsPerSample;
			result = TRUE;
			break;

		case WAVE_FORMAT_ADPCM:
		case WAVE_FORMAT_DVI_ADPCM:
			out->wBitsPerSample = 16;
			result = TRUE;
			break;
      case WAVE_FORMAT_MPEG4_AAC:
            out->wBitsPerSample = in->wBitsPerSample;
            result = TRUE;
            break;
      //case WAVE_FORMAT_MPEG_ADTS_AAC:
      //   out->wBitsPerSample = in->wBitsPerSample;
      //   result = TRUE;
      //   break;
   }

	out->nBlockAlign = out->nChannels * out->wBitsPerSample / 8;
	out->nAvgBytesPerSec = out->nSamplesPerSec * out->nBlockAlign;

	return result;
}

static void rdpsnd_xaudio_set_format(rdpsndDevicePlugin* device, AUDIO_FORMAT* format, int latency)
{
	rdpsndWinmmPlugin* xaudio = (rdpsndWinmmPlugin*) device;

	if (format)
	{
		rdpsnd_xaudio_convert_format(format, &xaudio->format);

		xaudio->wformat = format->wFormatTag;
		xaudio->block_size = format->nBlockAlign;
	}
}

static void CALLBACK rdpsnd_xaudio_callback_function(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	RDPSND_WAVE* wave;
	LPWAVEHDR lpWaveHdr;
	rdpsndWinmmPlugin* xaudio = (rdpsndWinmmPlugin*) dwInstance;

	switch (uMsg)
	{
		case MM_WOM_OPEN:
			WLog_DBG(TAG,  "MM_WOM_OPEN");
			break;
		
		case MM_WOM_CLOSE:
			WLog_DBG(TAG,  "MM_WOM_CLOSE");
			break;

		case MM_WOM_DONE:
			{
				UINT32 wTimeDelta;
				lpWaveHdr = (LPWAVEHDR) dwParam1;

				if (!lpWaveHdr)
					return;

				wave = (RDPSND_WAVE*) lpWaveHdr->dwUser;

				if (!wave)
					return;

				WLog_DBG(TAG,  "MM_WOM_DONE: dwBufferLength: %d cBlockNo: %d",
						 lpWaveHdr->dwBufferLength, wave->cBlockNo);
				wave->wLocalTimeB = get_tick_count();
				wTimeDelta = wave->wLocalTimeB - wave->wLocalTimeA;
				wave->wTimeStampB = wave->wTimeStampA + wTimeDelta;

				xaudio->device.WaveConfirm(&(xaudio->device), wave);

            //if(xaudio->aac_context != NULL)
            //{
            //   if(lpWaveHdr->lpData)
            //   {
            //      av_freep(((uint8_t ** )lpWaveHdr->lpData)[0]);
            //      av_freep((uint8_t **)lpWaveHdr->lpData);
            //   }
            //}
            //else
#ifdef METROWIN
            _aligned_free(lpWaveHdr->lpData);
#else
            {
               free(lpWaveHdr->lpData);
            }
#endif

				free(wave);
			}
			break;
	}
}


static void rdpsnd_xaudio_open(rdpsndDevicePlugin* device, AUDIO_FORMAT* format, int latency)
{
	MMRESULT mmResult;
	rdpsndWinmmPlugin* xaudio = (rdpsndWinmmPlugin*) device;

	if (xaudio->hWaveOut)
		return;

   rdpsnd_xaudio_set_format(device,format,latency);
   //freerdp_dsp_context_reset_adpcm(xaudio->dsp_context);

   if(format->wFormatTag == WAVE_FORMAT_MPEG4_AAC)
   {
      xaudio->dsp_context = NULL;
      xaudio->format.wFormatTag = 1;
      xaudio->aac_context = calloc(1,sizeof(AAC_CONTEXT));
      if(!mf_aac_init(xaudio->aac_context, xaudio->format.nSamplesPerSec, xaudio->format.nChannels,format))
      {
         WLog_ERR(TAG,"mf_aac_init failed");
      }
   }

	mmResult = waveOutOpen(&xaudio->hWaveOut, WAVE_MAPPER, &xaudio->format,
			(DWORD_PTR) rdpsnd_xaudio_callback_function, (DWORD_PTR) xaudio, CALLBACK_FUNCTION);

	if (mmResult != MMSYSERR_NOERROR)
	{
		WLog_ERR(TAG,  "waveOutOpen failed: %d", mmResult);
	}
}

static void rdpsnd_xaudio_close(rdpsndDevicePlugin* device)
{
	MMRESULT mmResult;
	rdpsndWinmmPlugin* xaudio = (rdpsndWinmmPlugin*) device;

	if (xaudio->hWaveOut)
	{
		mmResult = waveOutReset(xaudio->hWaveOut);

		mmResult = waveOutClose(xaudio->hWaveOut);

		if (mmResult != MMSYSERR_NOERROR)
		{
			WLog_ERR(TAG,  "waveOutClose failure: %d", mmResult);
		}
		
		xaudio->hWaveOut = NULL;
	}
}

static void rdpsnd_xaudio_free(rdpsndDevicePlugin* device)
{
	rdpsndWinmmPlugin* xaudio = (rdpsndWinmmPlugin*) device;

	if (xaudio)
	{
		rdpsnd_xaudio_close(device);

		freerdp_dsp_context_free(xaudio->dsp_context);

		free(xaudio);
	}
}

static BOOL rdpsnd_xaudio_format_supported(rdpsndDevicePlugin* device, AUDIO_FORMAT* format)
{
	MMRESULT result;
	WAVEFORMATEX out;

	if (rdpsnd_xaudio_convert_format(format, &out))
	{
//		result = waveOutOpen(NULL, WAVE_MAPPER, &out, 0, 0, WAVE_FORMAT_QUERY);
      result = MMSYSERR_ERROR;
      if(format->wFormatTag == WAVE_FORMAT_MPEG4_AAC)
      {
         result = MMSYSERR_NOERROR;
         //format->cbSize = sizeof(WAVEFORMATEX);
      }
		if (result == MMSYSERR_NOERROR)
			return TRUE;
	}

	return FALSE;
}

static UINT32 rdpsnd_xaudio_get_volume(rdpsndDevicePlugin* device)
{
	DWORD dwVolume;
	UINT16 dwVolumeLeft;
	UINT16 dwVolumeRight;

	rdpsndWinmmPlugin* xaudio = (rdpsndWinmmPlugin*) device;

	dwVolumeLeft = ((50 * 0xFFFF) / 100); /* 50% */
	dwVolumeRight = ((50 * 0xFFFF) / 100); /* 50% */
	dwVolume = (dwVolumeLeft << 16) | dwVolumeRight;

	if (!xaudio->hWaveOut)
		return dwVolume;

//	waveOutGetVolume(xaudio->hWaveOut, &dwVolume);

	return dwVolume;
}

static void rdpsnd_xaudio_set_volume(rdpsndDevicePlugin* device, UINT32 value)
{
	rdpsndWinmmPlugin* xaudio = (rdpsndWinmmPlugin*) device;

	if (!xaudio->hWaveOut)
		return;

//	waveOutSetVolume(xaudio->hWaveOut, value);
}

static void rdpsnd_xaudio_wave_decode(rdpsndDevicePlugin* device, RDPSND_WAVE* wave)
{
	int length;
	BYTE* data;
	rdpsndWinmmPlugin* xaudio = (rdpsndWinmmPlugin*) device;

	if (xaudio->wformat == WAVE_FORMAT_ADPCM)
	{
		xaudio->dsp_context->decode_ms_adpcm(xaudio->dsp_context,
			wave->data, wave->length, xaudio->format.nChannels, xaudio->block_size);
		length = xaudio->dsp_context->adpcm_size;
		data = xaudio->dsp_context->adpcm_buffer;
	}
	else if (xaudio->wformat == WAVE_FORMAT_DVI_ADPCM)
	{
		xaudio->dsp_context->decode_ima_adpcm(xaudio->dsp_context,
			wave->data, wave->length, xaudio->format.nChannels, xaudio->block_size);
		length = xaudio->dsp_context->adpcm_size;
		data = xaudio->dsp_context->adpcm_buffer;
	}
	else
	{
		length = wave->length;
		data = wave->data;
	}

	wave->data = (BYTE*) malloc(length);
	CopyMemory(wave->data, data, length);
	wave->length = length;
}

void rdpsnd_xaudio_wave_play(rdpsndDevicePlugin* device, RDPSND_WAVE* wave)
{
	MMRESULT mmResult;
	LPWAVEHDR lpWaveHdr;
	rdpsndWinmmPlugin* xaudio = (rdpsndWinmmPlugin*) device;

	if (!xaudio->hWaveOut)
		return;

	wave->AutoConfirm = FALSE;

	lpWaveHdr = (LPWAVEHDR) malloc(sizeof(WAVEHDR));

	if (!lpWaveHdr)
		return;

	ZeroMemory(lpWaveHdr, sizeof(WAVEHDR));
   LPSTR data = NULL;
   int l = 0;
   if(xaudio->aac_context != NULL)
   {
      l = audio_decode_example2(xaudio->aac_context,&data,wave->data,wave->length);
      free(wave->data);
   }
   else
   {
      data = wave->data;
      l = wave->length;
   }

   if(data != NULL && l > 0)
   {

      lpWaveHdr->dwFlags = 0;
      lpWaveHdr->dwLoops = 0;
      lpWaveHdr->lpData = (LPSTR)data;
      lpWaveHdr->dwBufferLength = l;
      lpWaveHdr->dwUser = (DWORD_PTR)wave;
      lpWaveHdr->lpNext = NULL;

//      mmResult = waveOutPrepareHeader(xaudio->hWaveOut,lpWaveHdr,sizeof(WAVEHDR));

      mmResult = MMSYSERR_NOERROR;

      if(mmResult != MMSYSERR_NOERROR)
      {
         WLog_ERR(TAG,"waveOutPrepareHeader failure: %d",mmResult);
         return;
      }

      mmResult = waveOutWrite(xaudio->hWaveOut,lpWaveHdr,sizeof(WAVEHDR));

      if(xaudio->aac_context != NULL)
      {
//         free(data);
      }

   }

	if (mmResult != MMSYSERR_NOERROR)
	{
		WLog_ERR(TAG,  "waveOutWrite failure: %d", mmResult);
      _aligned_free(data);
//		waveOutUnprepareHeader(xaudio->hWaveOut, lpWaveHdr, sizeof(WAVEHDR));
		return;
	}
}

static void rdpsnd_xaudio_start(rdpsndDevicePlugin* device)
{
	//rdpsndWinmmPlugin* xaudio = (rdpsndWinmmPlugin*) device;
}

static void rdpsnd_xaudio_parse_addin_args(rdpsndDevicePlugin* device, ADDIN_ARGV* args)
{

}

#ifdef STATIC_CHANNELS
#define freerdp_rdpsnd_client_subsystem_entry	xaudio_freerdp_rdpsnd_client_subsystem_entry
#endif

int freerdp_rdpsnd_client_subsystem_entry(PFREERDP_RDPSND_DEVICE_ENTRY_POINTS pEntryPoints)
{
	ADDIN_ARGV* args;
	rdpsndWinmmPlugin* xaudio;

	xaudio = (rdpsndWinmmPlugin*) calloc(1, sizeof(rdpsndWinmmPlugin));

	if (!xaudio)
		return -1;

	xaudio->device.DisableConfirmThread = TRUE;

	xaudio->device.Open = rdpsnd_xaudio_open;
	xaudio->device.FormatSupported = rdpsnd_xaudio_format_supported;
	xaudio->device.SetFormat = rdpsnd_xaudio_set_format;
	xaudio->device.GetVolume = rdpsnd_xaudio_get_volume;
	xaudio->device.SetVolume = rdpsnd_xaudio_set_volume;
	xaudio->device.WaveDecode = rdpsnd_xaudio_wave_decode;
	xaudio->device.WavePlay = rdpsnd_xaudio_wave_play;
	xaudio->device.Start = rdpsnd_xaudio_start;
	xaudio->device.Close = rdpsnd_xaudio_close;
	xaudio->device.Free = rdpsnd_xaudio_free;

	args = pEntryPoints->args;
	rdpsnd_xaudio_parse_addin_args((rdpsndDevicePlugin*) xaudio, args);

	xaudio->dsp_context = freerdp_dsp_context_new();

	pEntryPoints->pRegisterRdpsndDevice(pEntryPoints->rdpsnd, (rdpsndDevicePlugin*) xaudio);

	return 0;
}

