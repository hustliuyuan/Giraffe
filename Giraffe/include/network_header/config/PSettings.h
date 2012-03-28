// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

//#ifndef  #define #endif tuxuping 2008-5-23

#ifndef __PSETTINGS_H__
#define __PSETTINGS_H__

#include "PConfigValue.h"
#include <string>
#include "PIniReader.h"


class NETWORK_EXPORT PSettings
{
protected:

	PSettings();

public:

	~PSettings(void);

	// load environment.
	bool								loadEnv( PIniReader & inireader );

	PConfigValue						getConfig( const std::string & key) const;

	static PSettings&					getInstance();

	//static void							delInstance();

public:

	struct sConnection
	{
		U32				timeoutselectsecond;      // seconds.
		U32				timeoutselectmicroseconds; // microseconds
	} Connection;

S32		setConfig(const char *keyName,
					PConfigValue::configValueType_t type, const char * value);

private:
		void				setup( );

#ifdef __linux__
		int					getlocaip(char *ip);
#endif
	//static PSettings*				m_pInstance;

			  Configs				m_mapConfig;
	//    config_left =  config_value
	//    all lefts are stored in the m_ConfigLeft .

		   ConfigKeys				m_ConfigLeft;

		   //static PSettings			m_Instance;
};
#endif //__PSETTINGS_H__

