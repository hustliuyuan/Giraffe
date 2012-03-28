// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef  PPLAT_TRANS_THREAD_FUNC_H
#define PPLAT_TRANS_THREAD_FUNC_H 

#include "common/IPacketHandlerTask.h"
#include "common/PTaskQueue.h"
#include "common/IAppTerminate.h"


class NETWORK_EXPORT PPlatTransThreadFunc : public IAppTerminate
{
public:

	// if the applications has not a windows UI, 
	// then uimsg_handler is NULL by defualt. tuxuping 2009-10-21
	PPlatTransThreadFunc(ITask *transmitter, 
		IPacketHandlerTask * pkt_handler, 
		ITask * uimsg_handler = NULL); 

public:

	virtual						~PPlatTransThreadFunc();

	// sorry, this operator is not implemented as plugable and not detachable.
	// you can just invoke this operater only once.
	void						operator() ( ) ;	

	void						stop( ) ;

	// added by tuxuping 2009-6-4
//	void								addTask( ITask *  task);

	virtual  void				applicationTerminate( );

private:

	void						initialize( ) ;


	// the platform will write the message to the UI to process.
	// come from outside. dont delete it when this object is destructed.
	//PToUIMsgReceiver	*					m_pUpMsgReceiver;

	B8							m_bIsInitialized;

	// deleted when this object is destructed.
	PTaskQueue	*				m_pTaskQueue;

	// deleted when this object is destructed.
	IPacketHandlerTask *		m_pPktHandler;

	ITask	*   m_pITransmitter_task ;

	// added by tuxuping 2009-6-4
	// ui msg handler 
	ITask *						m_pUIMsgHandler;

	// to stop the thread.
	B8							m_bIsStop;

};


#endif //PPlatTransThreadFunc
