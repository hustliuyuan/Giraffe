#include "Base/GHeader.h"
#include "GPaxosPktHandler.h"
#include "Base/PPktDef.h"
#include "GPaxosUtils.h"
#include "Packets/Giraffe/paxos/PGRFPaxosPackets.h"

typedef Network::PacketINFO PacketINFO;
typedef Network::PAddressable PAddressable;
typedef Network::PInetAddr  PInetAddr;

using namespace Network;

GPaxosPktHandler::GPaxosPktHandler(Network::PTransmitter	*transmiter/*, IAppTerminate * iappquit */)
{
	PSettings & settings = PSettings::getInstance();

	m_iIP = PUtils::addr2Long( settings.getConfig("local-ip").m_value.strvalue );
	m_iTcpPort = settings.getConfig("paxos-tcp-port").m_value.intvalue;
	m_iUdpPort = settings.getConfig("paxos-udp-port").m_value.intvalue;
	//m_curTime = PUtils::seconds();

	this->transmiter=transmiter;

	m_iInterval_time = settings.getConfig("query_address_interval-seconds").m_value.intvalue;
}

GPaxosPktHandler::GPaxosPktHandler(Network::PTransmitter	*transmiter, GPaxosProtocol* pPaxos/*,OraDBConnectPool* connPool*/)
{
	PSettings & settings = PSettings::getInstance();

	m_iIP = PUtils::addr2Long( settings.getConfig("local-ip").m_value.strvalue );

	this->transmiter=transmiter;

	m_iInterval_time = settings.getConfig("query_address_interval-seconds").m_value.intvalue;

	m_pPaxos = pPaxos;
}



void GPaxosPktHandler::handleInternalQuitApplication( const PacketINFO * packetInfo)
{
	this->m_pAppterminate->applicationTerminate();
}

GPaxosPktHandler::~GPaxosPktHandler() 
{
	
}

void GPaxosPktHandler::setAppTerminate(IAppTerminate * iappquit)
{
	m_pAppterminate = iappquit;
}

void	GPaxosPktHandler::maintain( )
{
	m_pPaxos->checkState();
}

void	GPaxosPktHandler::onRun( )
{
	maintain();
}

std::string		GPaxosPktHandler::getName()const
{
	return "GPAXOSPktHandler";
}


void	GPaxosPktHandler::handlePkt( const PacketINFO *  packetInfo)
{
	U8 type = packetInfo->packet->getType();
	U8 subtype = packetInfo->packet->getSubType();

	char temp[2046] = {0};
	packetInfo->toString(temp);
	LOG_DEBUG("GPaxosPktHandler","handlePkt():"<<temp);
	
	using namespace PMyPktDef;
	
	if( type == MY_MAJOR_PKT )
	{
		switch(subtype)
		{
		case GRF_PAXOS_EVENT:
			this->handlePaxosEvent(packetInfo);
			break;

		case GRF_PAXOS_KEEPALIVE_LEADER_REQ:
			this->handlePaxosKeepAliveReqEvent(packetInfo);
			break;

		case GRF_PAXOS_KEEPALIVE_LEADER_RES:
			this->handlePaxosKeepAliveResEvent(packetInfo);
			break;

		case GRF_BROADCAST_EVENT:
			this->handleBroadcastEvent(packetInfo);
			break;

		default:
			assert(0);
			break;
		}
	}
	deletePktInfo(packetInfo);
}

void GPaxosPktHandler::handlePaxosEvent( const PacketINFO * pktinfo)
{
	PInetAddr addr = pktinfo->addr;
	S8 temp[10240] = {0};
	pktinfo->packet->toString(temp);
	const PGRFPaxosEventPkt *pkt =  (const PGRFPaxosEventPkt *)pktinfo->packet;

	PaxosEvent pe = PaxosEvent(pkt->m_iPaxosType,pkt->m_iID,pkt->m_iEpoch,pkt->m_lTxid,pkt->m_oAddr, addr);
	m_pPaxos->dispatchPaxosEvent(pe);

}

void GPaxosPktHandler::handlePaxosKeepAliveReqEvent( const PacketINFO * pktinfo)
{
	PInetAddr addr = pktinfo->addr;
	S8 temp[10240] = {0};
	pktinfo->packet->toString(temp);
	const PGRFPaxosLeaderKeepAliveReq *pkt =  (const PGRFPaxosLeaderKeepAliveReq *)pktinfo->packet;
	
	PaxosEvent pe = PaxosEvent(PAXOS_EVENT::PING_LEADER_EVENT,pkt->m_iServerID , pkt->m_iEpoch ,0 ,0 , addr);
	m_pPaxos->dispatchPaxosEvent(pe);
}

void GPaxosPktHandler::handlePaxosKeepAliveResEvent( const PacketINFO * pktinfo)
{
	PInetAddr addr = pktinfo->addr;
	PaxosEvent pe = PaxosEvent(PAXOS_EVENT::PING_FOLLOWER_EVENT,0 ,0 ,0 ,0 , addr);
	m_pPaxos->dispatchPaxosEvent(pe);
}
void GPaxosPktHandler::handleBroadcastEvent( const PacketINFO * pktinfo)
{
	PInetAddr addr = pktinfo->addr;
	S8 temp[10240] = {0};
	pktinfo->packet->toString(temp);
	const PGRFBroadcastEventPkt *pkt =  (const PGRFBroadcastEventPkt *)pktinfo->packet;

	PaxosEvent pe = PaxosEvent(pkt->m_iPaxosType, pkt->m_iID ,pkt->m_iEpoch ,pkt->m_lTxid ,addr , pkt->m_sValue);
	m_pPaxos->dispatchPaxosEvent(pe);
}
