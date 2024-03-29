/**
 * FreeRDP: A Remote Desktop Protocol Implementation
 * RPC Fault Handling
 *
 * Copyright 2012 Marc-Andre Moreau <marcandre.moreau@gmail.com>
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

#include <freerdp/log.h>

#include "rpc_fault.h"
#ifdef _UWP
#include <rpc.h>
#endif

#define TAG FREERDP_TAG("core.gateway.rpc")

static const RPC_FAULT_CODE RPC_FAULT_CODES[] =
{
   DEFINE_RPC_FAULT_CODE(nca_s_fault_object_not_found)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_cancel)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_addr_error)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_context_mismatch)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_fp_div_zero)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_fp_error)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_fp_overflow)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_fp_underflow)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_ill_inst)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_int_div_by_zero)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_int_overflow)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_invalid_bound)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_invalid_tag)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_pipe_closed)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_pipe_comm_error)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_pipe_discipline)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_pipe_empty)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_pipe_memory)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_pipe_order)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_remote_no_memory)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_user_defined)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_tx_open_failed)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_codeset_conv_error)
   DEFINE_RPC_FAULT_CODE(nca_s_fault_no_client_stub)
   DEFINE_RPC_FAULT_CODE(RPC_S_INVALID_STRING_BINDING)
   DEFINE_RPC_FAULT_CODE(RPC_S_WRONG_KIND_OF_BINDING)
   DEFINE_RPC_FAULT_CODE(RPC_S_INVALID_BINDING)
   DEFINE_RPC_FAULT_CODE(RPC_S_PROTSEQ_NOT_SUPPORTED)
   DEFINE_RPC_FAULT_CODE(RPC_S_INVALID_RPC_PROTSEQ)
   DEFINE_RPC_FAULT_CODE(RPC_S_INVALID_STRING_UUID)
   DEFINE_RPC_FAULT_CODE(RPC_S_INVALID_ENDPOINT_FORMAT)
   DEFINE_RPC_FAULT_CODE(RPC_S_INVALID_NET_ADDR)
   DEFINE_RPC_FAULT_CODE(RPC_S_NO_ENDPOINT_FOUND)
   DEFINE_RPC_FAULT_CODE(RPC_S_INVALID_TIMEOUT)
   DEFINE_RPC_FAULT_CODE(RPC_S_OBJECT_NOT_FOUND)
   DEFINE_RPC_FAULT_CODE(RPC_S_ALREADY_REGISTERED)
   DEFINE_RPC_FAULT_CODE(RPC_S_TYPE_ALREADY_REGISTERED)
   DEFINE_RPC_FAULT_CODE(RPC_S_ALREADY_LISTENING)
   DEFINE_RPC_FAULT_CODE(RPC_S_NO_PROTSEQS_REGISTERED)
   DEFINE_RPC_FAULT_CODE(RPC_S_NOT_LISTENING)
   DEFINE_RPC_FAULT_CODE(RPC_S_UNKNOWN_MGR_TYPE)
   DEFINE_RPC_FAULT_CODE(RPC_S_UNKNOWN_IF)
   DEFINE_RPC_FAULT_CODE(RPC_S_NO_BINDINGS)
   DEFINE_RPC_FAULT_CODE(RPC_S_NO_PROTSEQS)
   DEFINE_RPC_FAULT_CODE(RPC_S_CANT_CREATE_ENDPOINT)
   DEFINE_RPC_FAULT_CODE(RPC_S_OUT_OF_RESOURCES)
   DEFINE_RPC_FAULT_CODE(RPC_S_SERVER_UNAVAILABLE)
   DEFINE_RPC_FAULT_CODE(RPC_S_SERVER_TOO_BUSY)
   DEFINE_RPC_FAULT_CODE(RPC_S_INVALID_NETWORK_OPTIONS)
   DEFINE_RPC_FAULT_CODE(RPC_S_NO_CALL_ACTIVE)
   DEFINE_RPC_FAULT_CODE(RPC_S_CALL_FAILED)
   DEFINE_RPC_FAULT_CODE(RPC_S_CALL_FAILED_DNE)
   DEFINE_RPC_FAULT_CODE(RPC_S_PROTOCOL_ERROR)
   DEFINE_RPC_FAULT_CODE(RPC_S_PROXY_ACCESS_DENIED)
   DEFINE_RPC_FAULT_CODE(RPC_S_UNSUPPORTED_TRANS_SYN)
   DEFINE_RPC_FAULT_CODE(RPC_S_UNSUPPORTED_TYPE)
   DEFINE_RPC_FAULT_CODE(RPC_S_INVALID_TAG)
   DEFINE_RPC_FAULT_CODE(RPC_S_INVALID_BOUND)
   DEFINE_RPC_FAULT_CODE(RPC_S_NO_ENTRY_NAME)
   DEFINE_RPC_FAULT_CODE(RPC_S_INVALID_NAME_SYNTAX)
   DEFINE_RPC_FAULT_CODE(RPC_S_UNSUPPORTED_NAME_SYNTAX)
   DEFINE_RPC_FAULT_CODE(RPC_S_UUID_NO_ADDRESS)
   DEFINE_RPC_FAULT_CODE(RPC_S_DUPLICATE_ENDPOINT)
   DEFINE_RPC_FAULT_CODE(RPC_S_UNKNOWN_AUTHN_TYPE)
   DEFINE_RPC_FAULT_CODE(RPC_S_MAX_CALLS_TOO_SMALL)
   DEFINE_RPC_FAULT_CODE(RPC_S_STRING_TOO_LONG)
   DEFINE_RPC_FAULT_CODE(RPC_S_PROTSEQ_NOT_FOUND)
   DEFINE_RPC_FAULT_CODE(RPC_S_PROCNUM_OUT_OF_RANGE)
   DEFINE_RPC_FAULT_CODE(RPC_S_BINDING_HAS_NO_AUTH)
   DEFINE_RPC_FAULT_CODE(RPC_S_UNKNOWN_AUTHN_SERVICE)
   DEFINE_RPC_FAULT_CODE(RPC_S_UNKNOWN_AUTHN_LEVEL)
   DEFINE_RPC_FAULT_CODE(RPC_S_INVALID_AUTH_IDENTITY)
   DEFINE_RPC_FAULT_CODE(RPC_S_UNKNOWN_AUTHZ_SERVICE)
   DEFINE_RPC_FAULT_CODE(EPT_S_INVALID_ENTRY)
   DEFINE_RPC_FAULT_CODE(EPT_S_CANT_PERFORM_OP)
   DEFINE_RPC_FAULT_CODE(EPT_S_NOT_REGISTERED)
   DEFINE_RPC_FAULT_CODE(RPC_S_NOTHING_TO_EXPORT)
   DEFINE_RPC_FAULT_CODE(RPC_S_INCOMPLETE_NAME)
   DEFINE_RPC_FAULT_CODE(RPC_S_INVALID_VERS_OPTION)
   DEFINE_RPC_FAULT_CODE(RPC_S_NO_MORE_MEMBERS)
   DEFINE_RPC_FAULT_CODE(RPC_S_NOT_ALL_OBJS_UNEXPORTED)
   DEFINE_RPC_FAULT_CODE(RPC_S_INTERFACE_NOT_FOUND)
   DEFINE_RPC_FAULT_CODE(RPC_S_ENTRY_ALREADY_EXISTS)
   DEFINE_RPC_FAULT_CODE(RPC_S_ENTRY_NOT_FOUND)
   DEFINE_RPC_FAULT_CODE(RPC_S_NAME_SERVICE_UNAVAILABLE)
   DEFINE_RPC_FAULT_CODE(RPC_S_INVALID_NAF_ID)
   DEFINE_RPC_FAULT_CODE(RPC_S_CANNOT_SUPPORT)
   DEFINE_RPC_FAULT_CODE(RPC_S_NO_CONTEXT_AVAILABLE)
   DEFINE_RPC_FAULT_CODE(RPC_S_INTERNAL_ERROR)
   DEFINE_RPC_FAULT_CODE(RPC_S_ZERO_DIVIDE)
   DEFINE_RPC_FAULT_CODE(RPC_S_ADDRESS_ERROR)
   DEFINE_RPC_FAULT_CODE(RPC_S_FP_DIV_ZERO)
   DEFINE_RPC_FAULT_CODE(RPC_S_FP_UNDERFLOW)
   DEFINE_RPC_FAULT_CODE(RPC_S_FP_OVERFLOW)
   DEFINE_RPC_FAULT_CODE(RPC_X_NO_MORE_ENTRIES)
   DEFINE_RPC_FAULT_CODE(RPC_X_SS_CHAR_TRANS_OPEN_FAIL)
   DEFINE_RPC_FAULT_CODE(RPC_X_SS_CHAR_TRANS_SHORT_FILE)
   DEFINE_RPC_FAULT_CODE(RPC_X_SS_IN_NULL_CONTEXT)
   DEFINE_RPC_FAULT_CODE(RPC_X_SS_CONTEXT_DAMAGED)
   DEFINE_RPC_FAULT_CODE(RPC_X_SS_HANDLES_MISMATCH)
   DEFINE_RPC_FAULT_CODE(RPC_X_SS_CANNOT_GET_CALL_HANDLE)
   DEFINE_RPC_FAULT_CODE(RPC_X_NULL_REF_POINTER)
   DEFINE_RPC_FAULT_CODE(RPC_X_ENUM_VALUE_OUT_OF_RANGE)
   DEFINE_RPC_FAULT_CODE(RPC_X_BYTE_COUNT_TOO_SMALL)
   DEFINE_RPC_FAULT_CODE(RPC_X_BAD_STUB_DATA)
   DEFINE_RPC_FAULT_CODE(RPC_S_NO_INTERFACES)
   DEFINE_RPC_FAULT_CODE(RPC_S_CALL_CANCELLED)
   DEFINE_RPC_FAULT_CODE(RPC_S_BINDING_INCOMPLETE)
   DEFINE_RPC_FAULT_CODE(RPC_S_COMM_FAILURE)
   DEFINE_RPC_FAULT_CODE(RPC_S_UNSUPPORTED_AUTHN_LEVEL)
   DEFINE_RPC_FAULT_CODE(RPC_S_NO_PRINC_NAME)
   DEFINE_RPC_FAULT_CODE(RPC_S_NOT_RPC_ERROR)
   DEFINE_RPC_FAULT_CODE(RPC_S_UUID_LOCAL_ONLY)
   DEFINE_RPC_FAULT_CODE(RPC_S_SEC_PKG_ERROR)
   DEFINE_RPC_FAULT_CODE(RPC_S_NOT_CANCELLED)
   DEFINE_RPC_FAULT_CODE(RPC_X_INVALID_ES_ACTION)
   DEFINE_RPC_FAULT_CODE(RPC_X_WRONG_ES_VERSION)
   DEFINE_RPC_FAULT_CODE(RPC_X_WRONG_STUB_VERSION)
   DEFINE_RPC_FAULT_CODE(RPC_X_INVALID_PIPE_OBJECT)
   DEFINE_RPC_FAULT_CODE(RPC_X_WRONG_PIPE_ORDER)
   DEFINE_RPC_FAULT_CODE(RPC_X_WRONG_PIPE_VERSION)
   DEFINE_RPC_FAULT_CODE(RPC_S_COOKIE_AUTH_FAILED)
   DEFINE_RPC_FAULT_CODE(RPC_S_GROUP_MEMBER_NOT_FOUND)
   DEFINE_RPC_FAULT_CODE(EPT_S_CANT_CREATE)
   DEFINE_RPC_FAULT_CODE(RPC_S_INVALID_OBJECT)
   {
      0, NULL
   }
};

static const RPC_FAULT_CODE RPC_TSG_FAULT_CODES[] =
{
   DEFINE_RPC_FAULT_CODE(RPC_S_OK)
   DEFINE_RPC_FAULT_CODE(RPC_S_INVALID_ARG)
   DEFINE_RPC_FAULT_CODE(RPC_S_OUT_OF_MEMORY)
   DEFINE_RPC_FAULT_CODE(RPC_S_OUT_OF_THREADS)
   DEFINE_RPC_FAULT_CODE(RPC_S_INVALID_LEVEL)
   DEFINE_RPC_FAULT_CODE(RPC_S_BUFFER_TOO_SMALL)
   DEFINE_RPC_FAULT_CODE(RPC_S_INVALID_SECURITY_DESC)
   DEFINE_RPC_FAULT_CODE(RPC_S_ACCESS_DENIED)
   DEFINE_RPC_FAULT_CODE(RPC_S_SERVER_OUT_OF_MEMORY)
   DEFINE_RPC_FAULT_CODE(RPC_S_ASYNC_CALL_PENDING)
   DEFINE_RPC_FAULT_CODE(RPC_S_UNKNOWN_PRINCIPAL)
   DEFINE_RPC_FAULT_CODE(RPC_S_TIMEOUT)
   DEFINE_RPC_FAULT_CODE(RPC_X_NO_MEMORY)
   DEFINE_RPC_FAULT_CODE(RPC_X_INVALID_BOUND)
   DEFINE_RPC_FAULT_CODE(RPC_X_INVALID_TAG)
   DEFINE_RPC_FAULT_CODE(RPC_X_ENUM_VALUE_TOO_LARGE)
   DEFINE_RPC_FAULT_CODE(RPC_X_SS_CONTEXT_MISMATCH)
   DEFINE_RPC_FAULT_CODE(RPC_X_INVALID_BUFFER)
   DEFINE_RPC_FAULT_CODE(RPC_X_PIPE_APP_MEMORY)
   DEFINE_RPC_FAULT_CODE(RPC_X_INVALID_PIPE_OPERATION)
   DEFINE_RPC_FAULT_CODE(ERROR_ONLY_IF_CONNECTED)
   DEFINE_RPC_FAULT_CODE(ERROR_GRACEFUL_DISCONNECT)
   DEFINE_RPC_FAULT_CODE(ERROR_OPERATION_ABORTED)
   DEFINE_RPC_FAULT_CODE(ERROR_BAD_ARGUMENTS)
   DEFINE_RPC_FAULT_CODE(E_PROXY_INTERNALERROR)
   DEFINE_RPC_FAULT_CODE(E_PROXY_RAP_ACCESSDENIED)
   DEFINE_RPC_FAULT_CODE(E_PROXY_NAP_ACCESSDENIED)
   DEFINE_RPC_FAULT_CODE(E_PROXY_TS_CONNECTFAILED)
   DEFINE_RPC_FAULT_CODE(E_PROXY_ALREADYDISCONNECTED)
   DEFINE_RPC_FAULT_CODE(E_PROXY_QUARANTINE_ACCESSDENIED)
   DEFINE_RPC_FAULT_CODE(E_PROXY_NOCERTAVAILABLE)
   DEFINE_RPC_FAULT_CODE(E_PROXY_COOKIE_BADPACKET)
   DEFINE_RPC_FAULT_CODE(E_PROXY_COOKIE_AUTHENTICATION_ACCESS_DENIED)
   DEFINE_RPC_FAULT_CODE(E_PROXY_UNSUPPORTED_AUTHENTICATION_METHOD)
   DEFINE_RPC_FAULT_CODE(E_PROXY_CAPABILITYMISMATCH)
   DEFINE_RPC_FAULT_CODE(HRESULT_CODE(E_PROXY_NOTSUPPORTED))
   DEFINE_RPC_FAULT_CODE(HRESULT_CODE(E_PROXY_TS_CONNECTFAILED))
   DEFINE_RPC_FAULT_CODE(HRESULT_CODE(E_PROXY_MAXCONNECTIONSREACHED))
   DEFINE_RPC_FAULT_CODE(HRESULT_CODE(E_PROXY_INTERNALERROR))
   DEFINE_RPC_FAULT_CODE(HRESULT_CODE(E_PROXY_SESSIONTIMEOUT))
   DEFINE_RPC_FAULT_CODE(HRESULT_CODE(E_PROXY_REAUTH_AUTHN_FAILED))
   DEFINE_RPC_FAULT_CODE(HRESULT_CODE(E_PROXY_REAUTH_CAP_FAILED))
   DEFINE_RPC_FAULT_CODE(HRESULT_CODE(E_PROXY_REAUTH_RAP_FAILED))
   DEFINE_RPC_FAULT_CODE(HRESULT_CODE(E_PROXY_SDR_NOT_SUPPORTED_BY_TS))
   DEFINE_RPC_FAULT_CODE(HRESULT_CODE(E_PROXY_REAUTH_NAP_FAILED))
   DEFINE_RPC_FAULT_CODE(HRESULT_CODE(E_PROXY_CONNECTIONABORTED))
   DEFINE_RPC_FAULT_CODE(HRESULT_CODE(E_PROXY_NOCERTAVAILABLE))
   DEFINE_RPC_FAULT_CODE(__HRESULT_FROM_WIN32(RPC_S_CALL_CANCELLED))
   {
      0, NULL
   }
};

/**
 * [MS-RPCE] 3.1.1.5.5 Returning Win32 Error Values:
 * http://msdn.microsoft.com/en-us/library/ee442005/
 */

static UINT32 rpc_map_status_code_to_win32_error_code(UINT32 code)
{
   switch (code)
   {
   case nca_s_comm_failure:
      return RPC_S_COMM_FAILURE;

   case nca_s_op_rng_error:
      return RPC_S_PROCNUM_OUT_OF_RANGE;

   case nca_s_unk_if:
      return RPC_S_UNKNOWN_IF;

   case nca_s_wrong_boot_time:
      return nca_s_wrong_boot_time;

   case nca_s_you_crashed:
      return RPC_S_CALL_FAILED;

   case nca_s_proto_error:
      return RPC_S_PROTOCOL_ERROR;

   case nca_s_out_args_too_big:
      return RPC_S_SERVER_OUT_OF_MEMORY;

   case nca_s_server_too_busy:
      return RPC_S_SERVER_TOO_BUSY;

   case nca_s_unsupported_type:
      return RPC_S_UNSUPPORTED_TYPE;

   case nca_s_fault_int_div_by_zero:
      return RPC_S_ZERO_DIVIDE;

   case nca_s_fault_addr_error:
      return RPC_S_ADDRESS_ERROR;

   case nca_s_fault_fp_div_zero:
      return RPC_S_FP_DIV_ZERO;

   case nca_s_fault_fp_underflow:
      return RPC_S_FP_UNDERFLOW;

   case nca_s_fault_fp_overflow:
      return RPC_S_FP_OVERFLOW;

   case nca_s_fault_invalid_tag:
      return RPC_S_INVALID_TAG;

   case nca_s_fault_invalid_bound:
      return RPC_S_INVALID_BOUND;

   case nca_s_rpc_version_mismatch:
      return RPC_S_PROTOCOL_ERROR;

   case nca_s_unspec_reject:
      return RPC_S_CALL_FAILED;

   case nca_s_bad_actid:
      return RPC_S_CALL_FAILED_DNE;

   case nca_s_who_are_you_failed:
      return RPC_S_CALL_FAILED;

   case nca_s_manager_not_entered:
      return RPC_S_CALL_FAILED_DNE;

   case nca_s_fault_cancel:
      return RPC_S_CALL_CANCELLED;

   case nca_s_fault_ill_inst:
      return RPC_S_ADDRESS_ERROR;

   case nca_s_fault_fp_error:
      return RPC_S_FP_OVERFLOW;

   case nca_s_fault_int_overflow:
      return RPC_S_ADDRESS_ERROR;

   case nca_s_fault_unspec:
      return RPC_S_CALL_FAILED;

   case nca_s_fault_remote_comm_failure:
      return nca_s_fault_remote_comm_failure;

   case nca_s_fault_pipe_empty:
      return RPC_X_PIPE_EMPTY;

   case nca_s_fault_pipe_closed:
      return RPC_X_PIPE_CLOSED;

   case nca_s_fault_pipe_order:
      return RPC_X_WRONG_PIPE_ORDER;

   case nca_s_fault_pipe_discipline:
      return RPC_X_PIPE_DISCIPLINE_ERROR;

   case nca_s_fault_pipe_comm_error:
      return RPC_S_COMM_FAILURE;

   case nca_s_fault_pipe_memory:
      return RPC_S_OUT_OF_MEMORY;

   case nca_s_fault_context_mismatch:
      return RPC_X_SS_CONTEXT_MISMATCH;

   case nca_s_fault_remote_no_memory:
      return RPC_S_SERVER_OUT_OF_MEMORY;

   case nca_s_invalid_pres_context_id:
      return RPC_S_PROTOCOL_ERROR;

   case nca_s_unsupported_authn_level:
      return RPC_S_UNSUPPORTED_AUTHN_LEVEL;

   case nca_s_invalid_checksum:
      return RPC_S_CALL_FAILED_DNE;

   case nca_s_invalid_crc:
      return RPC_S_CALL_FAILED_DNE;

   case nca_s_fault_user_defined:
      return nca_s_fault_user_defined;

   case nca_s_fault_tx_open_failed:
      return nca_s_fault_tx_open_failed;

   case nca_s_fault_codeset_conv_error:
      return nca_s_fault_codeset_conv_error;

   case nca_s_fault_object_not_found:
      return nca_s_fault_object_not_found;

   case nca_s_fault_no_client_stub:
      return nca_s_fault_no_client_stub;
   }

   return code;
}

int rpc_recv_fault_pdu(rpcconn_hdr_t* header)
{
   int index;
   UINT32 code;
   WLog_ERR(TAG,  "RPC Fault PDU:");
   code = rpc_map_status_code_to_win32_error_code(header->fault.status);

   for (index = 0; RPC_FAULT_CODES[index].name != NULL; index++)
   {
      if (RPC_FAULT_CODES[index].code == code)
      {
         WLog_ERR(TAG,  "status: %s (0x%08"PRIX32")", RPC_FAULT_CODES[index].name, code);
         return 0;
      }
   }

   for (index = 0; RPC_TSG_FAULT_CODES[index].name != NULL; index++)
   {
      if (RPC_TSG_FAULT_CODES[index].code == code)
      {
         WLog_ERR(TAG,  "status: %s (0x%08"PRIX32")", RPC_TSG_FAULT_CODES[index].name, code);
         return 0;
      }
   }

   WLog_ERR(TAG,  "status: %s (0x%08"PRIX32")", "UNKNOWN", code);
   return 0;
}
