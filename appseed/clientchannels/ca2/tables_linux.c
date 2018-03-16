/**
 * FreeRDP: A Remote Desktop Protocol Implementation
 * Static Entry Point Tables
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

#include "channels/client/tables.h"



extern void audin_DVCPluginEntry();
extern void disp_DVCPluginEntry();
extern void echo_DVCPluginEntry();
extern void rdpei_DVCPluginEntry();
extern void rdpgfx_DVCPluginEntry();
extern void tsmf_DVCPluginEntry();

extern BOOL VCAPITYPE cliprdr_VirtualChannelEntryEx(PCHANNEL_ENTRY_POINTS, PVOID);
extern BOOL VCAPITYPE drdynvc_VirtualChannelEntryEx(PCHANNEL_ENTRY_POINTS, PVOID);
extern BOOL VCAPITYPE encomsp_VirtualChannelEntryEx(PCHANNEL_ENTRY_POINTS, PVOID);
extern BOOL VCAPITYPE rail_VirtualChannelEntryEx(PCHANNEL_ENTRY_POINTS, PVOID);
extern BOOL VCAPITYPE rdpdr_VirtualChannelEntryEx(PCHANNEL_ENTRY_POINTS, PVOID);
extern BOOL VCAPITYPE rdpsnd_VirtualChannelEntryEx(PCHANNEL_ENTRY_POINTS, PVOID);
extern BOOL VCAPITYPE remdesk_VirtualChannelEntryEx(PCHANNEL_ENTRY_POINTS, PVOID);

extern void drive_DeviceServiceEntry();
extern void printer_DeviceServiceEntry();
extern void smartcard_DeviceServiceEntry();


const STATIC_ENTRY CLIENT_DVCPluginEntry_TABLE[] =
{

	{ "audin", audin_DVCPluginEntry },
	{ "disp", disp_DVCPluginEntry },
	{ "echo", echo_DVCPluginEntry },
	{ "rdpei", rdpei_DVCPluginEntry },
	{ "rdpgfx", rdpgfx_DVCPluginEntry },
	{ NULL, NULL }
};
const STATIC_ENTRY CLIENT_VirtualChannelEntryEx_TABLE[] =
{

	{ "cliprdr", cliprdr_VirtualChannelEntryEx },
	{ "drdynvc", drdynvc_VirtualChannelEntryEx },
	{ "encomsp", encomsp_VirtualChannelEntryEx },
	{ "rail", rail_VirtualChannelEntryEx },
	{ "rdpdr", rdpdr_VirtualChannelEntryEx },
	{ "rdpsnd", rdpsnd_VirtualChannelEntryEx },
	{ "remdesk", remdesk_VirtualChannelEntryEx },
	{ NULL, NULL }
};
const STATIC_ENTRY CLIENT_DeviceServiceEntry_TABLE[] =
{

	{ "drive", drive_DeviceServiceEntry },
	{ "smartcard", smartcard_DeviceServiceEntry },
	{ NULL, NULL }
};


const STATIC_ENTRY_TABLE CLIENT_STATIC_ENTRY_TABLES[] =
{
	{ "DVCPluginEntry", CLIENT_DVCPluginEntry_TABLE },
	{ "VirtualChannelEntry", CLIENT_VirtualChannelEntryEx_TABLE },
	{ "DeviceServiceEntry", CLIENT_DeviceServiceEntry_TABLE },
	{ NULL, NULL }
};


extern void alsa_freerdp_audin_client_subsystem_entry();
extern void alsa_freerdp_rdpsnd_client_subsystem_entry();


extern void pulse_freerdp_audin_client_subsystem_entry();
extern void pulse_freerdp_rdpsnd_client_subsystem_entry();

const STATIC_SUBSYSTEM_ENTRY CLIENT_AUDIN_SUBSYSTEM_TABLE[] =
{
	{ "alsa", "", alsa_freerdp_audin_client_subsystem_entry },
	{ "pulse", "", pulse_freerdp_audin_client_subsystem_entry },
	{ NULL, NULL, NULL }
};
const STATIC_SUBSYSTEM_ENTRY CLIENT_CLIPRDR_SUBSYSTEM_TABLE[] =
{
	{ NULL, NULL, NULL }
};
const STATIC_SUBSYSTEM_ENTRY CLIENT_DISP_SUBSYSTEM_TABLE[] =
{
	{ NULL, NULL, NULL }
};
const STATIC_SUBSYSTEM_ENTRY CLIENT_DRDYNVC_SUBSYSTEM_TABLE[] =
{
	{ NULL, NULL, NULL }
};
const STATIC_SUBSYSTEM_ENTRY CLIENT_DRIVE_SUBSYSTEM_TABLE[] =
{
	{ NULL, NULL, NULL }
};
const STATIC_SUBSYSTEM_ENTRY CLIENT_ECHO_SUBSYSTEM_TABLE[] =
{
	{ NULL, NULL, NULL }
};
const STATIC_SUBSYSTEM_ENTRY CLIENT_ENCOMSP_SUBSYSTEM_TABLE[] =
{
	{ NULL, NULL, NULL }
};
const STATIC_SUBSYSTEM_ENTRY CLIENT_PRINTER_SUBSYSTEM_TABLE[] =
{
	{ NULL, NULL, NULL }
};
const STATIC_SUBSYSTEM_ENTRY CLIENT_RAIL_SUBSYSTEM_TABLE[] =
{
	{ NULL, NULL, NULL }
};
const STATIC_SUBSYSTEM_ENTRY CLIENT_RDPDR_SUBSYSTEM_TABLE[] =
{
	{ NULL, NULL, NULL }
};
const STATIC_SUBSYSTEM_ENTRY CLIENT_RDPEI_SUBSYSTEM_TABLE[] =
{
	{ NULL, NULL, NULL }
};
const STATIC_SUBSYSTEM_ENTRY CLIENT_RDPGFX_SUBSYSTEM_TABLE[] =
{
	{ NULL, NULL, NULL }
};
const STATIC_SUBSYSTEM_ENTRY CLIENT_RDPSND_SUBSYSTEM_TABLE[] =
{
	{ "alsa", "", alsa_freerdp_rdpsnd_client_subsystem_entry },
	{ "pulse", "", pulse_freerdp_rdpsnd_client_subsystem_entry },
	{ NULL, NULL, NULL }
};

extern void alsa_freerdp_tsmf_client_audio_subsystem_entry();
//extern void pulse_freerdp_tsmf_client_audio_subsystem_entry();

const STATIC_SUBSYSTEM_ENTRY CLIENT_TSMF_SUBSYSTEM_TABLE[] =
{
	{ "alsa", "", alsa_freerdp_tsmf_client_audio_subsystem_entry },
	//{ "pulse", "", pulse_freerdp_tsmf_client_audio_subsystem_entry },
	{ NULL, NULL, NULL }
};
const STATIC_SUBSYSTEM_ENTRY CLIENT_REMDESK_SUBSYSTEM_TABLE[] =
{
	{ NULL, NULL, NULL }
};
const STATIC_SUBSYSTEM_ENTRY CLIENT_SMARTCARD_SUBSYSTEM_TABLE[] =
{
	{ NULL, NULL, NULL }
};

const STATIC_ADDIN_TABLE CLIENT_STATIC_ADDIN_TABLE[] =
{
	{ "audin", audin_DVCPluginEntry, CLIENT_AUDIN_SUBSYSTEM_TABLE },
	{ "cliprdr", cliprdr_VirtualChannelEntryEx, CLIENT_CLIPRDR_SUBSYSTEM_TABLE },
	{ "disp", disp_DVCPluginEntry, CLIENT_DISP_SUBSYSTEM_TABLE },
	{ "drdynvc", drdynvc_VirtualChannelEntryEx, CLIENT_DRDYNVC_SUBSYSTEM_TABLE },
	{ "drive", drive_DeviceServiceEntry, CLIENT_DRIVE_SUBSYSTEM_TABLE },
	{ "echo", echo_DVCPluginEntry, CLIENT_ECHO_SUBSYSTEM_TABLE },
	{ "encomsp", encomsp_VirtualChannelEntryEx, CLIENT_ENCOMSP_SUBSYSTEM_TABLE },
	{ "rail", rail_VirtualChannelEntryEx, CLIENT_RAIL_SUBSYSTEM_TABLE },
	{ "rdpdr", rdpdr_VirtualChannelEntryEx, CLIENT_RDPDR_SUBSYSTEM_TABLE },
	{ "rdpei", rdpei_DVCPluginEntry, CLIENT_RDPEI_SUBSYSTEM_TABLE },
	{ "rdpgfx", rdpgfx_DVCPluginEntry, CLIENT_RDPGFX_SUBSYSTEM_TABLE },
	{ "rdpsnd", rdpsnd_VirtualChannelEntryEx, CLIENT_RDPSND_SUBSYSTEM_TABLE },
	{ "remdesk", remdesk_VirtualChannelEntryEx, CLIENT_REMDESK_SUBSYSTEM_TABLE },
	{ "smartcard", smartcard_DeviceServiceEntry, CLIENT_SMARTCARD_SUBSYSTEM_TABLE },
	{ "tsmf", tsmf_DVCPluginEntry, CLIENT_TSMF_SUBSYSTEM_TABLE },
	{ NULL, NULL, NULL }
};

