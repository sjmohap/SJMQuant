#pragma once
#include "SJMTimeUtils_CommonDef.h"

class DLL_SJMTIMEUTILS SJMTradeHeartBeat
{

public:
	// derived class overwrites heartbeat to tell if itself is still alive
	bool heatbeat(int);

protected:
	time_t last_time;
};

