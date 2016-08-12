#include  "FreeImageFramework.h"


#include "Metadata/Exif.cpp"
#include "Metadata/FIRational.cpp"
#include "Metadata/FreeImageTag.cpp"
#include "Metadata/IPTC.cpp"
#include "Metadata/TagConversion.cpp"
#include "Metadata/TagLib.cpp"
//#include "Metadata/XTIFF.cpp"

//O volume na unidade Z ‚ eco
// O N£mero de S‚rie do Volume ‚ 4A91-94E4
//
// Pasta de Z:\nordroid\app\axis\freeimage\Source\FreeImage
//
#include "BitmapAccess.cpp"
#include "CacheFile.cpp"
#include "ColorLookup.cpp"
#include "Conversion.cpp"
#include "Conversion16_555.cpp"
#include "Conversion16_565.cpp"
#include "Conversion24.cpp"
#include "Conversion32.cpp"
#include "Conversion4.cpp"
#include "Conversion8.cpp"
#include "ConversionFloat.cpp"
#include "ConversionRGB16.cpp"
#include "ConversionRGBF.cpp"
#include "ConversionType.cpp"
#include "ConversionUINT16.cpp"
#include "FreeImage.cpp"
#include "FreeImageFramework.cpp"
#include "FreeImageIO.cpp"
#include "GetType.cpp"
#include "Halftoning.cpp"
//#include "J2KHelper.cpp"
#include "MemoryIO.cpp"
#include "MNGHelper.cpp"
#include "MultiPage.cpp"
#include "NNQuantizer.cpp"
#include "PixelAccess.cpp"
#include "Plugin.cpp"


#include "PSDParser.h"



#define PLUGIN_INCL_EXT ".cpp"
#define IPLUGIN(PLUGIN) PLUGIN
#define PLUGIN_CONCAT(NAME, PLUGIN)  NAME ## PLUGIN
#define PLUGIN_PREP(NAME, PLUGIN)  PLUGIN_CONCAT(NAME, PLUGIN)


#undef PLUGIN
#define PLUGIN BMP
#include "_Plugin.h"
#include "PluginBMP.cpp"


#undef PLUGIN
#define PLUGIN CUT
#include "_Plugin.h"
#include "PluginCUT.cpp"


#undef PLUGIN
#define PLUGIN DDS
#include "_Plugin.h"
#include "PluginDDS.cpp"
//#include "PluginEXR.cpp"
//#include "PluginG3.cpp"


#undef PLUGIN
#define PLUGIN GIF
#include "_Plugin.h"
#include "PluginGIF.cpp"


#undef PLUGIN
#define PLUGIN HDR
#include "_Plugin.h"
#include "PluginHDR.cpp"


#undef PLUGIN
#define PLUGIN ICO
#include "_Plugin.h"
#include "PluginICO.cpp"


#undef PLUGIN
#define PLUGIN IFF
#include "_Plugin.h"
#include "PluginIFF.cpp"
//#include "PluginJ2K.cpp"


#undef PLUGIN
#define PLUGIN JNG
#include "_Plugin.h"
#include "PluginJNG.cpp"
//#include "PluginJP2.cpp"
//#include "PluginJPEG.cpp"
//#include "PluginJXR.cpp"


#undef PLUGIN
#define PLUGIN KOALA
#include "_Plugin.h"
#include "PluginKOALA.cpp"


#undef PLUGIN
#define PLUGIN MNG
#include "_Plugin.h"
#include "PluginMNG.cpp"


#undef PLUGIN
#define PLUGIN PCD
#include "_Plugin.h"
#include "PluginPCD.cpp"


#undef PLUGIN
#define PLUGIN PCX
#include "_Plugin.h"
#include "PluginPCX.cpp"


#undef PLUGIN
#define PLUGIN PFM
#include "_Plugin.h"
#include "PluginPFM.cpp"


#undef PLUGIN
#define PLUGIN PICT
#include "_Plugin.h"
#include "PluginPICT.cpp"
//#include "PluginPNG.cpp"


#undef PLUGIN
#define PLUGIN PNM
#include "_Plugin.h"
#include "PluginPNM.cpp"


#undef PLUGIN
#define PLUGIN PSD
#include "_Plugin.h"
#include "PluginPSD.cpp"


#undef PLUGIN
#define PLUGIN RAS
#include "_Plugin.h"
#include "PluginRAS.cpp"
//#include "PluginRAW.cpp"


#undef PLUGIN
#define PLUGIN SGI
#include "_Plugin.h"
#include "PluginSGI.cpp"


#undef PLUGIN
#define PLUGIN TARGA
#include "_Plugin.h"
#include "PluginTARGA.cpp"
//#include "PluginTIFF.cpp"


#undef PLUGIN
#define PLUGIN WBMP
#include "_Plugin.h"
#include "PluginWBMP.cpp"

//#include "PluginWebP.cpp"


#undef PLUGIN
#define PLUGIN XBM
#include "_Plugin.h"
#include "PluginXBM.cpp"


#undef PLUGIN
#define PLUGIN XPM
#include "_Plugin.h"
#include "PluginXPM.cpp"




#undef s_format_id
#undef Format
#undef Description
#undef Extension
#undef RegExpr
#undef MimeType
#undef Validate
#undef SupportsExportDepth
#undef SupportsExportType
#undef SupportsNoPixels
#undef Load
#undef Open
#undef Close
#undef Save
#undef PageCount
#undef SupportsICCProfiles



#include "PSDParser.cpp"
#include "TIFFLogLuv.cpp"
#include "tmoColorConvert.cpp"
#include "tmoDrago03.cpp"
#include "tmoFattal02.cpp"
#include "tmoReinhard05.cpp"
#include "ToneMapping.cpp"
#include "WuQuantizer.cpp"
#include "ZLibInterface.cpp"
//2015-01-11  08:29                 0 _FreeImage.cpp"
//              69 arquivo(s)      1.154.664 bytes
//               0 pasta(s)   67.622.215.680 bytes dispon¡veis
