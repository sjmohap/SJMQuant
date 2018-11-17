#pragma once
#include "SJMTimeUtils_CommonDef.h"

struct DLL_SJMTIMEUTILS SJMTimeUtils
	{
		std::string getTimeInYmdStr();
		std::string getTimeInYmdhmsStr();
		std::string getTimeInYmdhmsfStr();
		std::string getTimeInHmsfStr();
		int getHmsfToIntTime(std::string hmsf);

		void msleep(uint64_t _ms);
		

		std::string getPtimeToStr(const boost::posix_time::ptime& pt);
		time_t strToTime_t(const std::string& s);
		std::string Time_tToStr(time_t tt);
		time_t pTimeToTime_t(boost::posix_time::ptime t);

		int toIntDate();
		int toIntDate(time_t time);

		int toIntTime();		
		int toIntTime(time_t time);

		int intTimeToIntTimespan(int time);							// convert to # of seconds
		int intTimespanToIntTime(int timespan);						// # of seconds to int time

		int intTimeAdd(int firsttime, int timespaninseconds);		// in seconds
		int intTimeDiff(int firsttime, int latertime);				// in seconds

	public:
		std::string getFullDateTimeTillMilliSecondsStr();
		uint64_t getMicroTime();
	};