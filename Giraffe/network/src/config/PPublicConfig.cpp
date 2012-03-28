// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#include "common/Header.h"
#include "config/PPublicConfig.h"
//#include "win32.h"


PPublicConfig::PPublicConfig(const std::string & config_file):PIniReader(config_file)
{
	//this->addWantedProperty("max-tcp-packet-size", PConfigValue::INT_TYPE );
	//this->addWantedProperty("max-udp-packet-size", PConfigValue::INT_TYPE );
	//this->addWantedProperty("for-example-int", PConfigValue::INT_TYPE );
	//this->addWantedProperty("for-example-float", PConfigValue::DOUBLE_TYPE );
	//this->addWantedProperty("for-example-string", PConfigValue::STRING_TYPE );
	// for example structure.
	this->addWantedProperty("upstructure.config-item1", PConfigValue::INT_TYPE );
	this->addWantedProperty("upstructure.config-item2", PConfigValue::INT_TYPE );
	// added your properties here , and sign your name.
	// 10000
	this->addWantedProperty("log-level", PConfigValue::INT_TYPE );
	
	this->addWantedProperty("send-count-num", PConfigValue::INT_TYPE );
	this->addWantedProperty("select-timeout", PConfigValue::INT_TYPE );


	
}

PPublicConfig::~PPublicConfig(void)
{

}
