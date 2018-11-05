#ifndef __CONFIG_H
#define __CONFIG_H

#define FREERDP_DATA_PATH "${FREERDP_DATA_PATH}"
#define FREERDP_KEYMAP_PATH "${FREERDP_KEYMAP_PATH}"
#define FREERDP_PLUGIN_PATH "${FREERDP_PLUGIN_PATH}"

#define FREERDP_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}"

#define FREERDP_LIBRARY_PATH "${FREERDP_LIBRARY_PATH}"

#define FREERDP_ADDIN_PATH "${FREERDP_ADDIN_PATH}"

/* Include files */
#define HAVE_FCNTL_H
//#define HAVE_UNISTD_H
#define HAVE_STDINT_H
#define HAVE_INTTYPES_H
#define HAVE_SYS_MODEM_H
#define HAVE_SYS_FILIO_H
#define HAVE_SYS_SELECT_H
#define HAVE_SYS_STRTIO_H
#define HAVE_EVENTFD_H
#define HAVE_TIMERFD_H
#define HAVE_TM_GMTOFF
#define HAVE_AIO_H
//#define HAVE_POLL_H
//#define HAVE_PTHREAD_GNU_EXT
//#define HAVE_VALGRIND_MEMCHECK_H
#define HAVE_EXECINFO_H

/* Options */
#define WITH_PROFILER
#define WITH_GPROF
#define WITH_SSE2
//#define WITH_NEON
//#define WITH_IPP
//#define WITH_NATIVE_SSPI
#define WITH_JPEG
#define WITH_WIN8
#define WITH_RDPSND_DSOUND

#define WITH_FFMPEG
#define WITH_GSTREAMER_1_0
#define WITH_GSTREAMER_0_10
#define WITH_WINMM
#define WITH_MACAUDIO
#define WITH_ALSA
#define WITH_PULSE
#define WITH_IOSAUDIO
#define WITH_OPENSLES
//#define WITH_GSM

/* Plugins */
#define STATIC_CHANNELS
#define WITH_RDPDR


/* Debug */
#define WITH_DEBUG_CERTIFICATE
#define WITH_DEBUG_CAPABILITIES
#define WITH_DEBUG_CHANNELS
#define WITH_DEBUG_CLIPRDR
#define WITH_DEBUG_DVC
#define WITH_DEBUG_TSMF
#define WITH_DEBUG_GDI
#define WITH_DEBUG_KBD
#define WITH_DEBUG_LICENSE
#define WITH_DEBUG_NEGO
#define WITH_DEBUG_NLA
#define WITH_DEBUG_NTLM
#define WITH_DEBUG_TSG
#define WITH_DEBUG_ORDERS
#define WITH_DEBUG_RAIL
#define WITH_DEBUG_RDP
#define WITH_DEBUG_REDIR
#define WITH_DEBUG_RFX
#define WITH_DEBUG_SCARD
#define WITH_DEBUG_SND
#define WITH_DEBUG_SVC
#define WITH_DEBUG_RDPEI
#define WITH_DEBUG_TIMEZONE
#define WITH_DEBUG_TRANSPORT
#define WITH_DEBUG_WND
#define WITH_DEBUG_X11
#define WITH_DEBUG_X11_CLIPRDR
#define WITH_DEBUG_X11_LOCAL_MOVESIZE
#define WITH_DEBUG_XV
#define WITH_DEBUG_ANDROID_JNI
#endif


#define CMAKE_SHARED_LIBRARY_PREFIX "lib"
