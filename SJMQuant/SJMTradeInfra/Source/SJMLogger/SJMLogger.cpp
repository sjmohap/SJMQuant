#include "SJMLoggerPreCompiled.h"
#include <SJMLogger/SJMLogger.h>
#include <SJMTimeUtils/SJMTimeUtil.h>
#include <SJMCfgReader/SJMiniConfigReader.h>

SJMLogger* SJMLogger::m_instance = nullptr;
std::mutex SJMLogger::m_instancelock;

using namespace std;

using std::mutex;
using std::lock_guard;

SJMLogger & SJMLogger::instance()
{
	if (m_instance == nullptr) {
		std::lock_guard<std::mutex> g(m_instancelock);
		if (m_instance == nullptr) {
			m_instance = new SJMLogger();
		}
	}
	return *m_instance;
}

void SJMLogger::PrintToFile(const char * format, ...)
{
	lock_guard<mutex> g(m_instancelock);

	static char buf[1024 * 2];
	string tmpStr = SJMTimeUtils().getFullDateTimeTillMilliSecondsStr();
	size_t sz = tmpStr.size();
	strcpy(buf, tmpStr.c_str());
	buf[sz] = ' ';

	va_list args;
	va_start(args, format);
	vsnprintf(buf + sz + 1, 1024 * 2 - sz - 1, format, args);
	size_t buflen = strlen(buf);
	fwrite(buf, sizeof(char), buflen, logfile);
	va_end(args);
}

void SJMLogger::displayInConsole(const char * format, ...)
{
	lock_guard<mutex> g(m_instancelock);
	string tmpStr = SJMTimeUtils().getFullDateTimeTillMilliSecondsStr();

	printf("%s ", tmpStr.c_str()); 

	va_list argptr;
	va_start(argptr, format);
	vfprintf(stderr, format, argptr);
	va_end(argptr);
}

SJMLogger::SJMLogger() : logfile(nullptr) {
	Initialize();
}

void SJMLogger::Initialize()
{
	string logDir = SJMINIConfigReader::instance().getLogDir() + "/sjmSCLog" + SJMTimeUtils().getTimeInYmdStr() + ".log";
	// sjmtodo: define ccconfig first and then uncomment these sections to 
	/*if (CConfig::instance()._mode == RUN_MODE::REPLAY_MODE) {
		fname = CConfig::instance().logDir() + "/elitequant-replay-" + SJMTimeUtils().getTimeInYmdStr() + "..txt";
	}
	else {
		fname = CConfig::instance().logDir() + "/elitequant-" + SJMTimeUtils().getTimeInYmdStr() + "..txt";
	}*/

	logfile = fopen(logDir.c_str(), "w");
	setvbuf(logfile, nullptr, _IONBF, 0);
}

SJMLogger::~SJMLogger()
{
	fclose(logfile);
}
