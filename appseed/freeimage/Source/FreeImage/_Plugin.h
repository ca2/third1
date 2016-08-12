

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


#define s_format_id PLUGIN_PREP(s_format_id, PLUGIN)
#define Format PLUGIN_PREP(Format, PLUGIN)
#define Description PLUGIN_PREP(Description, PLUGIN)
#define Extension PLUGIN_PREP(Extension, PLUGIN)
#define RegExpr PLUGIN_PREP(RegExpr, PLUGIN)
#define MimeType PLUGIN_PREP(MimeType, PLUGIN)
#define Validate PLUGIN_PREP(Validate, PLUGIN)
#define SupportsExportDepth PLUGIN_PREP(SupportsExportDepth, PLUGIN)
#define SupportsExportType PLUGIN_PREP(SupportsExportType, PLUGIN)
#define SupportsNoPixels PLUGIN_PREP(SupportsNoPixels, PLUGIN)
#define Load PLUGIN_PREP(Load, PLUGIN)
#define Open PLUGIN_PREP(Open, PLUGIN)
#define Close PLUGIN_PREP(Close, PLUGIN)
#define Save PLUGIN_PREP(Save, PLUGIN)
#define PageCount PLUGIN_PREP(PageCount, PLUGIN)
#define SupportsICCProfiles PLUGIN_PREP(SupportsICCProfiles, PLUGIN)







