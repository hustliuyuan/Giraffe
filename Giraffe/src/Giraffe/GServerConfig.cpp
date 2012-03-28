#include "Base/GHeader.h"
#include "GServerConfig.h"


GServerConfig::GServerConfig(const std::string & config_file):PIniReader(config_file)
{
	this->addWantedProperty("paxos-tcp-port", PConfigValue::INT_TYPE );
	this->addWantedProperty("paxos-udp-port", PConfigValue::INT_TYPE );
	this->addWantedProperty("local-tcp-server-port", PConfigValue::INT_TYPE );
	this->addWantedProperty("local-udp-server-port", PConfigValue::INT_TYPE );
	this->addWantedProperty("giraffe-lease-interval", PConfigValue::INT_TYPE );
	this->addWantedProperty("giraffe-mode",PConfigValue::STRING_TYPE);
	this->addWantedProperty("giraffe-keepalive-interval", PConfigValue::INT_TYPE);
	this->addWantedProperty("giraffe-grace-period", PConfigValue::INT_TYPE);
	this->addWantedProperty("giraffe-workers",PConfigValue::INT_TYPE);
	this->addWantedProperty("giraffe-paxos-sync-interval",PConfigValue::INT_TYPE);
	this->addWantedProperty("giraffe-leader-lease-interval",PConfigValue::INT_TYPE);
	
	
	this->addWantedProperty("replica.1-ip",PConfigValue::STRING_TYPE);
	this->addWantedProperty("replica.1-port",PConfigValue::INT_TYPE);

	this->addWantedProperty("replica.2-ip",PConfigValue::STRING_TYPE);
	this->addWantedProperty("replica.2-port",PConfigValue::INT_TYPE);

	this->addWantedProperty("replica.3-ip",PConfigValue::STRING_TYPE);
	this->addWantedProperty("replica.3-port",PConfigValue::INT_TYPE);

	this->addWantedProperty("replica.4-ip",PConfigValue::STRING_TYPE);
	this->addWantedProperty("replica.4-port",PConfigValue::INT_TYPE);

	this->addWantedProperty("replica.5-ip",PConfigValue::STRING_TYPE);
	this->addWantedProperty("replica.5-port",PConfigValue::INT_TYPE);
	this->addWantedProperty("giraffe-id",PConfigValue::INT_TYPE);
}

GServerConfig::~GServerConfig(void)
{

}
