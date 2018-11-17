#pragma once
#include "ISJMCfgReader.h"

class DLL_ISJMCFGREADER SJMINIConfigReader : public ISJMCfgReader
{
public:
	static SJMINIConfigReader& instance();
	std::string getSJMSCConfig() const;
	std::string getLogDir() const;
	std::string getDBPath() const;
	~SJMINIConfigReader();
private:
	SJMINIConfigReader();
	void readConfig();

private:
	std::map< std::string, std::string> _configMap;

	static SJMINIConfigReader* m_instance;
	static std::mutex m_instanceLock;
};