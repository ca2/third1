
#include <stdio.h>
#include <string.h>

#include "strophe.h"
#include "common.h"
#include "hash.h"

#ifdef _WIN32
#define inline __inline
#endif


xmpp_stanza_t *xmpp_stanza_get_child_by_name_and_ns(xmpp_stanza_t * const stanza,
   const char * const name, const char * const ns)
{
   xmpp_stanza_t *child;

   for (child = stanza->children; child; child = child->next) {
      if (child->type == XMPP_STANZA_TAG &&
         (strcmp(name, xmpp_stanza_get_name(child)) == 0)
         && (strcmp(ns, xmpp_stanza_get_ns(child)) == 0))
         break;
   }

   return child;
}



#ifdef VSNORD

#include <android/log.h>

void xmpp_default_logger(void * const userdata,
   const xmpp_log_level_t level,
   const char * const area,
   const char * const msg)
{
   
   xmpp_log_level_t filter_level = *(xmpp_log_level_t*)userdata;

   if (level >= filter_level)
   {

      int iLevel = ANDROID_LOG_UNKNOWN;

      if (level == XMPP_LEVEL_DEBUG)
      {

         iLevel = ANDROID_LOG_DEBUG;

      }
      else if (level == XMPP_LEVEL_INFO)
      {

         iLevel = ANDROID_LOG_INFO;

      }
      else if (level == XMPP_LEVEL_WARN)
      {

         iLevel = ANDROID_LOG_WARN;

      }
      else if (level == XMPP_LEVEL_ERROR)
      {

         iLevel = ANDROID_LOG_ERROR;

      }

      __android_log_print(iLevel, area, msg);

   }

}


#endif