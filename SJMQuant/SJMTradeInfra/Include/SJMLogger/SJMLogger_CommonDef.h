#pragma once

#ifdef DLL_EXPORT_SJMLogger
#define DLL_SJMLogger __declspec(dllexport)   // export DLL information
#else
#define DLL_SJMLogger __declspec(dllimport)   // import DLL information
#endif