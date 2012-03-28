#include "Base/GHeader.h"
#include "boost/bind.hpp"
#include "boost/thread.hpp"
#include "GPaxosPktHandler.h"
#include "GServerConfig.h"
#include "Base/PPktDef.h"
#include "packet/internal/PInternalQuitApplicationPkt.h"
#include "GMSPktParser.h"
#include "GPaxosComm.h"
//#include "Giraffe/SessionManager.h"

GPaxosComm::GPaxosComm(GPaxosProtocol * pPaxos)
{
	m_pPaxos = pPaxos;

	m_pTransmitter = new Network::PTransmitter();	
	PSettings & settings = PSettings::getInstance();

	U16	m_iTcpPort = settings.getConfig("paxos-tcp-port").m_value.intvalue;
	U16 m_iUdpPort = settings.getConfig("paxos-udp-port").m_value.intvalue;

	m_handlerTask = new GPaxosPktHandler( m_pTransmitter, m_pPaxos);
	m_pTransmitter->registerPacketHandler(  m_handlerTask, m_iTcpPort, m_iUdpPort ) ;
	
    //parser packet register
	m_pPktParser = new GMSPktParser();
	PPacketFactory & pkt_factory_instance = PPacketFactory::getInstance();
    pkt_factory_instance.registerPktParser(PMyPktDef::MY_MAJOR_PKT, m_pPktParser,PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
	
    //create network thread
	m_pPlatTransThreadFunc = new PPlatTransThreadFunc(m_pTransmitter, m_handlerTask);
	m_thread_groups.create_thread( boost::ref(*m_pPlatTransThreadFunc) );
}


GPaxosComm::~GPaxosComm(void)
{
}

Network::PTransmitter* GPaxosComm::getTransmitter()
{
	return m_pTransmitter;
}

void GPaxosComm::tcpSend( const PInetAddr & inAddr ,  const PPacketBase   * inPacket )
{
	m_pTransmitter->reliableSend (inAddr, inPacket);
}

void GPaxosComm::udpSend( const PInetAddr & inAddr , const PPacketBase * inPacket )
{
	m_pTransmitter->unreliableSend (inAddr, inPacket);
}

void GPaxosComm::setAppTerminate()
{
	((GPaxosPktHandler*)(m_handlerTask))->setAppTerminate(&(boost::ref(*m_pPlatTransThreadFunc).get()));
	PUtils::sleep( 1000 );
	LOG_DEBUG("paxos protocol comm","all thread exit");	
	m_thread_groups.join_all();
}

