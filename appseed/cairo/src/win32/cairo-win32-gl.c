#include <windows.h>
#include "cairo-gl-private.h"
#include <stdint.h>
#include <gl/GL.h>
#include <gl/glext.h>


PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
PFNGLBLENDFUNCSEPARATE glBlendFuncSeparate = NULL;

void win32_gl_initialize()
{

	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
	glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATE) wglGetProcAddress("glBlendFuncSeparate");

}
