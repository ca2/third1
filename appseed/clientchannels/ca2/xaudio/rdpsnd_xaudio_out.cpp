
#include "rdpsnd_xaudio_out.h"

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


#define MM_WOM_OPEN         0x3BB           /* waveform output */
#define MM_WOM_CLOSE        0x3BC
#define MM_WOM_DONE         0x3BD
#define MMRESULT int

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

xaudio_wave_out::xaudio_wave_out()
{
   m_pcallback          = NULL;
   m_instance           = 0;
   m_pxaudio            = NULL;
   m_pvoice             = NULL;
   m_psourcevoice       = NULL;
   m_iBufferedCount     = 0;
   m_estate             = state_initial;
   m_mmr = 0;
   hsem = CreateSemaphore(NULL,64,64,NULL);
}

xaudio_wave_out::~xaudio_wave_out()
{
   ::CloseHandle(hsem);
}





int xaudio_wave_out::wave_out_open_ex(int32_t iBufferCount,int32_t iBufferSampleCount,uint32_t uiSamplesPerSec,uint32_t uiChannelCount,uint32_t uiBitsPerSample)
{

   if(m_pxaudio != NULL && m_pvoice != NULL && m_psourcevoice != NULL && m_estate != state_initial)
      return MMSYSERR_ERROR;

   m_iBuffer = 0;

   int mmr;
   assert(m_pxaudio == NULL);
   assert(m_pvoice == NULL);
   assert(m_psourcevoice == NULL);
   assert(m_estate == state_initial);

   CoInitializeEx(nullptr,COINIT_MULTITHREADED);

   // by (indirect casey)
   if(FAILED(XAudio2Create(&m_pxaudio,0,XAUDIO2_DEFAULT_PROCESSOR)))
   {

      return MMSYSERR_ERROR;

   }

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG)
   // To see the trace output, you need to view ETW logs for this application:
   //    Go to Control Panel, Administrative Tools, Event Viewer.
   //    View->Show Analytic and Debug Logs.
   //    Applications and Services Logs / Microsoft / Windows / XAudio2.
   //    Right click on Microsoft Windows XAudio2 debug logging, Properties, then Enable Logging, and hit OK
   XAUDIO2_DEBUG_CONFIGURATION debug ={0};
   debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
   debug.BreakMask = XAUDIO2_LOG_ERRORS;
   m_pxaudio->SetDebugConfiguration(&debug,0);
#endif


   HRESULT hr;

   if(FAILED(hr = m_pxaudio->CreateMasteringVoice(&m_pvoice,uiChannelCount,uiSamplesPerSec)))
   {
      return MMSYSERR_ERROR;
   }

   mmr = MMSYSERR_NOERROR;
   m_waveformatex.wFormatTag = WAVE_FORMAT_PCM;
   m_waveformatex.nChannels = uiChannelCount;
   m_waveformatex.nSamplesPerSec = uiSamplesPerSec;
   m_waveformatex.wBitsPerSample = sizeof(short) * 8;
   m_waveformatex.nBlockAlign = m_waveformatex.wBitsPerSample * m_waveformatex.nChannels / 8;
   m_waveformatex.nAvgBytesPerSec = m_waveformatex.nSamplesPerSec * m_waveformatex.nBlockAlign;
   m_waveformatex.cbSize = 0;

   if(FAILED(hr = m_pxaudio->CreateSourceVoice(&m_psourcevoice,&m_waveformatex,XAUDIO2_VOICE_NOSRC | XAUDIO2_VOICE_NOPITCH,1.0f,this)))
   {
      return MMSYSERR_ERROR;
   }

   if(mmr != MMSYSERR_NOERROR)
   {

      return mmr;

   }

Opened:
   int iAlign = 2048;
   iBufferCount = 4;
   iBufferSampleCount = (1 << 10);

   uint32_t uiBufferSize = iBufferSampleCount * m_waveformatex.nChannels * 2;

   //assert((uiBufferSize % 2048) == 0);// Streaming size must be 2K aligned to use for async I/O






   m_estate = state_opened;

   return MMSYSERR_NOERROR;

}



int xaudio_wave_out::wave_out_close()
{

   //single_lock sLock(&m_mutex,TRUE);

   if(m_estate == state_playing)
   {
      wave_out_stop();
   }

   if(m_estate != state_opened)
      return MMSYSERR_NOERROR;

   int mmr;

   int32_t i,iSize;

   //iSize =  wave_out_get_buffer()->GetBufferCount();

   //for(i = 0; i < iSize; i++)
   //{

   mmr = SUCCEEDED(m_psourcevoice->Stop()) ? MMSYSERR_NOERROR : MMSYSERR_ERROR;

   //for(i = 0; i < iSize; i++)
   //{

   //   delete m_bufferptra[i];

   //}


   //m_psourcevoice->DestroyVoice();

   try
   {

      m_psourcevoice = nullptr;

   }
   catch(...)
   {

   }

   //m_bufferptra.remove_all();


   if(m_pvoice != nullptr)
   {

      //m_pvoice->DestroyVoice();

      m_pvoice = nullptr;

   }


   if(m_pxaudio != nullptr)
   {

      m_pxaudio = nullptr;

   }


   //mmr = xaudio::translate(waveOutClose(m_hwaveout));

   //m_hwaveout = NULL;

//   m_pprebuffer->Reset();

   m_estate = state_initial;

   return MMSYSERR_NOERROR;

}



//void xaudio_wave_out::wave_out_buffer_ready(int iBuffer)
//{
//
//   if(m_estate != state_playing)
//   {
//      //TRACE("ERROR xaudio_wave_out::BufferReady while wave_out_get_state() != state_playing");
//      return;
//   }
//
//   //::multimedia::audio::wave_buffer * pwbuffer = wave_out_get_buffer();
//   //::multimedia::audio::wave_buffer::buffer * pbuffer = pwbuffer->get_buffer(iBuffer);
//
//   //int mmr;
//   //if(m_peffect != NULL)
//   //{
//   //   m_peffect->Process16bits((int16_t *)pbuffer->m_pData,pwbuffer->m_uiBufferSize / 2);
//   //}
//
//   //XAUDIO2_BUFFER b;
//   //ZERO(b);
//   //b.pContext = pbuffer;
//   //b.AudioBytes = pwbuffer->m_uiBufferSize;
//   //b.pAudioData = (const BYTE *)pbuffer->m_pData;
//
//   ////single_lock sLock(&m_mutex,TRUE);
//
//
//
//   //mmr = xaudio::translate(m_psourcevoice->SubmitSourceBuffer(&b));
//
//   //VERIFY(MMSYSERR_NOERROR == mmr);
//
//   //if(mmr == MMSYSERR_NOERROR)
//   //{
//
//   //   m_iBufferedCount++;
//
//   //}
//
//}




int xaudio_wave_out::wave_out_stop()
{

   //single_lock sLock(&m_mutex,TRUE);

   if(m_estate != state_playing && m_estate != state_paused)
      return MMSYSERR_ERROR;

   //m_eventStopped.ResetEvent();

   //m_pprebuffer->Stop();

   m_estate = state_stopping;

   //// waveOutReset
   //// The waveOutReset function stops playback on the given
   //// waveform-audio_xaudio output device and resets the current position
   //// to zero. All pending playback buffers are marked as done and
   //// returned to the application.
   m_mmr = SUCCEEDED(m_psourcevoice->Stop()) ? MMSYSERR_NOERROR : MMSYSERR_ERROR;

   if(m_mmr == MMSYSERR_NOERROR)
   {

      m_estate = state_opened;

   }

   return m_mmr;

}


int xaudio_wave_out::wave_out_pause()
{

//   single_lock sLock(&m_mutex,TRUE);

   assert(m_estate == state_playing);

   if(m_estate != state_playing)
      return MMSYSERR_ERROR;

   // waveOutReset
   // The waveOutReset function stops playback on the given
   // waveform-audio_xaudio output device and resets the current position
   // to zero. All pending playback buffers are marked as done and
   // returned to the application.

   m_mmr = SUCCEEDED(m_psourcevoice->Stop()) ? MMSYSERR_NOERROR : MMSYSERR_ERROR;

   assert(m_mmr == MMSYSERR_NOERROR);

   if(m_mmr == MMSYSERR_NOERROR)
   {
      m_estate = state_paused;
   }

   return m_mmr;

}

int xaudio_wave_out::wave_out_start()
{

   if(m_estate == state_playing)
      return MMSYSERR_NOERROR;

   assert(m_estate == state_opened || m_estate == state_stopped);

   m_estate = state_playing;


   m_mmr = SUCCEEDED(m_psourcevoice->Start(0,XAUDIO2_COMMIT_NOW)) ? MMSYSERR_NOERROR : MMSYSERR_ERROR;

   //         m_prunstepthread = new run_step_thread(this);


   return MMSYSERR_NOERROR;

}


int xaudio_wave_out::wave_out_restart()
{

   assert(m_estate == state_paused);

   if(m_estate != state_paused)
      return MMSYSERR_ERROR;

   // waveOutReset
   // The waveOutReset function stops playback on the given
   // waveform-audio_xaudio output device and resets the current position
   // to zero. All pending playback buffers are marked as done and
   // returned to the application.
   m_mmr = SUCCEEDED(m_psourcevoice->Start(0,XAUDIO2_COMMIT_NOW)) ? MMSYSERR_NOERROR : MMSYSERR_ERROR;

   assert(m_mmr == MMSYSERR_NOERROR);

   if(m_mmr == MMSYSERR_NOERROR)
   {

      m_estate = state_playing;

   }

   return m_mmr;

}


/*imedia_time xaudio_wave_out::GetPositionMillisForSynch()
{
int64_t dwMillis = GetPositionMillis();
int64_t dwPosition = m_pprebuffer->m_position * 8;
dwPosition /= m_waveformatex.wBitsPerSample;
dwPosition *= 1000;
dwPosition /= m_waveformatex.nChannels * m_waveformatex.nSamplesPerSec;
if(m_pprebuffer != NULL && m_pprebuffer->m_pdecoder != NULL)
return dwMillis + dwPosition - m_pprebuffer->m_pdecoder->DecoderGetLostMillis(dwMillis + dwPosition) - (((int64_t) m_dwLostSampleCount) /  ((int64_t) m_waveformatex.nSamplesPerSec));
else
return dwMillis + dwPosition - ((m_dwLostSampleCount) * 1000 / m_waveformatex.nSamplesPerSec);
}*/

/*imedia_position xaudio_wave_out::get_position_for_synch()
{
imedia_position position = get_position();
if(m_pprebuffer != NULL && m_pprebuffer->m_pdecoder != NULL)
return m_pprebuffer->m_position + position - m_pprebuffer->m_pdecoder->DecoderGetLostPositionOffset(position) - m_dwLostSampleCount * m_waveformatex.wBitsPerSample * m_waveformatex.nChannels / 8;
else
return m_pprebuffer->m_position + position - m_dwLostSampleCount * m_waveformatex.wBitsPerSample * m_waveformatex.nChannels / 8;
}*/


//void xaudio_wave_out::wave_out_free(int iBuffer)
//{
//
//   //wave_out_free(wave_hdr(iBuffer));
//
//
//}


//void xaudio_wave_out::wave_out_on_playback_end()
//{
//
//   wave_out_stop();
//
//   if(m_pprebuffer->m_pstreameffectOut != NULL)
//   {
//      ::multimedia::iaudio::wave_stream_effect * peffect = m_pprebuffer->m_pstreameffectOut;
//      m_pprebuffer->m_pstreameffectOut = NULL;
//      delete peffect;
//   }
//
//   m_eventStopped.SetEvent();
//
//   m_pplayer->OnEvent(::multimedia::audio::wave_player::EventPlaybackEnd);
//
//}

//int32_t xaudio_wave_out::wave_out_get_buffered_buffer_count()
//{
//
//   return ::multimedia::audio::xaudio_wave_out::wave_out_get_buffered_buffer_count();
//
//}


//WAVEFORMATEX * xaudio_wave_out::wave_format()
//{
//
//   xaudio::translate(m_waveformatex,m_pwaveformat);
//
//   return &m_waveformatex;
//
//}


//void * xaudio_wave_out::get_os_data()
//{
//   return m_psourcevoice;
//}
//
//
//void xaudio_wave_out::wave_out_run_step()
//{
//
//   int iPlay =  -1;
//
//
//}
//



//
// Callback handlers, only implement the buffer events for maintaining play state
//
void xaudio_wave_out::OnVoiceProcessingPassStart(UINT32 /*bytesRequired*/)
{
}
void xaudio_wave_out::OnVoiceProcessingPassEnd()
{
}

void xaudio_wave_out::OnBufferStart(void* pBufferContext)
{

   //::multimedia::audio::wave_buffer::buffer * pbuffer = (::multimedia::audio::wave_buffer::buffer *)pBufferContext;

   //pbuffer->m_bIsPlaying =  true;

}

void xaudio_wave_out::OnBufferEnd(void* pBufferContext)
{
   ::LPWAVEHDR phdr = (::LPWAVEHDR)pBufferContext;
   m_iBufferedCount--;
   ReleaseSemaphore(hsem, 1, NULL);

   (*m_pcallback)((HWAVEOUT) this,MM_WOM_DONE,m_instance,(DWORD_PTR)pBufferContext,0);

}


void xaudio_wave_out::OnLoopEnd(void* /*bufferContext*/)
{
}
void xaudio_wave_out::OnVoiceError(void* /*bufferContext*/,HRESULT /*error*/)
{
}


extern "C"
{


   MMRESULT waveOutOpen(
      LPHWAVEOUT     phwo,
      UINT_PTR       uDeviceID,
      LPWAVEFORMATEX pwfx,
      DWORD_PTR      dwCallback,
      DWORD_PTR      dwCallbackInstance,
      DWORD          fdwOpen
      )
   {
      *phwo = new xaudio_wave_out();
      (*phwo)->m_pcallback = (pfn_xaudio_callback_function) dwCallback;
      (*phwo)->m_instance = dwCallbackInstance;


      return (*phwo)->wave_out_open_ex(0,0,pwfx->nSamplesPerSec,
         pwfx->nChannels,
         pwfx->wBitsPerSample);

   }

   MMRESULT waveOutWrite(
      HWAVEOUT  hwo,
      LPWAVEHDR pwh,
      UINT      cbwh
      )
   {
         XAUDIO2_BUFFER b;
         memset(&b, 0, sizeof(b));
         b.pContext = pwh;
         b.AudioBytes = pwh->dwBufferLength;
         b.pAudioData = (const BYTE *)pwh->lpData;
      
      
         if(hwo->m_estate != xaudio_wave_out::state_playing)
         {
            hwo->wave_out_start();
            

         }

         WaitForSingleObject(hwo->hsem,INFINITE);
         try
         {

            if(hwo->m_iBufferedCount < 64)
            {

               hwo->m_iBufferedCount++;

               int mmr = SUCCEEDED(hwo->m_psourcevoice->SubmitSourceBuffer(&b)) ? MMSYSERR_NOERROR : MMSYSERR_ERROR;


            }
            else
            {
            }
         }
         catch(...)
         {
         }


      return MMSYSERR_NOERROR;
   }

   MMRESULT waveOutReset(HWAVEOUT h)
   {
      return MMSYSERR_NOERROR;
   }
   MMRESULT waveOutClose(HWAVEOUT h)
   {
      return h->wave_out_close();
   }

} // extern "C"



