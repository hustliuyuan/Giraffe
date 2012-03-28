// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping ( borrowed)  
// Author: ArcherSC
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef  __I_APP_TERMINATE__
#define  __I_APP_TERMINATE__



class NETWORK_EXPORT IAppTerminate 
{
public:

	IAppTerminate( );

	virtual ~IAppTerminate(void);

	virtual void					applicationTerminate(  ) = 0 ;
	
private:

};

#endif //__I_APP_TERMINATE__