// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

//#ifndef  #define #endif tuxuping 2008-5-23
#ifndef __INI_READER_H__
#define __INI_READER_H__


#include  <map>
#include <string>
#include <fstream>
#include "PConfigvalue.h"

typedef std::map< std::string , PConfigValue >   Configs;
typedef std::map< std::string , PConfigValue::configValueType_t >   ConfigKeys;

class NETWORK_EXPORT PIniReader
{
public:

	friend class PSettings;

protected:
	PIniReader();

	PIniReader( const std::string & file);

public:
	virtual ~PIniReader(void);

public:

	void					addWantedProperty( const std::string  &  leftstring, PConfigValue::configValueType_t   valueType) ;

	//   assure the item are legal and well configed , and put to the map.


	PConfigValue			getValue( const std::string &     leftstring) const ;

	S32						setConfig(const char *keyName, const char * value);

	// read one config.
	// format:
	//  [BEGIN]
	// # comments;
	// leftstring1 = value1;
	// leftint2 = 34;
	// leftdouble3 = 3.4;


	// this function decide the config type ,

	// default to read a config file inside the inireader.
	virtual  bool			readOneConfig( );

	virtual  bool			readOneConfig( std::ifstream&   in);

	// just assure every config item must be configed. dont'  check if the value are illegal
	bool					verifyConfig(  ) const ;

	//virtual   SInt32		getType(  ) const = 0 ;

	//virtual   void		DecideTypeFromFirstItem() =0 ;

private:

	bool					readOneItem( const std::string &  itemstring );

	Configs					m_mapConfig;
	//    config_left =  config_value
	//    all lefts are stored in the m_ConfigLeft .

	ConfigKeys				m_ConfigLeft;

	std::ifstream			m_Envstream;
};

#endif

