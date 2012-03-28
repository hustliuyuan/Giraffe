// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#include "common/Header.h"
#include "config/PConfigvalue.h"
#include <stdlib.h>


PConfigValue::PConfigValue(void)
{
	this->m_valueType = NULL_TYPE;
	PUtils::setZero(this->m_value.strvalue, sizeof( this->m_value ) ) ;
}


PConfigValue::PConfigValue( configValueType_t type,  const char *  valuestr)
{
	this->m_valueType = type;
	PUtils::setZero(this->m_value.strvalue,  sizeof( this->m_value ) ) ;

	//  avoid buffer overflow.
	assert(strlen( valuestr) < 1024 ) ;

	if( type == PConfigValue::INT_TYPE ) {
		this->m_value.intvalue = atoi( valuestr);
	}
	else if ( type == PConfigValue::DOUBLE_TYPE ) {
		this->m_value.doublevalue = atof( valuestr);
	}
	else{
		strcpy(this->m_value.strvalue, valuestr);
		//memcpy( this->m_value.strvalue, valuestr, sizeof( this->value) );
	}
}

PConfigValue::PConfigValue( const PConfigValue &  from)
{
	this->m_valueType = from.m_valueType;
	// we don't care what the type is,
	PUtils::setZero(this->m_value.strvalue, sizeof( this->m_value) ) ;
	memcpy( this->m_value.strvalue, from.m_value.strvalue, sizeof( this->m_value) );
}
//
PConfigValue::~PConfigValue(void)
{

}

