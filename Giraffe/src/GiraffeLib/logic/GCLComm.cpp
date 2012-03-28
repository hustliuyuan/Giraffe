#include "boost/bind.hpp"
#include "boost/thread.hpp"
#include "logic/GCLPktHandler.h"
#include "config/GClientConfig.h"
#include "Base/PPktDef.h"
#include "packet/internal/PInternalQuitApplicationPkt.h"
#include "logic/GCLPktParser.h"
#include "Packets/Giraffe/PGRFPackets.h"
#include "Session.h"
#include "GCLComm.h"

GCLComm::GCLComm(Session * pSession)
{	
	m_pTransmitter = new Network::PTransmitter();	
	m_handlerTask = new GCLPktHandler( m_pTransmitter, pSession);
	m_pTransmitter->registerPacketHandler(  m_handlerTask ) ;

    //client packet parser register
	m_pCLPktParser = new GCLPktParser();
	PPacketFactory & pkt_factory_instance = PPacketFactory::getInstance();
    pkt_factory_instance.registerPktParser(PMyPktDef::MY_MAJOR_PKT, m_pCLPktParser,PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
	
    //create network thread
	m_pPlatTransThreadFunc = new PPlatTransThreadFunc(m_pTransmitter, m_handlerTask);
	m_threadGroups.create_thread( boost::ref(*m_pPlatTransThreadFunc) );
}


GCLComm::~GCLComm(void)
{
}

Network::PTransmitter* GCLComm::getTransmitter()
{
	return m_pTransmitter;
}

void GCLComm::tcpSend( const PInetAddr & inAddr ,  const PPacketBase   * inPacket )
{
	m_pTransmitter->reliableSend (inAddr, inPacket);
}

void GCLComm::udpSend( const PInetAddr & inAddr , const PPacketBase * inPacket )
{
	m_pTransmitter->unreliableSend (inAddr, inPacket);
}

void GCLComm::setAppTerminate()
{
	((GCLPktHandler*)(m_handlerTask))->setAppTerminate(&(boost::ref(*m_pPlatTransThreadFunc).get()));
	PUtils::sleep( 1000 );
	LOG_DEBUG("comm","all thread exit");	
	m_threadGroups.join_all();
}

