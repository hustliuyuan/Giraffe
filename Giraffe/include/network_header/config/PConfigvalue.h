// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

//#ifndef  #define #endif tuxuping 2008-5-23
#ifndef __PCONFIGVALUE_H__
#define __PCONFIGVALUE_H__


#include <string>
#include "os/PUtils.h"

class NETWORK_EXPORT PConfigValue
{
public:
	enum configValueType{
		INT_TYPE =1,
		DOUBLE_TYPE = 2,
		STRING_TYPE =3,
		NULL_TYPE = 4,  // no value in this object.
	};
	typedef enum configValueType configValueType_t;

	union uConfigValue{
		int intvalue;
		double doublevalue;
		char  strvalue[1024];		
	} ;
	typedef union uConfigValue uConfigValue_t;

	PConfigValue() ;

	PConfigValue( const PConfigValue &  from) ;

	PConfigValue(configValueType_t type ,  const char *  valuestr) ;

	~PConfigValue();

	
public:
	configValueType_t  m_valueType;  // 
	uConfigValue_t m_value;	
};
#endif

