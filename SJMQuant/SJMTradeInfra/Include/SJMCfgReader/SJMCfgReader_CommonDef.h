#pragma once

#ifdef DLL_EXPORT_ISJMCFGREADER
#define DLL_ISJMCFGREADER __declspec(dllexport)   // export DLL information
#else
#define DLL_ISJMCFGREADER __declspec(dllimport)   // import DLL information
#endif