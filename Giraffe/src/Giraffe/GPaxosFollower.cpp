#include "GPaxosUtils.h"
#include "GPaxosState.h"
#include "GPaxosLeaderElection.h"
#include "logic/GPaxosComm.h"
#include "GPaxosFollower.h"
#include "Packets/Giraffe/paxos/PGRFPaxosPackets.h"

GPaxosFollower::GPaxosFollower(GPaxosState* paxos_state, GPaxosLeaderElection* election )
{
	m_bStop = true;
	m_pPaxosState = paxos_state;
	m_pComm = m_pPaxosState->getComm();
	assert(election);
	m_pElectionInstance = election;
	
	PSettings & settings = PSettings::getInstance();
	m_sLeaderLeasePeriod = settings.getConfig("giraffe-leader-lease-interval").m_value.intvalue;
}

GPaxosFollower::~GPaxosFollower()
{

}

void GPaxosFollower::handleEvent(PaxosEvent pe)
{
	if (m_pPaxosState->m_state != PAXOS_STATE::FOLLOWING)
	{
		//print warning
		cout << "unexpected event event type:" << GPaxosUtils::strEventType(pe.m_iEventType) << endl;
		return;
	}

	unsigned eventType = pe.m_iEventType;

	switch(eventType)
	{
	case PAXOS_EVENT::PING_FOLLOWER_EVENT:
		cout <<  " get leader ping"<< endl;
		m_sLastPingFromLeader = PUtils::localMilliSeconds();
		break;
	case PAXOS_EVENT::UAB_PROPOSAL_EVENT:
		handleProposeEvent(pe);
		break;

	case PAXOS_EVENT::UAB_COMMIT_EVENT:
		handleCommitEvent(pe);
		break;

	default:
		assert(0);
		break;
	}
}

void GPaxosFollower::pingToLeader()
{
	if (!m_bStop)
	{
		S64 time = PUtils::localMilliSeconds();
		if (time - m_sLastPingFromLeader <= m_sLeaderLeasePeriod)
		{
			cout << "time:" << time - m_sLastPingFromLeader << "period" << m_sLeaderLeasePeriod << endl; 
			cout << "ping to leader, epoch:" << m_pPaxosState->m_iLeaderEpoch<< "leader id:" <<m_pPaxosState->m_iLeaderId << endl;
			PPacketBase* pingPkt = (PGRFPaxosLeaderKeepAliveReq*)new PGRFPaxosLeaderKeepAliveReq();
			//PPacketBase* pingPkt = (PGRFPaxosEventPkt*)new PGRFPaxosEventPkt(PAXOS_EVENT::PING_LEADER_EVENT, m_pPaxosState->m_iMyid, m_pPaxosState->m_iCurrentEpoch, m_pPaxosState->m_iCurrentTxid, m_pPaxosState->m_oLocalPaxosAddr);
			m_pComm->tcpSend(m_pPaxosState->m_oLeaderAddr, pingPkt);

		}
		else//leader time out
		{
			startLeaderElection();
			m_bStop = true;
		}
	}
	else
	{
			m_sLastPingFromLeader = PUtils::localMilliSeconds();
			PPacketBase* pingPkt = (PGRFPaxosLeaderKeepAliveReq*)new PGRFPaxosLeaderKeepAliveReq();
			//PPacketBase* pingPkt = (PGRFPaxosEventPkt*)new PGRFPaxosEventPkt(PAXOS_EVENT::PING_LEADER_EVENT, m_pPaxosState->m_iMyid, m_pPaxosState->m_iCurrentEpoch, m_pPaxosState->m_iCurrentTxid, m_pPaxosState->m_oLocalPaxosAddr);
			m_pComm->tcpSend(m_pPaxosState->m_oLeaderAddr, pingPkt);
			m_bStop = false;
	}
}

void GPaxosFollower::startLeaderElection()
{
	cout << "leader time out!!! look for new leader!!" <<endl;
	m_pPaxosState->setLookerState();//reset state, increase epoch number 
	m_pElectionInstance->Initialize();// reset proposal issue
}

void GPaxosFollower::handleProposeEvent(PaxosEvent& pe)
{
	cout << "get uab event epoch:" << pe.m_iEpoch << "zxid: " << pe.m_lTxid << "serer id: " << pe.m_sValue << endl;
	if(pe.m_iEpoch < m_pPaxosState->m_iLeaderId || pe.m_lTxid < m_pPaxosState->m_iMaxCommitTxid)
	{
		cout << "get error broadcast event from leader" << endl;
		//may handle exception event here
		return;
	}
	//send ack to leader, and should write event to log and add event to commit_waiting queue
	////
	////

}

void GPaxosFollower::handleCommitEvent(PaxosEvent& pe)
{
	cout << "commit event epoch: " << pe.m_iEpoch << "zxid: " << pe.m_lTxid << "serer id: " << pe.m_sValue << endl;
	m_pPaxosState->m_iCurrentTxid = pe.m_lTxid;
	//validate the commit event 
	//if not ok, handle exception
	//else
	//MaxCommitTxid +1; set log item true and write value into fs
}

