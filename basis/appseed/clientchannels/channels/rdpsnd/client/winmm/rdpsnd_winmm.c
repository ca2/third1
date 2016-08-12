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

typedef struct rdpsnd_winmm_plugin rdpsndWinmmPlugin;


struct rdpsnd_winmm_plugin
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


static BOOL rdpsnd_winmm_convert_format(const AUDIO_FORMAT* in, WAVEFORMATEX* out)
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

static void rdpsnd_winmm_set_format(rdpsndDevicePlugin* device, AUDIO_FORMAT* format, int latency)
{
	rdpsndWinmmPlugin* winmm = (rdpsndWinmmPlugin*) device;

	if (format)
	{
		rdpsnd_winmm_convert_format(format, &winmm->format);

		winmm->wformat = format->wFormatTag;
		winmm->block_size = format->nBlockAlign;
	}
}

static void CALLBACK rdpsnd_winmm_callback_function(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	RDPSND_WAVE* wave;
	LPWAVEHDR lpWaveHdr;
	rdpsndWinmmPlugin* winmm = (rdpsndWinmmPlugin*) dwInstance;

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
				wave->wLocalTimeB = GetTickCount();
				wTimeDelta = wave->wLocalTimeB - wave->wLocalTimeA;
				wave->wTimeStampB = wave->wTimeStampA + wTimeDelta;

				winmm->device.WaveConfirm(&(winmm->device), wave);

            //if(winmm->aac_context != NULL)
            //{
            //   if(lpWaveHdr->lpData)
            //   {
            //      av_freep(((uint8_t ** )lpWaveHdr->lpData)[0]);
            //      av_freep((uint8_t **)lpWaveHdr->lpData);
            //   }
            //}
            //else
            {
               free(lpWaveHdr->lpData);
            }

				free(wave);
			}
			break;
	}
}


static void rdpsnd_winmm_open(rdpsndDevicePlugin* device, AUDIO_FORMAT* format, int latency)
{
	MMRESULT mmResult;
	rdpsndWinmmPlugin* winmm = (rdpsndWinmmPlugin*) device;

	if (winmm->hWaveOut)
		return;

   rdpsnd_winmm_set_format(device,format,latency);
   //freerdp_dsp_context_reset_adpcm(winmm->dsp_context);

   if(format->wFormatTag == WAVE_FORMAT_MPEG4_AAC)
   {
      winmm->dsp_context = NULL;
      winmm->format.wFormatTag = 1;
      winmm->aac_context = calloc(1,sizeof(AAC_CONTEXT));
      if(!mf_aac_init(winmm->aac_context, winmm->format.nSamplesPerSec, winmm->format.nChannels,format))
      {
         WLog_ERR(TAG,"mf_aac_init failed");
      }
   }

	mmResult = waveOutOpen(&winmm->hWaveOut, WAVE_MAPPER, &winmm->format,
			(DWORD_PTR) rdpsnd_winmm_callback_function, (DWORD_PTR) winmm, CALLBACK_FUNCTION);

	if (mmResult != MMSYSERR_NOERROR)
	{
		WLog_ERR(TAG,  "waveOutOpen failed: %d", mmResult);
	}
}

static void rdpsnd_winmm_close(rdpsndDevicePlugin* device)
{
	MMRESULT mmResult;
	rdpsndWinmmPlugin* winmm = (rdpsndWinmmPlugin*) device;

	if (winmm->hWaveOut)
	{
		mmResult = waveOutReset(winmm->hWaveOut);

		mmResult = waveOutClose(winmm->hWaveOut);

		if (mmResult != MMSYSERR_NOERROR)
		{
			WLog_ERR(TAG,  "waveOutClose failure: %d", mmResult);
		}
		
		winmm->hWaveOut = NULL;
	}
}

static void rdpsnd_winmm_free(rdpsndDevicePlugin* device)
{
	rdpsndWinmmPlugin* winmm = (rdpsndWinmmPlugin*) device;

	if (winmm)
	{
		rdpsnd_winmm_close(device);

		freerdp_dsp_context_free(winmm->dsp_context);

		free(winmm);
	}
}

static BOOL rdpsnd_winmm_format_supported(rdpsndDevicePlugin* device, AUDIO_FORMAT* format)
{
	MMRESULT result;
	WAVEFORMATEX out;

	if (rdpsnd_winmm_convert_format(format, &out))
	{
		result = waveOutOpen(NULL, WAVE_MAPPER, &out, 0, 0, WAVE_FORMAT_QUERY);
      if(format->wFormatTag == WAVE_FORMAT_MPEG4_AAC && result == MMSYSERR_NOERROR)
      {
         //format->cbSize = sizeof(WAVEFORMATEX);
      }
		if (result == MMSYSERR_NOERROR)
			return TRUE;
	}

	return FALSE;
}

static UINT32 rdpsnd_winmm_get_volume(rdpsndDevicePlugin* device)
{
	DWORD dwVolume;
	UINT16 dwVolumeLeft;
	UINT16 dwVolumeRight;

	rdpsndWinmmPlugin* winmm = (rdpsndWinmmPlugin*) device;

	dwVolumeLeft = ((50 * 0xFFFF) / 100); /* 50% */
	dwVolumeRight = ((50 * 0xFFFF) / 100); /* 50% */
	dwVolume = (dwVolumeLeft << 16) | dwVolumeRight;

	if (!winmm->hWaveOut)
		return dwVolume;

	waveOutGetVolume(winmm->hWaveOut, &dwVolume);

	return dwVolume;
}

static void rdpsnd_winmm_set_volume(rdpsndDevicePlugin* device, UINT32 value)
{
	rdpsndWinmmPlugin* winmm = (rdpsndWinmmPlugin*) device;

	if (!winmm->hWaveOut)
		return;

	waveOutSetVolume(winmm->hWaveOut, value);
}

static void rdpsnd_winmm_wave_decode(rdpsndDevicePlugin* device, RDPSND_WAVE* wave)
{
	int length;
	BYTE* data;
	rdpsndWinmmPlugin* winmm = (rdpsndWinmmPlugin*) device;

	if (winmm->wformat == WAVE_FORMAT_ADPCM)
	{
		winmm->dsp_context->decode_ms_adpcm(winmm->dsp_context,
			wave->data, wave->length, winmm->format.nChannels, winmm->block_size);
		length = winmm->dsp_context->adpcm_size;
		data = winmm->dsp_context->adpcm_buffer;
	}
	else if (winmm->wformat == WAVE_FORMAT_DVI_ADPCM)
	{
		winmm->dsp_context->decode_ima_adpcm(winmm->dsp_context,
			wave->data, wave->length, winmm->format.nChannels, winmm->block_size);
		length = winmm->dsp_context->adpcm_size;
		data = winmm->dsp_context->adpcm_buffer;
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

void rdpsnd_winmm_wave_play(rdpsndDevicePlugin* device, RDPSND_WAVE* wave)
{
	MMRESULT mmResult;
	LPWAVEHDR lpWaveHdr;
	rdpsndWinmmPlugin* winmm = (rdpsndWinmmPlugin*) device;

	if (!winmm->hWaveOut)
		return;

	wave->AutoConfirm = FALSE;

	lpWaveHdr = (LPWAVEHDR) malloc(sizeof(WAVEHDR));

	if (!lpWaveHdr)
		return;

	ZeroMemory(lpWaveHdr, sizeof(WAVEHDR));
   LPSTR data = NULL;
   int l = 0;
   if(winmm->aac_context != NULL)
   {
      l = audio_decode_example2(winmm->aac_context,&data,wave->data,wave->length);
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

      mmResult = waveOutPrepareHeader(winmm->hWaveOut,lpWaveHdr,sizeof(WAVEHDR));

      if(mmResult != MMSYSERR_NOERROR)
      {
         WLog_ERR(TAG,"waveOutPrepareHeader failure: %d",mmResult);
         return;
      }

      mmResult = waveOutWrite(winmm->hWaveOut,lpWaveHdr,sizeof(WAVEHDR));

      if(winmm->aac_context != NULL)
      {
//         free(data);
      }

   }

	if (mmResult != MMSYSERR_NOERROR)
	{
		WLog_ERR(TAG,  "waveOutWrite failure: %d", mmResult);
		waveOutUnprepareHeader(winmm->hWaveOut, lpWaveHdr, sizeof(WAVEHDR));
		return;
	}
}

static void rdpsnd_winmm_start(rdpsndDevicePlugin* device)
{
	//rdpsndWinmmPlugin* winmm = (rdpsndWinmmPlugin*) device;
}

static void rdpsnd_winmm_parse_addin_args(rdpsndDevicePlugin* device, ADDIN_ARGV* args)
{

}

#ifdef STATIC_CHANNELS
#define freerdp_rdpsnd_client_subsystem_entry	winmm_freerdp_rdpsnd_client_subsystem_entry
#endif

int freerdp_rdpsnd_client_subsystem_entry(PFREERDP_RDPSND_DEVICE_ENTRY_POINTS pEntryPoints)
{
	ADDIN_ARGV* args;
	rdpsndWinmmPlugin* winmm;

	winmm = (rdpsndWinmmPlugin*) calloc(1, sizeof(rdpsndWinmmPlugin));

	if (!winmm)
		return -1;

	winmm->device.DisableConfirmThread = TRUE;

	winmm->device.Open = rdpsnd_winmm_open;
	winmm->device.FormatSupported = rdpsnd_winmm_format_supported;
	winmm->device.SetFormat = rdpsnd_winmm_set_format;
	winmm->device.GetVolume = rdpsnd_winmm_get_volume;
	winmm->device.SetVolume = rdpsnd_winmm_set_volume;
	winmm->device.WaveDecode = rdpsnd_winmm_wave_decode;
	winmm->device.WavePlay = rdpsnd_winmm_wave_play;
	winmm->device.Start = rdpsnd_winmm_start;
	winmm->device.Close = rdpsnd_winmm_close;
	winmm->device.Free = rdpsnd_winmm_free;

	args = pEntryPoints->args;
	rdpsnd_winmm_parse_addin_args((rdpsndDevicePlugin*) winmm, args);

	winmm->dsp_context = freerdp_dsp_context_new();

	pEntryPoints->pRegisterRdpsndDevice(pEntryPoints->rdpsnd, (rdpsndDevicePlugin*) winmm);

	return 0;
}
