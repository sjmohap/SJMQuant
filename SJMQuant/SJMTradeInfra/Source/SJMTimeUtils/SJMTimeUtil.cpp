
#include <Windows.h>
#include <WinBase.h>

#include <string>
#include <ctime>
#include <chrono> //for msleep
#include <thread> //for msleep
#include <ctime>
#include <mutex>
#include <sstream>
#include <iterator>
#include <limits.h>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/local_time/local_time_types.hpp>
#include <boost/date_time/local_time/local_time_io.hpp>


#include <SJMTimeUtils/SJMTimeUtil.h>

#define LOCALTIME_S(x,y) localtime_s(x,y)
#define DATE_FORMAT "%Y-%m-%d"
#define DATE_FORMAT_CLEAN  "%4d-%02d-%02d"
#define DATE_TIME_FORMAT "%Y-%m-%d %H:%M:%S"
#define DATE_TIME_FORMAT_CLEAN  "%4d-%02d-%02d %02d:%02d:%02d"

#define TIMEZONE_STRING(s) #s
#define NYC_TZ_OFFSET -04
#define NYC_TZ_STR "UTC" TIMEZONE_STRING(NYC_TZ_OFFSET) ":00:00"

std::string SJMTimeUtils::getTimeInYmdStr()
{
	char buf[128] = { 0 };
	const size_t sz = sizeof("0000-00-00");
	{
		time_t timer;
		struct tm* tm_info;
		time(&timer);
		tm_info = localtime(&timer);
		strftime(buf, sz, DATE_FORMAT, tm_info);
	}
	return std::string(buf);
}

std::string SJMTimeUtils::getTimeInYmdhmsStr() {
	char buf[128] = { 0 };
	const size_t sz = sizeof("0000-00-00 00-00-00");
	{
		time_t timer;
		time(&timer);
		struct tm* tm_info = localtime(&timer);
		strftime(buf, sz, DATE_TIME_FORMAT, tm_info);
	}
	return std::string(buf);
}

std::string SJMTimeUtils::getTimeInYmdhmsfStr() {
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::chrono::system_clock::duration tp = now.time_since_epoch();
	tp -= std::chrono::duration_cast<std::chrono::seconds>(tp);
	time_t tt = std::chrono::system_clock::to_time_t(now);

	// tm t = *gmtime(&tt);
	tm t = *localtime(&tt);

	char buf[64];
	std::sprintf(buf, "%04u-%02u-%02u %02u:%02u:%02u.%03u", t.tm_year + 1900,
		t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec,
		static_cast<unsigned>(tp / std::chrono::milliseconds(1)));

	return std::string(buf);
}

std::string SJMTimeUtils::getTimeInHmsfStr() {
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::chrono::system_clock::duration tp = now.time_since_epoch();
	tp -= std::chrono::duration_cast<std::chrono::seconds>(tp);
	time_t tt = std::chrono::system_clock::to_time_t(now);

	// tm t = *gmtime(&tt);			// utc
	tm t = *localtime(&tt);

	char buf[64];
	std::sprintf(buf, "%02u:%02u:%02u.%03u", t.tm_hour, t.tm_min, t.tm_sec,
		static_cast<unsigned>(tp / std::chrono::milliseconds(1)));

	return std::string(buf);
}

int SJMTimeUtils::getHmsfToIntTime(std::string hmsf) {
	return std::stoi(hmsf.substr(0, 2)) * 10000 + std::stoi(hmsf.substr(3, 2)) * 100 + std::stoi(hmsf.substr(6, 2));
}

void SJMTimeUtils::msleep(uint64_t _ms) {
	if (_ms == 0) { return; }
	std::this_thread::sleep_for(std::chrono::milliseconds(_ms));
}



std::string SJMTimeUtils::getPtimeToStr(const boost::posix_time::ptime & pt)
{
	boost::local_time::time_zone_ptr tz_cet(new boost::local_time::posix_time_zone(NYC_TZ_STR));
	boost::local_time::local_date_time dt_with_zone(pt, tz_cet); //glocale::instance()._ny_tzone);

	tm _t = to_tm(dt_with_zone);
	char buf[32] = { 0 };
	strftime(buf, 32, DATE_TIME_FORMAT, &_t);
	return buf;
}

time_t SJMTimeUtils::strToTime_t(const std::string & s)
{
	boost::posix_time::ptime pt(boost::posix_time::time_from_string(s));
	return pTimeToTime_t(pt);
}

std::string SJMTimeUtils::Time_tToStr(time_t tt)
{
	boost::posix_time::ptime pt = boost::posix_time::from_time_t(tt);
	// return ptime2str(pt);
	return to_simple_string(pt);
}

time_t SJMTimeUtils::pTimeToTime_t(boost::posix_time::ptime t)
{
	static boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
	boost::posix_time::time_duration::sec_type x = (t - epoch).total_seconds() - 3600 * NYC_TZ_OFFSET;
	//hours(4).total_seconds() = 3600 * 4
	// ... check overflow here ...
	return time_t(x);
}

int SJMTimeUtils::toIntDate() {
	time_t current_time;
	time(&current_time);
	return toIntDate(current_time);
}

int SJMTimeUtils::toIntTime() {
	time_t current_time;
	time(&current_time);
	return toIntTime(current_time);
}

int SJMTimeUtils::toIntDate(time_t time) {
	struct tm timeinfo;
	LOCALTIME_S(&timeinfo, &time);

	return ((timeinfo.tm_year + 1900) * 10000) + ((timeinfo.tm_mon + 1) * 100) + timeinfo.tm_mday;
}

int SJMTimeUtils::toIntTime(time_t time) {
	//std::time_t rawtime;
	//std::tm* timeinfo;
	//char queryTime[80];
	//std::time(&rawtime);
	//timeinfo = std::localtime(&rawtime);
	//std::strftime(queryTime, 80, "%Y%m%d %H:%M:%S", timeinfo);
	struct tm timeinfo;
	LOCALTIME_S(&timeinfo, &time);

	return (timeinfo.tm_hour * 10000) + (timeinfo.tm_min * 100) + (timeinfo.tm_sec);
}
// # of seconds to int time
int SJMTimeUtils::intTimeToIntTimespan(int timespan) {
	int hour = timespan / 3600;
	int second = timespan % 3600;
	int minute = second / 60;
	second = second % 60;
	return (hour * 10000 + minute * 100 + second);
}

int SJMTimeUtils::intTimespanToIntTime(int timespan)
{
	int hour = timespan / 3600;
	int second = timespan % 3600;
	int minute = second / 60;
	second = second % 60;
	return (hour * 10000 + minute * 100 + second);
}

// adds inttime and int timespan (in seconds).  does not rollover 24hr periods.
int SJMTimeUtils::intTimeAdd(int firsttime, int timespaninseconds)
{
	int s1 = firsttime % 100;
	int m1 = ((firsttime - s1) / 100) % 100;
	int h1 = (int)((firsttime - m1 * 100 - s1) / 10000);
	s1 += timespaninseconds;
	if (s1 >= 60)
	{
		m1 += (int)(s1 / 60);
		s1 = s1 % 60;
	}
	if (m1 >= 60)
	{
		h1 += (int)(m1 / 60);
		m1 = m1 % 60;
	}
	int sum = h1 * 10000 + m1 * 100 + s1;
	return sum;
}

int SJMTimeUtils::intTimeDiff(int firsttime, int latertime)
{
	int span1 = intTimeToIntTimespan(firsttime);
	int span2 = intTimeToIntTimespan(latertime);
	return span2 - span1;
}


std::string SJMTimeUtils::getFullDateTimeTillMilliSecondsStr()
{
	char buf[128] = {};
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);

	sprintf(buf, "%04u-%02u-%02u %02u:%02u:%02u.%03u",
		SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,
		SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond, SystemTime.wMilliseconds);

	return buf;
}


/**
* uint64_t SJMTimeUtils::getMicroTime():
*
* Returns the real time, in micro-seconds, or 0 if an error occurred.
*
* Time is measured since an arbitrary and OS-dependent start time.
* The returned real time is only useful for computing an elapsed time
* between two calls to this function.
*/

uint64_t SJMTimeUtils::getMicroTime()
{
	uint64_t t = 0;
	FILETIME tm;
#if defined(NTDDI_WIN8) && NTDDI_VERSION >= NTDDI_WIN8
	/* Windows 8, Windows Server 2012 and later. ---------------- */
	GetSystemTimePreciseAsFileTime(&tm);
#else
	/* Windows 2000 and later. ---------------------------------- */
	GetSystemTimeAsFileTime(&tm);
#endif
	t = ((uint64_t)tm.dwHighDateTime << 32) | (uint64_t)tm.dwLowDateTime;
	return t;
}
