#include <SJMTradeConfig\SJMTradeConfig.h>

using std::mutex;

SJMTradeConfig* SJMTradeConfig::m_instance = nullptr;
mutex SJMTradeConfig::m_instanceLock;


SJMTradeConfig::SJMTradeConfig()
{
	readConfig();
}

SJMTradeConfig & SJMTradeConfig::instance()
{
	if (m_instance == nullptr) {
		std::lock_guard<mutex> g(m_instanceLock);
		if (m_instance == nullptr) {
			m_instance = new SJMTradeConfig();
		}
	}
	return *m_instance;
}

void SJMTradeConfig::readConfig()
{ 
	
}

std::string SJMTradeConfig::getConfigDir() const
{
	return std::string();
}

std::string SJMTradeConfig::getLogDir() const
{
	return std::string();
}

std::string SJMTradeConfig::getDataDir() const
{
	return std::string();
}


SJMTradeConfig::~SJMTradeConfig()
{
}
