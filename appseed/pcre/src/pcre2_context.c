/*************************************************
*      Perl-Compatible Regular Expressions       *
*************************************************/

/* PCRE is a library of functions to support regular expressions whose syntax
and semantics are as close as possible to those of the Perl 5 language.

                       Written by Philip Hazel
     Original API code Copyright (c) 1997-2012 University of Cambridge
         New API code Copyright (c) 2014 University of Cambridge

-----------------------------------------------------------------------------
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of the University of Cambridge nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
-----------------------------------------------------------------------------
*/


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pcre2_internal.h"



/*************************************************
*          Default malloc/free functions         *
*************************************************/

/* Ignore the "user data" argument in each case. */

static void *default_malloc(size_t size, void *data)
{
(void)data;
return malloc(size);
}


static void default_free(void *block, void *data)
{
(void)data;
free(block);
}



/*************************************************
*        Get a block and save memory control     *
*************************************************/

/* This internal function is called to get a block of memory in which the
memory control data is to be stored at the start for future use.

Arguments:
  size        amount of memory required
  memctl      pointer to a memctl block or NULL

Returns:      pointer to memory or NULL on failure
*/

extern void *
PRIV(memctl_malloc)(size_t size, pcre2_memctl *memctl)
{
pcre2_memctl *newmemctl;
void *yield = (memctl == NULL)? malloc(size) :
  memctl->_alloc(size, memctl->memory_data);
if (yield == NULL) return NULL;
newmemctl = (pcre2_memctl *)yield;
if (memctl == NULL)
  {
  newmemctl->_alloc = default_malloc;
  newmemctl->_free = default_free;
  newmemctl->memory_data = NULL;
  }
else *newmemctl = *memctl;
return yield;
}



/*************************************************
*          Create and initialize contexts        *
*************************************************/

/* Initializing for compile and match contexts is done in separate, private
functions so that these can be called from functions such as pcre2_compile()
when an external context is not supplied. The initializing functions have an
option to set up default memory management. */

PCRE2_EXP_DEFN pcre2_general_context * PCRE2_CALL_CONVENTION
pcre2_general_context_create(void *(*private_malloc)(size_t, void *),
  void (*private_free)(void *, void *), void *memory_data)
{
pcre2_general_context *gcontext;
if (private_malloc == NULL) private_malloc = default_malloc;
if (private_free == NULL) private_free = default_free;
gcontext = private_malloc(sizeof(pcre2_real_general_context), memory_data);
if (gcontext == NULL) return NULL;
gcontext->memctl._alloc = private_malloc;
gcontext->memctl._free = private_free;
gcontext->memctl.memory_data = memory_data;
return gcontext;
}


/* A default compile context is set up to save having to initialize at run time
when no context is supplied to the compile function. */

const pcre2_compile_context PRIV(default_compile_context) = {
  { default_malloc, default_free, NULL },
  NULL,
  NULL,
  PRIV(default_tables),
  BSR_DEFAULT,
  NEWLINE_DEFAULT,
  PARENS_NEST_LIMIT };

/* The create function copies the default into the new memory, but must
override the default memory handling functions if a gcontext was provided. */

PCRE2_EXP_DEFN pcre2_compile_context * PCRE2_CALL_CONVENTION
pcre2_compile_context_create(pcre2_general_context *gcontext)
{
pcre2_compile_context *ccontext = PRIV(memctl_malloc)(
  sizeof(pcre2_real_compile_context), (pcre2_memctl *)gcontext);
if (ccontext == NULL) return NULL;
*ccontext = PRIV(default_compile_context);
if (gcontext != NULL)
  *((pcre2_memctl *)ccontext) = *((pcre2_memctl *)gcontext);
return ccontext;
}


/* A default match context is set up to save having to initialize at run time
when no context is supplied to a match function. */

const pcre2_match_context PRIV(default_match_context) = {
  { default_malloc, default_free, NULL },
#ifdef HEAP_MATCH_RECURSE
  { default_malloc, default_free, NULL },
#endif
#ifdef SUPPORT_JIT
  NULL,
  NULL,
#endif
  NULL,
  NULL,
  MATCH_LIMIT,
  MATCH_LIMIT_RECURSION };

/* The create function copies the default into the new memory, but must
override the default memory handling functions if a gcontext was provided. */

PCRE2_EXP_DEFN pcre2_match_context * PCRE2_CALL_CONVENTION
pcre2_match_context_create(pcre2_general_context *gcontext)
{
pcre2_match_context *mcontext = PRIV(memctl_malloc)(
  sizeof(pcre2_real_match_context), (pcre2_memctl *)gcontext);
if (mcontext == NULL) return NULL;
*mcontext = PRIV(default_match_context);
if (gcontext != NULL)
  *((pcre2_memctl *)mcontext) = *((pcre2_memctl *)gcontext);
return mcontext;
}


/*************************************************
*              Context copy functions            *
*************************************************/

PCRE2_EXP_DEFN pcre2_general_context * PCRE2_CALL_CONVENTION
pcre2_general_context_copy(pcre2_general_context *gcontext)
{
pcre2_general_context *new =
  gcontext->memctl._alloc(sizeof(pcre2_real_general_context),
  gcontext->memctl.memory_data);
if (new == NULL) return NULL;
memcpy(new, gcontext, sizeof(pcre2_real_general_context));
return new;
}


PCRE2_EXP_DEFN pcre2_compile_context * PCRE2_CALL_CONVENTION
pcre2_compile_context_copy(pcre2_compile_context *ccontext)
{
pcre2_compile_context *new =
  ccontext->memctl._alloc(sizeof(pcre2_real_compile_context),
  ccontext->memctl.memory_data);
if (new == NULL) return NULL;
memcpy(new, ccontext, sizeof(pcre2_real_compile_context));
return new;
}


PCRE2_EXP_DEFN pcre2_match_context * PCRE2_CALL_CONVENTION
pcre2_match_context_copy(pcre2_match_context *mcontext)
{
pcre2_match_context *new =
  mcontext->memctl._alloc(sizeof(pcre2_real_match_context),
  mcontext->memctl.memory_data);
if (new == NULL) return NULL;
memcpy(new, mcontext, sizeof(pcre2_real_match_context));
return new;
}



/*************************************************
*              Context free functions            *
*************************************************/


PCRE2_EXP_DEFN void PCRE2_CALL_CONVENTION
pcre2_general_context_free(pcre2_general_context *gcontext)
{
if (gcontext != NULL)
  gcontext->memctl._free(gcontext, gcontext->memctl.memory_data);
}


PCRE2_EXP_DEFN void PCRE2_CALL_CONVENTION
pcre2_compile_context_free(pcre2_compile_context *ccontext)
{
if (ccontext != NULL)
  ccontext->memctl._free(ccontext, ccontext->memctl.memory_data);
}


PCRE2_EXP_DEFN void PCRE2_CALL_CONVENTION
pcre2_match_context_free(pcre2_match_context *mcontext)
{
if (mcontext != NULL)
  mcontext->memctl._free(mcontext, mcontext->memctl.memory_data);
}




/*************************************************
*             Set values in contexts             *
*************************************************/

/* All these functions return 0 for success or PCRE2_ERROR_BADDATA if invalid
data is given. Only some of the functions are able to test the validity of the
data. */


/* ------------ Compile contexts ------------ */

PCRE2_EXP_DEFN int PCRE2_CALL_CONVENTION
pcre2_set_character_tables(pcre2_compile_context *ccontext,
  const unsigned char *tables)
{
ccontext->tables = tables;
return 0;
}

PCRE2_EXP_DEFN int PCRE2_CALL_CONVENTION
pcre2_set_bsr(pcre2_compile_context *ccontext, uint32_t value)
{
switch(value)
  {
  case PCRE2_BSR_ANYCRLF:
  case PCRE2_BSR_UNICODE:
  ccontext->bsr_convention = value;
  return 0;

  default:
  return PCRE2_ERROR_BADDATA;
  }
}

PCRE2_EXP_DEFN int PCRE2_CALL_CONVENTION
pcre2_set_newline(pcre2_compile_context *ccontext, uint32_t newline)
{
switch(newline)
  {
  case PCRE2_NEWLINE_CR:
  case PCRE2_NEWLINE_LF:
  case PCRE2_NEWLINE_CRLF:
  case PCRE2_NEWLINE_ANY:
  case PCRE2_NEWLINE_ANYCRLF:
  ccontext->newline_convention = newline;
  return 0;

  default:
  return PCRE2_ERROR_BADDATA;
  }
}

PCRE2_EXP_DEFN int PCRE2_CALL_CONVENTION
pcre2_set_parens_nest_limit(pcre2_compile_context *ccontext, uint32_t limit)
{
ccontext->parens_nest_limit = limit;
return 0;
}

PCRE2_EXP_DEFN int PCRE2_CALL_CONVENTION
pcre2_set_compile_recursion_guard(pcre2_compile_context *ccontext,
  int (*guard)(uint32_t, void *), void *user_data)
{
ccontext->stack_guard = guard;
ccontext->stack_guard_data = user_data;
return 0;
}


/* ------------ Match contexts ------------ */

PCRE2_EXP_DEFN int PCRE2_CALL_CONVENTION
pcre2_set_callout(pcre2_match_context *mcontext,
  int (*callout)(pcre2_callout_block *, void *), void *callout_data)
{
mcontext->callout = callout;
mcontext->callout_data = callout_data;
return 0;
}

PCRE2_EXP_DEFN int PCRE2_CALL_CONVENTION
pcre2_set_match_limit(pcre2_match_context *mcontext, uint32_t limit)
{
mcontext->match_limit = limit;
return 0;
}

PCRE2_EXP_DEFN int PCRE2_CALL_CONVENTION
pcre2_set_recursion_limit(pcre2_match_context *mcontext, uint32_t limit)
{
mcontext->recursion_limit = limit;
return 0;
}

PCRE2_EXP_DEFN int PCRE2_CALL_CONVENTION
pcre2_set_recursion_memory_management(pcre2_match_context *mcontext,
  void *(*mymalloc)(size_t, void *), void (*myfree)(void *, void *),
  void *mydata)
{
#ifdef HEAP_MATCH_RECURSE
mcontext->stack_memctl._alloc = mymalloc;
mcontext->stack_memctl._free = myfree;
mcontext->stack_memctl.memory_data = mydata;
#else
(void)mcontext;
(void)mymalloc;
(void)myfree;
(void)mydata;
#endif
return 0;
}

/* End of pcre2_context.c */
