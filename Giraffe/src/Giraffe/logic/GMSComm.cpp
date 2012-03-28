#include "Base/GHeader.h"
#include "boost/bind.hpp"
#include "boost/thread.hpp"
#include "GMSPktHandler.h"
#include "GServerConfig.h"
#include "GMSPktParser.h"
#include "GMSComm.h"
#include "Giraffe/SessionManager.h"
#include "Base/PPktDef.h"

GMSComm::GMSComm(SessionManager * pMaster)
{	
	m_pTransmitter = new Network::PTransmitter();	
	m_handlerTask = new GMSPktHandler( m_pTransmitter, pMaster);
	m_pTransmitter->registerPacketHandler(  m_handlerTask ) ;

    //////////////////////////////////////////////////////////////////////////
    //packet parser initialize
	m_pMsPktParser = new GMSPktParser();
	PPacketFactory & pkt_factory_instance = PPacketFactory::getInstance();
    pkt_factory_instance.registerPktParser(PMyPktDef::MY_MAJOR_PKT, m_pMsPktParser,PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
	
   //////////////////////////////////////////////////////////////////////////
    //create network thread
	m_pPlatTransThreadFunc = new PPlatTransThreadFunc(m_pTransmitter, m_handlerTask);
	m_threadGroups.create_thread( boost::ref(*m_pPlatTransThreadFunc) );
}

GMSComm::~GMSComm(void)
{
}

Network::PTransmitter* GMSComm::getTransmitter()
{
	return m_pTransmitter;
}

void GMSComm::tcpSend( const PInetAddr & inAddr ,  const PPacketBase   * inPacket )
{
	m_pTransmitter->reliableSend (inAddr, inPacket);
}

void GMSComm::udpSend( const PInetAddr & inAddr , const PPacketBase * inPacket )
{
	m_pTransmitter->unreliableSend (inAddr, inPacket);
}

void GMSComm::setAppTerminate()
{
	((GMSPktHandler*)(m_handlerTask))->setAppTerminate(&(boost::ref(*m_pPlatTransThreadFunc).get()));
	PUtils::sleep( 1000 );
	LOG_DEBUG("comm","all thread exit");	
	m_threadGroups.join_all();
}

