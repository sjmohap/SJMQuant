#pragma once

#ifdef DLL_EXPORT_SJMTIMEUTILS
#define DLL_SJMTIMEUTILS  __declspec(dllexport)   // export DLL information
#else
#define DLL_SJMTIMEUTILS  __declspec(dllimport)   // import DLL information
#endif