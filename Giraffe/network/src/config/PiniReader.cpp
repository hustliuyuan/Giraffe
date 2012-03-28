// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#include "common/Header.h"
#include "config/PiniReader.h"
#include <string>
#include <memory.h>
//#include "win32.h"

PIniReader::PIniReader(void)
{

}

PIniReader::PIniReader( const std::string & envfile)
{
	m_Envstream.open(envfile.c_str());

	if(! m_Envstream.is_open()  )
	{
		LOG_ERROR("peerenvreader","envstream.is_open() error:");
		//assert( 0 );
		return ;
	}
}



PIniReader::~PIniReader(void)
{
	if( this->m_Envstream.is_open()){

		this->m_Envstream.close();
	}

}


bool    PIniReader::readOneItem( const std::string &  itemstring )
{

	char name[64] = {0};
	char equal_token[4]={0};
	char value[1024]={0};

	sscanf(itemstring.c_str(), "%s %s %s", name, equal_token, value );

	std::string names(name);
	std::string equal_tokens(equal_token);

	assert( equal_tokens == "=");

	ConfigKeys::const_iterator it ;

	it = m_ConfigLeft.find(name);

	if( it == m_ConfigLeft.end() )
	{
		// ERROR;
		return false;
	}

	setConfig(name, value);
	// log infor;
	return true;
}

bool	PIniReader::readOneConfig( ){


	if( PUtils::readConfigLine( m_Envstream ) != "[HEAD]" )
	{
		LOG_ERROR("peerenvreader","PUtils::ReadConfigLine error:");
		return false;
	}
	 return readOneConfig( m_Envstream );

}

bool    PIniReader::readOneConfig( std::ifstream&   in)
{

	std::string  linestr = PUtils::readConfigLine( in );
	if( linestr == "[TAIL]")
	{
		in.close(); // added by tuxuping.
		LOG_INFO("PIniReader", "readOneConfig() : read finished. closed config file ");
		return false;
	}
	if( linestr != "[BEGIN]")
	{
		LOG_ERROR("PIniReader", "ReadOneConfig() : not start with [BEGIN]");
		//std::cout<<" not start with [BEGIN]"<<std::endl;
		return false;
	}

	std::string  config_item = PUtils::readConfigLine( in );

	if( config_item =="[END]" ){
		return false;
	}

	readOneItem( config_item );

	//DecideTypeFromFirstItem();

	while ( true )
	{
		std::string  config_item = PUtils::readConfigLine( in );
		if( config_item =="[END]" )
		continue; // TODO: here, we assume just one config in this file.
		if( config_item == "[TAIL]")
		{
			in.close(); // added by tuxuping.
			LOG_INFO("PIniReader", "readOneConfig() : read finished. closed config file ");
			break;
		}
		readOneItem( config_item );
	}
	return this->verifyConfig();
}

void		PIniReader::addWantedProperty( const std::string  &  leftstring, PConfigValue::configValueType_t  valueType)
{
	ConfigKeys::const_iterator it ;

	it = m_ConfigLeft.find(leftstring);

	if( it != m_ConfigLeft.end() )
	{
	// warning;
	}

	m_ConfigLeft [  leftstring ] = valueType;
}

PConfigValue    PIniReader::getValue( const std::string &     leftstring) const
{

	Configs::const_iterator  it ;
	it =  m_mapConfig.find( leftstring ) ;
	if( it == m_mapConfig.end())
	{
		return PConfigValue();
	}
	return  it->second;
}

S32 PIniReader::setConfig(const char *name, const char * value)
{
	PConfigValue::configValueType_t  type = m_ConfigLeft[ name ] ;

	Configs::const_iterator  it2 = m_mapConfig.find( name ) ;

	if ( it2 != m_mapConfig.end() )
	{
		//return -1;
	}

	PConfigValue v( type, value) ;
	m_mapConfig[ name ] =  v;
	LOG_DEBUG("PIniReader", "SetOneConfig() : name[" << name << "] = " << value);
	return 0;
}

//
bool PIniReader::verifyConfig(  ) const
{
	ConfigKeys:: const_iterator  it ;
	for( it = m_ConfigLeft.begin();  it!= m_ConfigLeft.end(); ++it )
	{
		// Log all the items.
		std::string keyName = it->first;

		//std::cout<< keyName <<std::endl;
		Configs::const_iterator it2 = m_mapConfig.find(keyName);

		if( it2  == m_mapConfig.end() )
		{
			// ERROR, not config this item.
			//std::cout << " keyName = " << keyName << std::endl;
			LOG_INFO("PIniReader", "VerifyConfig() : the KeyName[" << keyName << "] can't get the value from the config file");
			continue;	// add by zhouqi, 20061023
			assert(0);
			return false;
		}

		std::string sKeyValue;
		char tmpValue[16];
		PUtils::setZero(tmpValue, sizeof(tmpValue));

		switch(it2->second.m_valueType){
			case PConfigValue::INT_TYPE:
				sprintf(tmpValue, "%d", it2->second.m_value.intvalue);
				sKeyValue = tmpValue;
				break;
			case PConfigValue::DOUBLE_TYPE:
				sprintf(tmpValue, "%f", it2->second.m_value.doublevalue);
				sKeyValue = tmpValue;
				break;
			case PConfigValue::STRING_TYPE:
				sKeyValue = it2->second.m_value.strvalue;
				break;
			default :
				sKeyValue = "error m_valueType";
				break;
		}
		LOG_DEBUG("IniReader","type = " << it2->second.m_valueType <<" key:"<<it2->first <<  "\t value = " << sKeyValue);
	}
	//assert(m_ConfigLeft.size() == m_mapConfig.size());
	return true;
}

