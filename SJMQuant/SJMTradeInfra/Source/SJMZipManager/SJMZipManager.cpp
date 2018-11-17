#include "SJMZipManager.h"
#include <zlib.h>     // declare the external fns -- uses zconf.h, too
//#pragma comment(lib, "zlibwapi.lib") // for access to the DLL

SJMZipManager::SJMZipManager()
{
}


SJMZipManager::~SJMZipManager()
{
}

bool SJMZipManager::compressDataToZip(const std::string & fileName, const std::string & zipFileName)
{
	return false;
}

std::string SJMZipManager::unCompressZipFile(const std::string & zipFileName)
{
	return std::string();
}

bool SJMZipManager::addDataToZipFile(const std::string & fileName, const std::string & existingZipFileName)
{
	return false;
}


//GetMaxCompressedLen -- Pass in the length of the source buffer and it returns the 
//maximum size needed for the compressed output.
//
//It should not be too shocking to learn that after running a lossless compression function, 
//it is possible for the output to be larger than the input.Compression algorithms work by 
//finding common repeated sequences.If there are no(or very few) repetitions, as in JPG files
//and encrypted files, then the algorithm can't find any air to squeeze out.  There is a minor
//amount of overhead (6 bytes overall and 5 bytes per 16K block, as documented on the website), 
//and this function takes that into consideration.

int SJMZipManager::GetMaxCompressedLen(int nLenSrc)
{
	int n16kBlocks = (nLenSrc + 16383) / 16384; // round up any fraction of a block
	return (nLenSrc + 6 + (n16kBlocks * 5));
}

//CompressData -- Pass in the address of the source(uncompressed) buffer and its length and 
//provide an output buffer and its length.Use GetMaxCompressedLen when setting up these final 
//two parameters.

int SJMZipManager::CompressData(const BYTE* abSrc, int nLenSrc, BYTE* abDst, int nLenDst)
{
	z_stream zInfo = {0};
	zInfo.total_in = zInfo.avail_in = nLenSrc;
	zInfo.total_out = zInfo.avail_out = nLenDst;
	zInfo.next_in = (BYTE*)abSrc;
	zInfo.next_out = abDst;

	int nErr, nRet = -1;
	nErr = deflateInit(&zInfo, Z_DEFAULT_COMPRESSION); // zlib function
	if (nErr == Z_OK)
	{
		nErr = deflate(&zInfo, Z_FINISH); // zlib function
		if (nErr == Z_STREAM_END)
		{
			nRet = zInfo.total_out;
		}
	}
	deflateEnd(&zInfo); // zlib function
	return (nRet);
}

//UncompressData -- Pass in the address of the source buffer(the compressed data) and its length 
//and provide an output buffer and its length.The output buffer needs to be large enough to hold 
//the uncompressed output.
//
//You should save the original data length(before compression) and use that in preparing the last
//two parameters.In this simplified example, I don't provide any means to estimate the output
//length (Note: The zlib algorithm can get compression as high as 1000-to-1 in certain extreme cases).

int SJMZipManager::UncompressData(const BYTE* abSrc, int nLenSrc, BYTE* abDst, int nLenDst)
{
	z_stream zInfo = {0};
	zInfo.total_in = zInfo.avail_in = nLenSrc;
	zInfo.total_out = zInfo.avail_out = nLenDst;
	zInfo.next_in = (BYTE*)abSrc;
	zInfo.next_out = abDst;

	int nErr, nRet = -1;
	nErr = inflateInit(&zInfo); // zlib function
	if (nErr == Z_OK)
	{
		nErr = inflate(&zInfo, Z_FINISH); // zlib function
		if (nErr == Z_STREAM_END)
		{
			nRet = zInfo.total_out;
		}
	}
	inflateEnd(&zInfo); // zlib function
	return (nRet); // -1 or len of output
}
