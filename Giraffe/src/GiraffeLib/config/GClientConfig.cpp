#include "Base/GHeader.h"
#include "GClientConfig.h"


GClientConfig::GClientConfig(const std::string & config_file):PIniReader(config_file)
{
	this->addWantedProperty("local-tcp-server-port", PConfigValue::INT_TYPE );
	this->addWantedProperty("local-udp-server-port", PConfigValue::INT_TYPE );
	this->addWantedProperty("giraffe-server-ip",PConfigValue::STRING_TYPE);
	this->addWantedProperty("giraffe-server-tcp-port",PConfigValue::INT_TYPE);
	this->addWantedProperty("giraffe-server-udp-port",PConfigValue::INT_TYPE);
	this->addWantedProperty("giraffe-lease-interval",PConfigValue::INT_TYPE);
	this->addWantedProperty("client-test-file-path",PConfigValue::STRING_TYPE);
	
}

GClientConfig::~GClientConfig(void)
{

}
