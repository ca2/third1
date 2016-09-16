#pragma once

#include "app/appseed/ace/_ace.h"
//#define WINVER _WIN32_WINNT_WIN8
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


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


#include <xaudio2.h>

#define MM_WOM_OPEN         0x3BB           /* waveform output */
#define MM_WOM_CLOSE        0x3BC
#define MM_WOM_DONE         0x3BD

typedef struct xaudio_wave_out * HWAVEOUT;
typedef HWAVEOUT * LPHWAVEOUT;

extern "C"
typedef void  CALLBACK xaudio_callback_function(HWAVEOUT hwo,UINT uMsg,DWORD_PTR dwInstance,DWORD_PTR dwParam1,DWORD_PTR dwParam2);
typedef xaudio_callback_function *  pfn_xaudio_callback_function;

struct xaudio_wave_out:
   virtual public IXAudio2VoiceCallback
{
public:

   enum  e_state
   {
      state_initial,
      state_opened,
      state_playing,
      state_stopping,
      state_stopped,
      state_paused,
   };
   pfn_xaudio_callback_function m_pcallback;
   DWORD_PTR m_instance;
   int m_mmr;
   //HANDLE streamEndEventHandle;
   //VoiceCallback(): streamEndEventHandle(CreateEvent(NULL,FALSE,FALSE,NULL)){}
   //~VoiceCallback()
   //{
   //CloseHandle(streamEndEventHandle);
   //}

   HANDLE hsem;

   // Called when the voice has just finished playing a contiguous audio stream.
   STDMETHOD_(void,OnStreamEnd())
   {

   }

   // Unused methods in this application
   STDMETHOD_(void,OnVoiceProcessingPassEnd());
   STDMETHOD_(void,OnVoiceProcessingPassStart(UINT32 SamplesRequired)) ;
   STDMETHOD_(void,OnBufferEnd(void * pBufferContext))  ;
   STDMETHOD_(void,OnBufferStart(void * pBufferContext));
   STDMETHOD_(void,OnLoopEnd(void * pBufferContext));
   STDMETHOD_(void,OnVoiceError(void * pBufferContext,HRESULT Error));
   e_state m_estate;
   IXAudio2 *                       m_pxaudio;
   IXAudio2MasteringVoice *         m_pvoice;
   IXAudio2SourceVoice *            m_psourcevoice;

   int                              m_iBuffer;

   WAVEFORMATEX                     m_waveformatex;

   int                              m_iBufferedCount;


   xaudio_wave_out();
   virtual ~xaudio_wave_out();


   int wave_out_start();

   virtual int wave_out_open_ex(int32_t iBufferCount,int32_t iBufferSampleCount,uint32_t uiSamplesPerSec,uint32_t uiChannelCount,uint32_t uiBitsPerSample);
   virtual int wave_out_stop();
   virtual int wave_out_close();
   virtual int wave_out_pause();
   virtual int wave_out_restart();


   //WAVEFORMATEX * wave_format();
};

