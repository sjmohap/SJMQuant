#pragma once
#include <string>
#include <mutex>
#include <vector>
#include <atomic>
#include <map>
#include <memory>
#include "SJMCfgReader_CommonDef.h"

class ISJMCfgReader
{
public:
	
	virtual ~ISJMCfgReader() {}
};