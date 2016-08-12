// =====================================================================
// FreeImage Plugin Interface
//
// Design and implementation by
// - Floris van den Berg (floris@geekhq.nl)
// - Rui Lopes (ruiglopes@yahoo.com)
// - Detlev Vendt (detlev.vendt@brillit.de)
// - Petr Pytelka (pyta@lightcomp.com)
//
// This file is part of FreeImage 3
//
// COVERED CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY
// OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES
// THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE
// OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED
// CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT
// THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY
// SERVICING, REPAIR OR CORRECTION. THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL
// PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER
// THIS DISCLAIMER.
//
// Use at your own risk!
// =====================================================================

//#ifdef _MSC_VER
//#pragma warning (disable : 4786) // identifier was truncated to 'number' characters
//#endif
//
//#ifdef _WIN32
//#include <windows.h>
//#include <io.h>
//#else
//#include <ctype.h>
//#endif // _WIN32

#include  "FreeImageFramework.h"
//#include "Utilities.h"
//#include "FreeImageIO.h"
#include "Plugin.h"

//#include "../Metadata/FreeImageTag.h"

#ifdef _WIN32
#include <io.h>
#endif
#ifdef LINUX
#include <dlfcn.h>
#endif
// =====================================================================

//using namespace std;

// =====================================================================
// Plugin search list
// =====================================================================

const char *
s_search_list[] = {
	"",
	"plugins\\",
};

static int s_search_list_size = sizeof(s_search_list) / sizeof(char *);
static PluginList *s_plugins = NULL;
static int s_plugin_reference_count = 0;


// =====================================================================
// Reimplementation of stricmp (it is not supported on some systems)
// =====================================================================

int
FreeImage_stricmp(const char *s1, const char *s2) {
	int c1, c2;

	do {
		c1 = tolower(*s1++);
		c2 = tolower(*s2++);
	} while (c1 && c1 == c2);

	return c1 - c2;
}

// =====================================================================
//  Implementation of PluginList
// =====================================================================

PluginList::PluginList() :
m_plugin_map(),
m_node_count(0) {
}


FREE_IMAGE_FORMAT
PluginList::AddNode(Plugin * plugin,FREE_IMAGE_FORMAT f, PluginNode * node,void *instance,const char *format,const char *description,const char *extension,const char *regexpr) {

   // get the format string (two possible ways)

   const char *the_format = NULL;

   if(format != NULL) {
      the_format = format;
   }
   else if(plugin->format_proc != NULL) {
      the_format = plugin->format_proc();
   }

   // add the node if it wasn't there already

   if(the_format == NULL)
   {
      delete plugin;
      delete node;

      return FIF_UNKNOWN;
   }
   node->m_id = f;
   node->m_instance = instance;
   node->m_plugin = plugin;
   node->m_format = format;
   node->m_description = description;
   node->m_extension = extension;
   node->m_regexpr = regexpr;
   node->m_enabled = TRUE;

   m_plugin_map[f] = node;
   m_formata.add_unique(f);

   return (FREE_IMAGE_FORMAT)node->m_id;

   // something went wrong while allocating the plugin... cleanup

}

FREE_IMAGE_FORMAT
PluginList::AddNode(FI_InitProc init_proc, void *instance, const char *format, const char *description, const char *extension, const char *regexpr)
{

   if(init_proc == NULL)
      return FIF_UNKNOWN;

   PluginNode *node = new PluginNode;
   Plugin *plugin = new Plugin;
   if(!node || !plugin)
   {
      if(node) delete node;
      if(plugin) delete plugin;
      FreeImage_OutputMessageProc(FIF_UNKNOWN,FI_MSG_ERROR_MEMORY);
      return FIF_UNKNOWN;
   }

   memset(plugin,0,sizeof(Plugin));

   // fill-in the plugin structure
   // note we have memset to 0, so all unset pointers should be NULL)

   int iFormat = Size();

   init_proc(plugin,iFormat);

   FREE_IMAGE_FORMAT f = (FREE_IMAGE_FORMAT) iFormat;

   return AddNode(plugin,f, node,instance,format,description,extension,regexpr);

}


//FREE_IMAGE_FORMAT
//   PluginList::AddNode(FI_InitProc2 init_proc,void *instance,const char *format,const char *description,const char *extension,const char *regexpr)
//{
//   if(init_proc == NULL)
//      return FIF_UNKNOWN;
//
//   PluginNode *node = new PluginNode;
//   Plugin *plugin = new Plugin;
//   if(!node || !plugin)
//   {
//      if(node) delete node;
//      if(plugin) delete plugin;
//      FreeImage_OutputMessageProc(FIF_UNKNOWN,FI_MSG_ERROR_MEMORY);
//      return FIF_UNKNOWN;
//   }
//
//   memset(plugin,0,sizeof(Plugin));
//
//   // fill-in the plugin structure
//   // note we have memset to 0, so all unset pointers should be NULL)
//
//   init_proc(plugin,(int)m_plugin_map.size());
//
//   return AddNode(plugin,node,instance,format,description,extension,regexpr);
//
//}
//
PluginNode *
PluginList::FindNodeFromFormat(const char *format) {
	for (std::map<FREE_IMAGE_FORMAT, PluginNode *>::iterator i = m_plugin_map.begin(); i != m_plugin_map.end(); ++i) {
		const char *the_format = ((*i).m_element2->m_format != NULL) ? (*i).m_element2->m_format : (*i).m_element2->m_plugin->format_proc();

		if ((*i).m_element2->m_enabled) {
			if (FreeImage_stricmp(the_format, format) == 0) {
				return (*i).m_element2;
			}
		}
	}

	return NULL;
}

PluginNode *
PluginList::FindNodeFromMime(const char *mime) {
   for(std::map<FREE_IMAGE_FORMAT, PluginNode *>::iterator i = m_plugin_map.begin(); i != m_plugin_map.end(); ++i) {
		const char *the_mime = ((*i).m_element2->m_plugin->mime_proc != NULL) ? (*i).m_element2->m_plugin->mime_proc() : "";

		if ((*i).m_element2->m_enabled) {
			if ((the_mime != NULL) && (strcmp(the_mime, mime) == 0)) {
				return (*i).m_element2;
			}
		}
	}

	return NULL;
}

PluginNode *
PluginList::FindNodeFromFIF(int node_id) {
   std::map<FREE_IMAGE_FORMAT, PluginNode *>::iterator i = m_plugin_map.find((FREE_IMAGE_FORMAT)node_id);

	if (i != m_plugin_map.end()) {
		return (*i).m_element2;
	}

	return NULL;
}

int
PluginList::Size() const {
	return (int)m_plugin_map.size();
}

WINBOOL
PluginList::IsEmpty() const {
	return m_plugin_map.empty();
}

PluginList::~PluginList() {
   for(std::map<FREE_IMAGE_FORMAT, PluginNode *>::iterator i = m_plugin_map.begin(); i != m_plugin_map.end(); ++i) {
#ifdef _WIN32
		if ((*i).m_element2->m_instance != NULL) {
			FreeLibrary((HINSTANCE)(*i).m_element2->m_instance);
		}
#endif
		delete (*i).m_element2->m_plugin;
		delete ((*i).m_element2);
	}
}

// =====================================================================
// Retrieve a pointer to the plugin list container
// =====================================================================

PluginList * DLL_CALLCONV
FreeImage_GetPluginList() {
	return s_plugins;
}

// =====================================================================
// Plugin System Initialization
// =====================================================================

void DLL_CALLCONV
FreeImage_Initialise(WINBOOL load_local_plugins_only) {
	if (s_plugin_reference_count++ == 0) {

		/*
		Note: initialize all singletons here
		in order to avoid race conditions with multi-threading
		*/

      TagLib::create_instance();

		// initialise the TagLib singleton
TagLib::instance();

		// internal plugin initialization

		s_plugins = new PluginList;

		if (s_plugins) {
			/* NOTE :
			The order used to initialize internal plugins below MUST BE the same order
			as the one used to define the FREE_IMAGE_FORMAT enum.
			*/
			s_plugins->AddNode(InitBMP);
			s_plugins->AddNode(InitICO);
			//s_plugins->AddNode(InitJPEG);
			s_plugins->AddNode(InitJNG);
			s_plugins->AddNode(InitKOALA);
			s_plugins->AddNode(InitIFF);
			s_plugins->AddNode(InitMNG);
			s_plugins->AddNode(InitPNM, NULL, "PBM", "Portable Bitmap (ASCII)", "pbm", "^P1");
			s_plugins->AddNode(InitPNM, NULL, "PBMRAW", "Portable Bitmap (RAW)", "pbm", "^P4");
			s_plugins->AddNode(InitPCD);
			s_plugins->AddNode(InitPCX);
			s_plugins->AddNode(InitPNM, NULL, "PGM", "Portable Greymap (ASCII)", "pgm", "^P2");
			s_plugins->AddNode(InitPNM, NULL, "PGMRAW", "Portable Greymap (RAW)", "pgm", "^P5");
			//s_plugins->AddNode(InitPNG);
			s_plugins->AddNode(InitPNM, NULL, "PPM", "Portable Pixelmap (ASCII)", "ppm", "^P3");
			s_plugins->AddNode(InitPNM, NULL, "PPMRAW", "Portable Pixelmap (RAW)", "ppm", "^P6");
			s_plugins->AddNode(InitRAS);
			s_plugins->AddNode(InitTARGA);
			//s_plugins->AddNode(InitTIFF);
			s_plugins->AddNode(InitWBMP);
			s_plugins->AddNode(InitPSD);
			s_plugins->AddNode(InitCUT);
			s_plugins->AddNode(InitXBM);
			s_plugins->AddNode(InitXPM);
			s_plugins->AddNode(InitDDS);
	        s_plugins->AddNode(InitGIF);
	        s_plugins->AddNode(InitHDR);
			//s_plugins->AddNode(InitG3);
			s_plugins->AddNode(InitSGI);
			//s_plugins->AddNode(InitEXR);
			//s_plugins->AddNode(InitJ2K);
			//s_plugins->AddNode(InitJP2);
			s_plugins->AddNode(InitPFM);
			s_plugins->AddNode(InitPICT);
			//s_plugins->AddNode(InitRAW);
			//s_plugins->AddNode(InitWEBP);*/
#if !(defined(_MSC_VER) && (_MSC_VER <= 1310))
			//s_plugins->AddNode(InitJXR);
#endif // unsupported by MS Visual Studio 2003 !!!

			// external plugin initialization

			if (!load_local_plugins_only)
         {

            ::file::patha patha;

            ::dir::ls(patha,::dir::ca2_module());

#if defined(VSNORD)

            patha.add("libaxis_image_png.so");
            patha.add("libaxis_image_jpeg.so");
            
#elif defined(APPLE_IOS)

            patha.add("libaxis_image_png.dylib");
            patha.add("libaxis_image_jpeg.dylib");

#endif

            for(auto & path : patha)
            {

#ifdef WINDOWS
               if(::str::ends_ci(path,".dll") && ::str::begins_ci(path.name(), "axis_image_"))
#elif defined(APPLEOS)
               if(::str::ends_ci(path,".dylib") && ::str::begins_ci(path.name(), "libaxis_image_"))
#else
               if(::str::ends_ci(path,".so") && ::str::begins_ci(path.name(), "libaxis_image_"))
#endif
               {

                  ::aura::library l(get_thread_app());

#ifdef METROWIN
                  l.open(path.name(), false);
#else
                  l.open(path, false);
#endif

                  if(l.is_opened())
                  {

                     string strProcName;

                     string strTitle = path.title();

                     ::str::begins_eat_ci(strTitle, "lib");

                     strTitle.make_lower();

                     strProcName = "FreeImage_InitPlugin_" + strTitle;

                     FI_InitProc pproc =l.get < FI_InitProc >(strProcName);

                     if(pproc != NULL)
                     {

                        s_plugins->AddNode(pproc,(void *)l.get_os_data());

                     }
                     else
                     {

                        l.close();

                     }

                  }

               }


            }


			}
		}
	}
}

void DLL_CALLCONV
FreeImage_DeInitialise()
{
	--s_plugin_reference_count;

	if (s_plugin_reference_count == 0)
   {
		delete s_plugins;

      TagLib::destroy_instance();

	}

}

// =====================================================================
// Open and close a bitmap
// =====================================================================

void * DLL_CALLCONV
FreeImage_Open(PluginNode *node, FreeImageIO *io, fi_handle handle, WINBOOL open_for_reading) {
	if (node->m_plugin->open_proc != NULL) {
       return node->m_plugin->open_proc(io, handle, open_for_reading);
	}

	return NULL;
}

void DLL_CALLCONV
FreeImage_Close(PluginNode *node, FreeImageIO *io, fi_handle handle, void *data) {
	if (node->m_plugin->close_proc != NULL) {
		node->m_plugin->close_proc(io, handle, data);
	}
}

// =====================================================================
// Plugin System Load/Save Functions
// =====================================================================

FIBITMAP * DLL_CALLCONV
FreeImage_LoadFromHandle(FREE_IMAGE_FORMAT fif, FreeImageIO *io, fi_handle handle, int flags) {
	//if ((fif >= 0) && (fif < FreeImage_GetFIFCount()))
	{
		PluginNode *node = s_plugins->FindNodeFromFIF(fif);

		if (node != NULL) {
			if(node->m_plugin->load_proc != NULL) {
				void *data = FreeImage_Open(node, io, handle, TRUE);

				FIBITMAP *bitmap = node->m_plugin->load_proc(io, handle, -1, flags, data);

				FreeImage_Close(node, io, handle, data);

				return bitmap;
			}
		}
	}

	return NULL;
}

FIBITMAP * DLL_CALLCONV
FreeImage_Load(FREE_IMAGE_FORMAT fif, const char *filename, int flags) {
	FreeImageIO io;
	SetDefaultIO(&io);

	FILE *handle = fopen(filename, "rb");

	if (handle) {
		FIBITMAP *bitmap = FreeImage_LoadFromHandle(fif, &io, (fi_handle)handle, flags);

		fclose(handle);

		return bitmap;
	} else {
		FreeImage_OutputMessageProc((int)fif, "FreeImage_Load: failed to open file %s", filename);
	}

	return NULL;
}

FIBITMAP * DLL_CALLCONV
FreeImage_LoadU(FREE_IMAGE_FORMAT fif, const wchar_t *filename, int flags) {
	FreeImageIO io;
	SetDefaultIO(&io);
#ifdef _WIN32
	FILE *handle = _wfopen(filename, L"rb");

	if (handle) {
		FIBITMAP *bitmap = FreeImage_LoadFromHandle(fif, &io, (fi_handle)handle, flags);

		fclose(handle);

		return bitmap;
	} else {
		FreeImage_OutputMessageProc((int)fif, "FreeImage_LoadU: failed to open input file");
	}
#endif
	return NULL;
}

WINBOOL DLL_CALLCONV
FreeImage_SaveToHandle(FREE_IMAGE_FORMAT fif, FIBITMAP *dib, FreeImageIO *io, fi_handle handle, int flags) {
	// cannot save "header only" formats
	if(FreeImage_HasPixels(dib) == FALSE) {
		FreeImage_OutputMessageProc((int)fif, "FreeImage_SaveToHandle: cannot save \"header only\" formats");
		return FALSE;
	}

	//if ((fif >= 0) && (fif < FreeImage_GetFIFCount()))
	{
		PluginNode *node = s_plugins->FindNodeFromFIF(fif);

		if (node) {
			if(node->m_plugin->save_proc != NULL) {
				void *data = FreeImage_Open(node, io, handle, FALSE);

				WINBOOL result = node->m_plugin->save_proc(io, dib, handle, -1, flags, data);

				FreeImage_Close(node, io, handle, data);

				return result;
			}
		}
	}

	return FALSE;
}


WINBOOL DLL_CALLCONV
FreeImage_Save(FREE_IMAGE_FORMAT fif, FIBITMAP *dib, const char *filename, int flags) {
	FreeImageIO io;
	SetDefaultIO(&io);

	FILE *handle = fopen(filename, "w+b");

	if (handle) {
		WINBOOL success = FreeImage_SaveToHandle(fif, dib, &io, (fi_handle)handle, flags);

		fclose(handle);

		return success;
	} else {
		FreeImage_OutputMessageProc((int)fif, "FreeImage_Save: failed to open file %s", filename);
	}

	return FALSE;
}

WINBOOL DLL_CALLCONV
FreeImage_SaveU(FREE_IMAGE_FORMAT fif, FIBITMAP *dib, const wchar_t *filename, int flags) {
	FreeImageIO io;
	SetDefaultIO(&io);
#ifdef _WIN32
	FILE *handle = _wfopen(filename, L"w+b");

	if (handle) {
		WINBOOL success = FreeImage_SaveToHandle(fif, dib, &io, (fi_handle)handle, flags);

		fclose(handle);

		return success;
	} else {
		FreeImage_OutputMessageProc((int)fif, "FreeImage_SaveU: failed to open output file");
	}
#endif
	return FALSE;
}

// =====================================================================
// Plugin construction + enable/disable functions
// =====================================================================

FREE_IMAGE_FORMAT DLL_CALLCONV
FreeImage_RegisterLocalPlugin(FI_InitProc proc_address, const char *format, const char *description, const char *extension, const char *regexpr) {
	return s_plugins->AddNode(proc_address, NULL, format, description, extension, regexpr);
}

#ifdef _WIN32
FREE_IMAGE_FORMAT DLL_CALLCONV
FreeImage_RegisterExternalPlugin(const char *path, const char *format, const char *description, const char *extension, const char *regexpr) {
	if (path != NULL) {
#ifdef METROWIN
		HINSTANCE instance = NULL;
#else
      HINSTANCE instance = LoadLibrary(path);
#endif

		if (instance != NULL) {
			FARPROC proc_address = GetProcAddress(instance, "_Init@8");

			FREE_IMAGE_FORMAT result = s_plugins->AddNode((FI_InitProc)proc_address, (void *)instance, format, description, extension, regexpr);

			if (result == FIF_UNKNOWN)
				FreeLibrary(instance);

			return result;
		}
	}

	return FIF_UNKNOWN;
}
#endif // _WIN32

int DLL_CALLCONV
FreeImage_SetPluginEnabled(FREE_IMAGE_FORMAT fif, WINBOOL enable) {
	if (s_plugins != NULL) {
		PluginNode *node = s_plugins->FindNodeFromFIF(fif);

		if (node != NULL) {
			WINBOOL previous_state = node->m_enabled;

			node->m_enabled = enable;

			return previous_state;
		}
	}

	return -1;
}

int DLL_CALLCONV
FreeImage_IsPluginEnabled(FREE_IMAGE_FORMAT fif) {
	if (s_plugins != NULL) {
		PluginNode *node = s_plugins->FindNodeFromFIF(fif);

		return (node != NULL) ? node->m_enabled : FALSE;
	}

	return -1;
}

// =====================================================================
// Plugin Access Functions
// =====================================================================

int DLL_CALLCONV
FreeImage_GetFIFCount() {
	return (s_plugins != NULL) ? s_plugins->Size() : 0;
}

FREE_IMAGE_FORMAT DLL_CALLCONV
FreeImage_GetFIFType(int iIndex) {
	return (s_plugins != NULL) ? s_plugins->m_formata[iIndex] : (FREE_IMAGE_FORMAT) -1;
}

FREE_IMAGE_FORMAT DLL_CALLCONV
FreeImage_GetFIFFromFormat(const char *format) {
	if (s_plugins != NULL) {
		PluginNode *node = s_plugins->FindNodeFromFormat(format);

		return (node != NULL) ? (FREE_IMAGE_FORMAT)node->m_id : FIF_UNKNOWN;
	}

	return FIF_UNKNOWN;
}

FREE_IMAGE_FORMAT DLL_CALLCONV
FreeImage_GetFIFFromMime(const char *mime) {
	if (s_plugins != NULL) {
		PluginNode *node = s_plugins->FindNodeFromMime(mime);

		return (node != NULL) ? (FREE_IMAGE_FORMAT)node->m_id : FIF_UNKNOWN;
	}

	return FIF_UNKNOWN;
}

const char * DLL_CALLCONV
FreeImage_GetFormatFromFIF(FREE_IMAGE_FORMAT fif) {
	if (s_plugins != NULL) {
		PluginNode *node = s_plugins->FindNodeFromFIF(fif);

		return (node != NULL) ? (node->m_format != NULL) ? node->m_format : node->m_plugin->format_proc() : NULL;
	}

	return NULL;
}

const char * DLL_CALLCONV
FreeImage_GetFIFMimeType(FREE_IMAGE_FORMAT fif) {
	if (s_plugins != NULL) {
		PluginNode *node = s_plugins->FindNodeFromFIF(fif);

		return (node != NULL) ? (node->m_plugin != NULL) ? ( node->m_plugin->mime_proc != NULL )? node->m_plugin->mime_proc() : NULL : NULL : NULL;
	}

	return NULL;
}

const char * DLL_CALLCONV
FreeImage_GetFIFExtensionList(FREE_IMAGE_FORMAT fif) {
	if (s_plugins != NULL) {
		PluginNode *node = s_plugins->FindNodeFromFIF(fif);

		return (node != NULL) ? (node->m_extension != NULL) ? node->m_extension : (node->m_plugin->extension_proc != NULL) ? node->m_plugin->extension_proc() : NULL : NULL;
	}

	return NULL;
}

const char * DLL_CALLCONV
FreeImage_GetFIFDescription(FREE_IMAGE_FORMAT fif) {
	if (s_plugins != NULL) {
		PluginNode *node = s_plugins->FindNodeFromFIF(fif);

		return (node != NULL) ? (node->m_description != NULL) ? node->m_description : (node->m_plugin->description_proc != NULL) ? node->m_plugin->description_proc() : NULL : NULL;
	}

	return NULL;
}

const char * DLL_CALLCONV
FreeImage_GetFIFRegExpr(FREE_IMAGE_FORMAT fif) {
	if (s_plugins != NULL) {
		PluginNode *node = s_plugins->FindNodeFromFIF(fif);

		return (node != NULL) ? (node->m_regexpr != NULL) ? node->m_regexpr : (node->m_plugin->regexpr_proc != NULL) ? node->m_plugin->regexpr_proc() : NULL : NULL;
	}

	return NULL;
}

WINBOOL DLL_CALLCONV
FreeImage_FIFSupportsReading(FREE_IMAGE_FORMAT fif) {
	if (s_plugins != NULL) {
		PluginNode *node = s_plugins->FindNodeFromFIF(fif);

		return (node != NULL) ? node->m_plugin->load_proc != NULL : FALSE;
	}

	return FALSE;
}

WINBOOL DLL_CALLCONV
FreeImage_FIFSupportsWriting(FREE_IMAGE_FORMAT fif) {
	if (s_plugins != NULL) {
		PluginNode *node = s_plugins->FindNodeFromFIF(fif);

		return (node != NULL) ? node->m_plugin->save_proc != NULL : FALSE ;
	}

	return FALSE;
}

WINBOOL DLL_CALLCONV
FreeImage_FIFSupportsExportBPP(FREE_IMAGE_FORMAT fif, int depth) {
	if (s_plugins != NULL) {
		PluginNode *node = s_plugins->FindNodeFromFIF(fif);

		return (node != NULL) ?
			(node->m_plugin->supports_export_bpp_proc != NULL) ?
				node->m_plugin->supports_export_bpp_proc(depth) : FALSE : FALSE;
	}

	return FALSE;
}

WINBOOL DLL_CALLCONV
FreeImage_FIFSupportsExportType(FREE_IMAGE_FORMAT fif, FREE_IMAGE_TYPE type) {
	if (s_plugins != NULL) {
		PluginNode *node = s_plugins->FindNodeFromFIF(fif);

		return (node != NULL) ?
			(node->m_plugin->supports_export_type_proc != NULL) ?
				node->m_plugin->supports_export_type_proc(type) : FALSE : FALSE;
	}

	return FALSE;
}

WINBOOL DLL_CALLCONV
FreeImage_FIFSupportsICCProfiles(FREE_IMAGE_FORMAT fif) {
	if (s_plugins != NULL) {
		PluginNode *node = s_plugins->FindNodeFromFIF(fif);

		return (node != NULL) ?
			(node->m_plugin->supports_icc_profiles_proc != NULL) ?
				node->m_plugin->supports_icc_profiles_proc() : FALSE : FALSE;
	}

	return FALSE;
}

WINBOOL DLL_CALLCONV
FreeImage_FIFSupportsNoPixels(FREE_IMAGE_FORMAT fif) {
	if (s_plugins != NULL) {
		PluginNode *node = s_plugins->FindNodeFromFIF(fif);

		return (node != NULL) ?
			(node->m_plugin->supports_no_pixels_proc != NULL) ?
				node->m_plugin->supports_no_pixels_proc() : FALSE : FALSE;
	}

	return FALSE;
}

FREE_IMAGE_FORMAT DLL_CALLCONV
FreeImage_GetFIFFromFilename(const char *filename) {
	if (filename != NULL) {
		const char *extension;

		// get the proper extension if we received a filename

		char *place = strrchr((char *)filename, '.');
		extension = (place != NULL) ? ++place : filename;

		// look for the extension in the plugin table

		for (int i = 0; i < s_plugins->m_formata.get_size(); ++i) {

			if (s_plugins->FindNodeFromFIF(s_plugins->m_formata[i])->m_enabled) {

				// compare the format id with the extension

				if (FreeImage_stricmp(FreeImage_GetFormatFromFIF(s_plugins->m_formata[i]), extension) == 0) {
					return s_plugins->m_formata[i];
				} else {
					// make a copy of the extension list and split it

					char *copy = (char *)malloc(strlen(FreeImage_GetFIFExtensionList(s_plugins->m_formata[i])) + 1);
					memset(copy, 0, strlen(FreeImage_GetFIFExtensionList(s_plugins->m_formata[i])) + 1);
					memcpy(copy, FreeImage_GetFIFExtensionList(s_plugins->m_formata[i]), strlen(FreeImage_GetFIFExtensionList(s_plugins->m_formata[i])));

					// get the first token

					char *token = strtok(copy, ",");

					while (token != NULL) {
						if (FreeImage_stricmp(token, extension) == 0) {
							free(copy);

								return s_plugins->m_formata[i];
						}

						token = strtok(NULL, ",");
					}

					// free the copy of the extension list

					free(copy);
				}
			}
		}
	}

	return FIF_UNKNOWN;
}

FREE_IMAGE_FORMAT DLL_CALLCONV
FreeImage_GetFIFFromFilenameU(const wchar_t *filename) {
#ifdef _WIN32
	if (filename == NULL) return FIF_UNKNOWN;

	// get the proper extension if we received a filename
	wchar_t *place = wcsrchr((wchar_t *)filename, '.');
	if (place == NULL) return FIF_UNKNOWN;
	// convert to single character - no national chars in extensions
	char *extension = (char *)malloc(wcslen(place)+1);
	unsigned int i=0;
	for(; i < wcslen(place); i++) // convert 16-bit to 8-bit
		extension[i] = (char)(place[i] & 0x00FF);
	// set terminating 0
	extension[i]=0;
	FREE_IMAGE_FORMAT fRet = FreeImage_GetFIFFromFilename(extension);
	free(extension);

	return fRet;
#else
	return FIF_UNKNOWN;
#endif // _WIN32
}

WINBOOL DLL_CALLCONV
FreeImage_Validate(FREE_IMAGE_FORMAT fif, FreeImageIO *io, fi_handle handle) {
	if (s_plugins != NULL) {
		WINBOOL validated = FALSE;

		PluginNode *node = s_plugins->FindNodeFromFIF(fif);

		if (node) {
			long tell = io->tell_proc(handle);

			validated = (node != NULL) ? (node->m_enabled) ? (node->m_plugin->validate_proc != NULL) ? node->m_plugin->validate_proc(io, handle) : FALSE : FALSE : FALSE;

			io->seek_proc(handle, tell, SEEK_SET);
		}

		return validated;
	}

	return FALSE;
}
