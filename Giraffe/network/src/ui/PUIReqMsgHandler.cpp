
#include "PUIReqMsg.h"
#include "PUIReqMsgHandler.h"
#include "PMsgMgr.h"

PUIMsgReqHandler::PUIMsgReqHandler( PSynQueue<IUIMsgReqBase* > * pMsgContainer )
:m_pMsgContainer(pMsgContainer)
{
	
}

void PUIMsgReqHandler::handleUIMsg( const  * pMsg )
{
	static PMsgMgr* manager = PMsgMgr::GetInstance();
	if( manager != NULL)
		    manager->HandleUIReqMsg(pMsg);

}

void PUIMsgReqHandler::onRun()
{
	m_pMsgContainer->lock();

	U16 iMsgCount = 10;
	for( U16 i = 0; i < iMsgCount; ++i )
	{
		if( m_pMsgContainer->empty() ) 
			break;
		IUIMsgReqBase * req_msg = m_pMsgContainer->front();
		m_pMsgContainer->pop();
		handleUIMsg( req_msg );
	}

	m_pMsgContainer->unlock();
}

std::string PUIMsgReqHandler::getName() const
{
	return "PUIReqMsgHandler";
}