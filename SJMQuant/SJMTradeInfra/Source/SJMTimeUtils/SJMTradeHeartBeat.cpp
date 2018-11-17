#include <time.h>
#include <SJMTimeUtils/SJMTradeHeartBeat.h>

bool SJMTradeHeartBeat::heatbeat(int interval)
{
	time_t tmp = time(0);
	if (tmp > last_time && (tmp - last_time) % interval == 0) {
		last_time = tmp + 1;
	}
	return true;
}
