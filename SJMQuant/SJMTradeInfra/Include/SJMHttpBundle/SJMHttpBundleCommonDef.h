#pragma once

#ifdef DLL_EXPORT_SJMHTTPBUNDLE
#define DLL_SJMHTTPBUNDLE __declspec(dllexport)   // export DLL information
#else
#define DLL_SJMHTTPBUNDLE __declspec(dllimport)   // import DLL information
#endif