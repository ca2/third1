/**
* FreeRDP: A Remote Desktop Protocol Implementation
* Audio Output Virtual Channel
*
* Copyright 2009-2012 Jay Sorg
* Copyright 2010-2012 Vic Lee
* Copyright 2015 Thincast Technologies GmbH
* Copyright 2015 DI (FH) Martin Haimberger <martin.haimberger@thincast.com>
* Copyright 2016 David PHAM-VAN <d.phamvan@inuvika.com>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Windows.h>
#include "app/appseed/aura/multimedia/audio/audio_c.h"

#include <winpr/crt.h>
#include <winpr/cmdline.h>

#include <freerdp/types.h>
#include <freerdp/codec/dsp.h>
#include <freerdp/channels/log.h>

#include "third/appseed/clientchannels/channels/rdpsnd/client/rdpsnd_main.h"



typedef struct _WAVEOUT_PACKET
{

   char        m_chChannel;
   void *      m_pdata;
   int         m_iChannelCount;
   int         m_iSamplesPerSec;

} WAVEOUT_PACKET, *LPWAVEOUTPACKET;


int wave_out_push_packet(WAVEOUT_PACKET * ppacket);

typedef struct rdpsnd_audio_decode_plugin rdpsndAudioDecodePlugin;

struct rdpsnd_audio_decode_plugin
{
   rdpsndDevicePlugin device;

   LPWAVEOUT lpwaveout;


   WAVEFORMATEX format;
   int wformat;
   int block_size;
   UINT32 volume;
   FREERDP_DSP_CONTEXT* dsp_context;
};

static BOOL rdpsnd_audio_decode_convert_format(const AUDIO_FORMAT* in, WAVEFORMATEX* out)
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

      //case WAVE_FORMAT_ADPCM:
      //case WAVE_FORMAT_DVI_ADPCM:
      //   out->wBitsPerSample = 16;
      //   result = TRUE;
      break;
   }

   out->nBlockAlign = out->nChannels * out->wBitsPerSample / 8;
   out->nAvgBytesPerSec = out->nSamplesPerSec * out->nBlockAlign;

   return result;
}

static BOOL rdpsnd_audio_decode_set_format(rdpsndDevicePlugin* device, AUDIO_FORMAT* format, int latency)
{
   rdpsndAudioDecodePlugin* paudiodecode = (rdpsndAudioDecodePlugin*)device;

   if (format)
   {
      if (!rdpsnd_audio_decode_convert_format(format, &paudiodecode->format))
         return FALSE;

      paudiodecode->wformat = format->wFormatTag;
      paudiodecode->block_size = format->nBlockAlign;
   }
   return TRUE;
}

static void CALLBACK rdpsnd_audio_decode_callback_function(LPWAVEOUT pwaveout, unsigned int uiMessage, void * pdata, unsigned int s)
{

   //waveRDPSND_WAVE* wave;
   ///rdpsndAudioDecodePlugin* paudiodecode = (rdpsndAudioDecodePlugin*)pwaveout->userdata;

}

//static void CALLBACK rdpsnd_audio_decode_callback_function(LPWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
//{
//   RDPSND_WAVE* wave;
//   LPWAVEHDR lpWaveHdr;
//   rdpsndAudioDecodePlugin* paudiodecode = (rdpsndAudioDecodePlugin*)dwInstance;
//
//   switch (uMsg)
//   {
//   case MM_WOM_OPEN:
//      WLog_DBG(TAG, "MM_WOM_OPEN");
//      break;
//
//   case MM_WOM_CLOSE:
//      WLog_DBG(TAG, "MM_WOM_CLOSE");
//      break;
//
//   case MM_WOM_DONE:
//   {
//      UINT32 wTimeDelta;
//      lpWaveHdr = (LPWAVEHDR)dwParam1;
//
//      if (!lpWaveHdr)
//         return;
//
//      wave = (RDPSND_WAVE*)lpWaveHdr->dwUser;
//
//      if (!wave)
//         return;
//
//      WLog_DBG(TAG, "MM_WOM_DONE: dwBufferLength: %d cBlockNo: %d",
//               lpWaveHdr->dwBufferLength, wave->cBlockNo);
//      wave->wLocalTimeB = GetTickCount();
//      wTimeDelta = wave->wLocalTimeB - wave->wLocalTimeA;
//      wave->wTimeStampB = wave->wTimeStampA + wTimeDelta;
//
//      paudiodecode->device.WaveConfirm(&(paudiodecode->device), wave);
//
//      free(lpWaveHdr->lpData);
//      free(lpWaveHdr);
//
//      free(wave);
//   }
//   break;
//   }
//}

static BOOL rdpsnd_audio_decode_open(rdpsndDevicePlugin* device, AUDIO_FORMAT* format, int latency)
{

   rdpsndAudioDecodePlugin* paudiodecode = (rdpsndAudioDecodePlugin*)device;

   if (paudiodecode->lpwaveout)
      return TRUE;

   rdpsnd_audio_decode_set_format(device, format, latency);

   freerdp_dsp_context_reset_adpcm(paudiodecode->dsp_context);

   AUDIOFORMAT audioformat;

   memset(&audioformat, 0, sizeof(audioformat));

   audioformat.m_iChannels = format->nChannels;
   audioformat.m_iSamplePerSec = format->nSamplesPerSec;
   audioformat.m_iBitsPerSample = format->wBitsPerSample;

   paudiodecode->lpwaveout = waveout_open(0, &audioformat, rdpsnd_audio_decode_callback_function);


   if (paudiodecode->lpwaveout == NULL)
   {

      WLog_ERR(TAG, "waveOutOpen failed (2)");

      return FALSE;

   }

   waveout_start(paudiodecode->lpwaveout);

   //mmResult = waveOutSetVolume(paudiodecode->hWaveOut, paudiodecode->volume);

   //if (mmResult != MMSYSERR_NOERROR)
   //{
   // WLog_ERR(TAG, "waveOutSetVolume failed: %d", mmResult);
   //return FALSE;
   //}

   return TRUE;

}

static void rdpsnd_audio_decode_close(rdpsndDevicePlugin* device)
{

   rdpsndAudioDecodePlugin* paudiodecode = (rdpsndAudioDecodePlugin*)device;

   if (paudiodecode->lpwaveout)
   {
      //mmResult = waveOutReset(paudiodecode->hWaveOut);

      //mmResult = waveOutClose(paudiodecode->hWaveOut);

      waveout_close(paudiodecode->lpwaveout);

      //if (mmResult != MMSYSERR_NOERROR)
      //{
      //   WLog_ERR(TAG, "waveOutClose failure: %d", mmResult);
      //}

      paudiodecode->lpwaveout = NULL;
   }
}

static void rdpsnd_audio_decode_free(rdpsndDevicePlugin* device)
{
   rdpsndAudioDecodePlugin* paudiodecode = (rdpsndAudioDecodePlugin*)device;

   if (paudiodecode)
   {
      rdpsnd_audio_decode_close(device);

      freerdp_dsp_context_free(paudiodecode->dsp_context);

      free(paudiodecode);
   }
}

static BOOL rdpsnd_audio_decode_format_supported(rdpsndDevicePlugin* device, AUDIO_FORMAT* format)
{
   //MMRESULT result;
   WAVEFORMATEX out;

   if (rdpsnd_audio_decode_convert_format(format, &out))
   {
      //result = waveOutOpen(NULL, WAVE_MAPPER, &out, 0, 0, WAVE_FORMAT_QUERY);

      //if (result == MMSYSERR_NOERROR)
      return TRUE;

   }

   return FALSE;
}

static UINT32 rdpsnd_audio_decode_get_volume(rdpsndDevicePlugin* device)
{
   DWORD dwVolume;
   UINT16 dwVolumeLeft;
   UINT16 dwVolumeRight;

   rdpsndAudioDecodePlugin* paudiodecode = (rdpsndAudioDecodePlugin*)device;

   dwVolumeLeft = ((50 * 0xFFFF) / 100); /* 50% */
   dwVolumeRight = ((50 * 0xFFFF) / 100); /* 50% */
   dwVolume = (dwVolumeLeft << 16) | dwVolumeRight;

   if (!paudiodecode->lpwaveout)
      return dwVolume;

//   waveOutGetVolume(paudiodecode->lpwaveout, &dwVolume);

   return dwVolume;
}

static BOOL rdpsnd_audio_decode_set_volume(rdpsndDevicePlugin* device, UINT32 value)
{
   //rdpsndAudioDecodePlugin* paudiodecode = (rdpsndAudioDecodePlugin*)device;

   //paudiodecode->volume = value;

   //if (!paudiodecode->lpwaveout)
   //   return TRUE;

   //return (waveOutSetVolume(paudiodecode->lpwaveout, value) == MMSYSERR_NOERROR);
   return TRUE;
}

static BOOL rdpsnd_audio_decode_wave_decode(rdpsndDevicePlugin* device, RDPSND_WAVE* wave)
{
   int length;
   BYTE* data;
   rdpsndAudioDecodePlugin* paudiodecode = (rdpsndAudioDecodePlugin*)device;

   if (paudiodecode->wformat == WAVE_FORMAT_ADPCM)
   {
      paudiodecode->dsp_context->decode_ms_adpcm(paudiodecode->dsp_context,
            wave->data, wave->length, paudiodecode->format.nChannels, paudiodecode->block_size);
      length = paudiodecode->dsp_context->adpcm_size;
      data = paudiodecode->dsp_context->adpcm_buffer;
   }
   else if (paudiodecode->wformat == WAVE_FORMAT_DVI_ADPCM)
   {
      paudiodecode->dsp_context->decode_ima_adpcm(paudiodecode->dsp_context,
            wave->data, wave->length, paudiodecode->format.nChannels, paudiodecode->block_size);
      length = paudiodecode->dsp_context->adpcm_size;
      data = paudiodecode->dsp_context->adpcm_buffer;
   }
   else
   {
      length = wave->length;
      data = wave->data;
   }

   wave->data = (BYTE*)malloc(length);
   if (!wave->data)
      return FALSE;
   CopyMemory(wave->data, data, length);
   wave->length = length;

   return TRUE;
}

void rdpsnd_audio_decode_wave_play(rdpsndDevicePlugin* device, RDPSND_WAVE* wave)
{
   //MMRESULT mmResult;
   //LPWAVEHDR lpWaveHdr;
   rdpsndAudioDecodePlugin* paudiodecode = (rdpsndAudioDecodePlugin*)device;

   if (!paudiodecode->lpwaveout)
      return;

   wave->AutoConfirm = TRUE;


   waveout_write(paudiodecode->lpwaveout, wave->data, wave->length);

   //lpWaveHdr = (LPWAVEHDR)malloc(sizeof(WAVEHDR));

   //if (!lpWaveHdr)
   //   return;

   //ZeroMemory(lpWaveHdr, sizeof(WAVEHDR));

   //lpWaveHdr->dwFlags = 0;
   //lpWaveHdr->dwLoops = 0;
   //lpWaveHdr->lpData = (LPSTR)wave->data;
   //lpWaveHdr->dwBufferLength = wave->length;
   //lpWaveHdr->dwUser = (DWORD_PTR)wave;
   //lpWaveHdr->lpNext = NULL;

   //mmResult = waveOutPrepareHeader(paudiodecode->hWaveOut, lpWaveHdr, sizeof(WAVEHDR));

   //if (mmResult != MMSYSERR_NOERROR)
   //{
   //   WLog_ERR(TAG, "waveOutPrepareHeader failure: %d", mmResult);
   //   return;
   //}

   //mmResult = waveOutWrite(paudiodecode->hWaveOut, lpWaveHdr, sizeof(WAVEHDR));

   //if (mmResult != MMSYSERR_NOERROR)
   //{
   //   WLog_ERR(TAG, "waveOutWrite failure: %d", mmResult);
   //   waveOutUnprepareHeader(paudiodecode->hWaveOut, lpWaveHdr, sizeof(WAVEHDR));
   //   free(lpWaveHdr);
   //   return;
   //}
}

static void rdpsnd_audio_decode_start(rdpsndDevicePlugin* device)
{
   rdpsndAudioDecodePlugin* paudiodecode = (rdpsndAudioDecodePlugin*) device;

}

static void rdpsnd_audio_decode_parse_addin_args(rdpsndDevicePlugin* device, ADDIN_ARGV* args)
{

}

//#ifdef BUILTIN_CHANNELS
//#define freerdp_rdpsnd_client_subsystem_entry   audio_decode_freerdp_rdpsnd_client_subsystem_entry
//#else
#define freerdp_rdpsnd_client_subsystem_entry   FREERDP_API freerdp_rdpsnd_client_subsystem_entry
//#endif

/**
* Function description
*
* @return 0 on success, otherwise a Win32 error code
*/
UINT freerdp_rdpsnd_client_subsystem_entry(PFREERDP_RDPSND_DEVICE_ENTRY_POINTS pEntryPoints)
{
   ADDIN_ARGV* args;
   rdpsndAudioDecodePlugin* paudiodecode;

   paudiodecode = (rdpsndAudioDecodePlugin*)calloc(1, sizeof(rdpsndAudioDecodePlugin));

   if (!paudiodecode)
      return CHANNEL_RC_NO_MEMORY;

   paudiodecode->device.DisableConfirmThread = TRUE;

   paudiodecode->device.Open = rdpsnd_audio_decode_open;
   paudiodecode->device.FormatSupported = rdpsnd_audio_decode_format_supported;
   paudiodecode->device.SetFormat = rdpsnd_audio_decode_set_format;
   paudiodecode->device.GetVolume = rdpsnd_audio_decode_get_volume;
   paudiodecode->device.SetVolume = rdpsnd_audio_decode_set_volume;
   paudiodecode->device.WaveDecode = rdpsnd_audio_decode_wave_decode;
   paudiodecode->device.WavePlay = rdpsnd_audio_decode_wave_play;
   paudiodecode->device.Start = rdpsnd_audio_decode_start;
   paudiodecode->device.Close = rdpsnd_audio_decode_close;
   paudiodecode->device.Free = rdpsnd_audio_decode_free;

   args = pEntryPoints->args;
   rdpsnd_audio_decode_parse_addin_args((rdpsndDevicePlugin*)paudiodecode, args);

   paudiodecode->dsp_context = freerdp_dsp_context_new();
   if (!paudiodecode->dsp_context)
   {
      free(paudiodecode);
      return CHANNEL_RC_NO_MEMORY;
   }

   paudiodecode->volume = 0xFFFFFFFF;

   pEntryPoints->pRegisterRdpsndDevice(pEntryPoints->rdpsnd, (rdpsndDevicePlugin*)paudiodecode);

   return CHANNEL_RC_OK;
}
