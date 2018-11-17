#pragma once
#include "SJMLogger_CommonDef.h"

class DLL_SJMLogger SJMLogger
{
public:
	static SJMLogger& instance();
	void PrintToFile(const char *format, ...);
	void displayInConsole(const char *format, ...);
	
private:
	SJMLogger();
	void Initialize();
	~SJMLogger();

public:
	static std::mutex m_instancelock;

private:
	static SJMLogger* m_instance;
	FILE* logfile = nullptr;
};

