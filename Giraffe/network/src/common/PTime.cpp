// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#include "common/Header.h"
#include "os/PUtils.h"
#include "common/PTime.h"
//#include "win32.h"

S64 PTime::m_isMilliSeconds = 0;
S32 PTime::m_isSeconds = 0;

S32	PTime::getLowResolutionSeconds()
{
	return m_isSeconds;
}

S64	PTime::getLowResolutionMilliSeconds()
{
	return m_isMilliSeconds;
}

void	PTime::setLowResolutionSeconds()
{
	m_isSeconds = PUtils::seconds();	
}

void	PTime::setLowResolutionMilliSeconds()
{
	m_isMilliSeconds= PUtils::milliSeconds();
}