#pragma once
#ifdef DLL_EXPORT_SJMMsgUtils
#define DLL_SJMMsgUtils __declspec(dllexport)   // export DLL information
#else
#define DLL_SJMMsgUtils __declspec(dllimport)   // import DLL information
#endif