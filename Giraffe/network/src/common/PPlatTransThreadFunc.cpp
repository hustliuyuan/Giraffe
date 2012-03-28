// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#include "common/Header.h"
#include "common/PPlatTransThreadFunc.h"
#include "common/IAppTerminate.h"


PPlatTransThreadFunc::PPlatTransThreadFunc( ITask *transmitter,
										   IPacketHandlerTask * pkt_handler,
										   ITask * uimsg_handler)
{
	m_bIsInitialized = false;
	this->m_bIsStop = false; 
	//m_pReaderTask = NULL;
	m_pPktHandler= pkt_handler;
	m_pUIMsgHandler = uimsg_handler ;
	m_pTaskQueue= NULL;	
	
	m_pITransmitter_task = transmitter; // added by tuxuping 2008-12-11

	if( !this->m_bIsInitialized ) 
	{
		this->initialize() ;
		this->m_bIsInitialized = true;
	}

	LOG_DEBUG("PPlatTransThreadFunc","PPlatTransThreadFunc(): this address:"<< this );
}

PPlatTransThreadFunc::~PPlatTransThreadFunc()
{
	LOG_DEBUG("PPlatTransThreadFunc","~PPlatTransThreadFunc(): this address:"<< this );

	delete m_pITransmitter_task; // added by tuxuping 2008-12-11
	//delete m_pReaderTask;
	delete m_pPktHandler;
	delete m_pUIMsgHandler;
	delete m_pTaskQueue;

}


void	PPlatTransThreadFunc::initialize( ) 
{
	this->m_pTaskQueue = new PTaskQueue();

	// network task;

	// commented by tuxuping , it is moved to outsider of this class, 
	// we should register pkthandler before we combine them together.
	//m_pITransmitter_task->registerPacketHandler(  m_pClient_packet_handler ) ;

	// add task together and let them in a thread function.
	m_pTaskQueue->addTask(m_pITransmitter_task );
	m_pTaskQueue->addTask(m_pPktHandler );
	// UI message handler is NULL by default. // tuxuping 2009-6-21
	if( m_pUIMsgHandler != NULL )
	  m_pTaskQueue->addTask(m_pUIMsgHandler);// added by tuxuping 2009-6-4

}

//void	PPlatTransThreadFunc::addTask( ITask *  task)
//{
//	m_pTaskQueue->addTask(task );
//}

void		PPlatTransThreadFunc::stop( ) 
{
	LOG_INFO("PPlatTransThreadFunc","Transmitter thread "<< this );
	this->m_bIsStop = true;	
}

void		PPlatTransThreadFunc::operator() ( ) 
{

    LOG_TRACE("PPlatTransThreadFunc","():  operator: " << this);

	while(! this->m_bIsStop ) 
	{			
		m_pTaskQueue->runOne();			
	}	

	LOG_TRACE("PPlatTransThreadFunc","(): PPlatTransThreadFunc thread quit. this address : " << this);
	
};

void	PPlatTransThreadFunc::applicationTerminate( )
{
	LOG_WARN("PPlatTransThreadFunc","applicationTerminate(): "<<
		"application is going to be terminated ");
	this->stop();
}

