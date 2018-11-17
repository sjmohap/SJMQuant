#include "SJMiniConfigReader.h"
#include <QtCore/qsettings.h>
#include <map>
using std::mutex;
SJMINIConfigReader* SJMINIConfigReader::m_instance = nullptr;
std::mutex SJMINIConfigReader::m_instanceLock;

const std::string __config_file_name__("sjmSCConfigPath");
const std::string __log_dir__("logDir");
const std::string __sqlite_db__("sqlitePath");

SJMINIConfigReader & SJMINIConfigReader::instance()
{
	if (m_instance == nullptr) {
		std::lock_guard<mutex> g(m_instanceLock);
		if (m_instance == nullptr) {
			m_instance = new SJMINIConfigReader ();
		}
	}
	return *m_instance;
}

SJMINIConfigReader::SJMINIConfigReader()
{
	readConfig();
}

void SJMINIConfigReader::readConfig()
{
	QSettings *settings = new QSettings("sjmTradeConfigurations.ini", QSettings::IniFormat);
	QString globalConfigSection("GlobalConfig/");
	_configMap[__sqlite_db__] = settings->value(globalConfigSection + QString::fromStdString(__sqlite_db__), "r").toString().toStdString();
	_configMap[__config_file_name__] = settings->value(globalConfigSection + QString::fromStdString(__config_file_name__), "r").toString().toStdString();
	_configMap[__log_dir__] = settings->value(globalConfigSection + QString::fromStdString(__log_dir__), "r").toString().toStdString();
}

std::string SJMINIConfigReader::getSJMSCConfig() const
{
	return _configMap.at(__config_file_name__);
}

std::string SJMINIConfigReader::getLogDir() const
{
	return _configMap.at(__log_dir__);
}

std::string SJMINIConfigReader::getDBPath() const
{
	return _configMap.at(__sqlite_db__);
}

SJMINIConfigReader::~SJMINIConfigReader()
{
	delete m_instance;
}
