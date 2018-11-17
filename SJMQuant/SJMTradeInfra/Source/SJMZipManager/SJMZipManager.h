#pragma once

#define ZLIB_WINAPI   // actually actually needed (for linkage)

#include "windows.h"  // get BYTE et al.

#include <string>

class SJMZipManager
{
public:
	SJMZipManager();
	virtual ~SJMZipManager();

public:
	bool compressDataToZip(const std::string& fileName, const std::string& zipFileName);
	std::string unCompressZipFile(const std::string& zipFileName);
	bool addDataToZipFile(const std::string& fileName, const std::string& existingZipFileName);

private:
	int GetMaxCompressedLen(int nLenSrc);
	int CompressData(const BYTE* abSrc, int nLenSrc, BYTE* abDst, int nLenDst);
	int UncompressData(const BYTE* abSrc, int nLenSrc, BYTE* abDst, int nLenDst);
};

