// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef		__VITUAL_TIME_H__
#define		__VITUAL_TIME_H__

class NETWORK_EXPORT PTime
{
public:

	static S32		getLowResolutionSeconds();

	static S64		getLowResolutionMilliSeconds();

	static void		setLowResolutionSeconds();

	static void		setLowResolutionMilliSeconds();

	//
private:

	// resolution: 2ms
	// the task run a cycle using 2ms.
	static	S64		m_isMilliSeconds;

	static	S32		m_isSeconds;

};

#endif //__VITUAL_GLOBAL_TIME_H__

