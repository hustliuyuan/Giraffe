#include "GPaxosLeader.h"
#include "Packets/Giraffe/paxos/PGRFPaxosPackets.h"

GPaxosLeader::GPaxosLeader(GPaxosState* paxos_state,GPaxosLeaderElection* election)
{
	m_pPaxosState = paxos_state;
	m_pComm = m_pPaxosState->getComm();
	m_pElectionInstance = election;
}

GPaxosLeader::~GPaxosLeader()
{

}

void GPaxosLeader::handleEvent(PaxosEvent pe)
{
	if (m_pPaxosState->m_state != PAXOS_STATE::LEADING)
	{
		//print warning
		cout << "unexpected event event type:" << GPaxosUtils::strEventType(pe.m_iEventType) << endl;
		return;
	}
	unsigned eventType = pe.m_iEventType;

	switch(eventType)
	{
	case PAXOS_EVENT::PING_LEADER_EVENT:
		handlePingFrmFollower(pe);
		break;

	case PAXOS_EVENT::UAB_PROPREPARE_EVENT:
		cout << "get prepare event from follower!!" << endl;
		handlePrepareEvent(pe);
		break;

	case PAXOS_EVENT::UAB_ACK_EVENT:
		cout << "get ack from follower" << endl;
		handleAckEvent(pe);
		break;

	default :
		assert(0);
		break;
	}
}

//following function is used for checking follower state,
//check out expired followers and  then renew the global membership
void GPaxosLeader::renewFollowerMembership()
{
	cout << "i am leader, renew global membership " << endl;
}

void GPaxosLeader::handlePingFrmFollower(PaxosEvent &pe)
{
	cout << "ping to follower!!" << endl;
	PPacketBase* pingPkt = (PGRFPaxosLeaderKeepAliveRes*)new PGRFPaxosLeaderKeepAliveRes();
	//PPacketBase* pingPkt = (PGRFPaxosEventPkt*)new PGRFPaxosEventPkt(PAXOS_EVENT::PING_LEADER_EVENT, m_pPaxosState->m_iMyid, m_pPaxosState->m_iCurrentEpoch, m_pPaxosState->m_iCurrentTxid, m_pPaxosState->m_oLocalPaxosAddr);
	m_pComm->tcpSend(pe.m_oFrmAddr, pingPkt);
}

//handle follower prepare event 
void GPaxosLeader::handlePrepareEvent(PaxosEvent &pe)
{
	//prepare broadcast event; set max zxid + 1; get followers list and broadcast pkt to them
	PPacketBase* pingPkt = (PGRFBroadcastEventPkt*)new PGRFBroadcastEventPkt();
	//PPacketBase* pingPkt = (PGRFPaxosEventPkt*)new PGRFPaxosEventPkt(PAXOS_EVENT::PING_LEADER_EVENT, m_pPaxosState->m_iMyid, m_pPaxosState->m_iCurrentEpoch, m_pPaxosState->m_iCurrentTxid, m_pPaxosState->m_oLocalPaxosAddr);
	m_pComm->tcpSend(pe.m_oFrmAddr, pingPkt);
}

void GPaxosLeader::handleAckEvent(PaxosEvent &pe)
{
	//check whether this pe has commit 
	//if has committed then send commit event to the follower and return;
	//else increase the counter of this pe,if check counter greater than half number of follower, if more than half then broadcast the event to the followers who have sent ack  
}
