#ifndef IUIMSG_HANDLER_H__
#define IUIMSG_HANDLER_H__

#include "ui/IUIMsg.h"
#include "common/PSynQueue.h"
#include "common/ITask.h"

// UIReqMsg means: ui raises msg.
class  NETWORK_EXPORT IUIReqMsgHandler  
{

public :

	virtual void handleUIReqMsg( const IUIReqMsgBase * ) = 0 ;

};

class  NETWORK_EXPORT PUIMsgHandlerTask : public ITask
{

public :

	PUIMsgHandlerTask( PSynQueue<IUIReqMsgBase* >  *  pMsgContainer, 
		IUIReqMsgHandler *  hdler = NULL);

	virtual ~PUIMsgHandlerTask();

	void		registerHandler( IUIReqMsgHandler *  hdler );

	virtual void onRun();

	virtual std::string getName() const;
 
protected :

	PSynQueue<IUIReqMsgBase* >   *  m_pMsgContainer;

	IUIReqMsgHandler *				m_pHandler ;
	
};

#endif
