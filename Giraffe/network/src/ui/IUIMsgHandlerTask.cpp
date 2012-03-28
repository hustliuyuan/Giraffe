#include "common\Header.h"
#include "ui/IUIMsg.h"
#include "ui/IUIMsgHandlerTask.h"


PUIMsgHandlerTask::PUIMsgHandlerTask( PSynQueue<IUIReqMsgBase* > * pMsgContainer , IUIReqMsgHandler *  hdler )
:m_pMsgContainer(pMsgContainer)
{
	this->m_pHandler = hdler;
}

PUIMsgHandlerTask::~PUIMsgHandlerTask () 
{
	//TODO: delete the two handlers? yes
	delete m_pMsgContainer;
	delete m_pHandler;
}

void	PUIMsgHandlerTask::registerHandler(  IUIReqMsgHandler *  hdler ) 
{
	this->m_pHandler = hdler ;
}


void PUIMsgHandlerTask::onRun()
{
	m_pMsgContainer->lock();

	U16 iMsgCount = 10;
	for( U16 i = 0; i < iMsgCount; ++i )
	{
		if( m_pMsgContainer->empty() ) 
			break;
		IUIReqMsgBase * req_msg = m_pMsgContainer->front();
		m_pMsgContainer->pop();
		m_pHandler->handleUIReqMsg( req_msg );
	}

	m_pMsgContainer->unlock();
}

std::string PUIMsgHandlerTask::getName() const
{
	return "PUIMsgHandlerTask";
}