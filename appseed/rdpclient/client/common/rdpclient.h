#pragma once



#if defined(CUBE) || !defined(_WIN32)
#define CLASS_DECL_EXPORT
#define CLASS_DECL_IMPORT
#else
#define CLASS_DECL_EXPORT _declspec(dllexport)
#define CLASS_DECL_IMPORT _declspec(dllimport)
#endif

#ifdef _AXIS_RDPCLIENT_LIBRARY

#define CLASS_DECL_AXIS_RDPCLIENT CLASS_DECL_EXPORT

#else 

#define CLASS_DECL_AXIS_RDPCLIENT CLASS_DECL_IMPORT

#endif


