#include "GPaxosUtils.h"
#include "Packets/Giraffe/paxos/PGRFPaxosPackets.h"
#include "GPaxosAcceptor.h"

GPaxosAcceptor::GPaxosAcceptor(GPaxosState* paxos_state)
{
	m_state = ACCEPTOR_STATE::INITIAL;
	m_pPaxosState = paxos_state;
	m_pComm = m_pPaxosState->getComm();
	m_iCurrentID = 0;
	m_iCurrentEpoch = 0;
	m_lCurrentTxid = 0;
	m_iRequestCounter = 0;
}

GPaxosAcceptor::~GPaxosAcceptor()
{

}

void GPaxosAcceptor::handleAcceptFrmProposer(PaxosEvent pe)
{
	cout << "get accept from proposer!!" <<endl;
	if(m_pPaxosState->m_state!= PAXOS_STATE::LOOKING && pe.m_iEpoch > m_iCurrentEpoch)
	{
		m_state = ACCEPTOR_STATE::ACCEPTED;
		PPacketBase* acceptedPkt = (PGRFPaxosEventPkt*)new PGRFPaxosEventPkt(PAXOS_EVENT::ACCEPTED_EVENT, m_iCurrentID, m_iCurrentEpoch, m_lCurrentTxid, m_oCurrentAddr);
		m_pComm->tcpSend(pe.m_oFrmAddr, acceptedPkt);
		return ;
	}

	if (m_iCurrentID == pe.m_iMyid && m_iCurrentEpoch == pe.m_iEpoch )
	{
		m_state = ACCEPTOR_STATE::ACCEPTED;
		PPacketBase* acceptedPkt = (PGRFPaxosEventPkt*)new PGRFPaxosEventPkt(PAXOS_EVENT::ACCEPTED_EVENT, m_iCurrentID, m_iCurrentEpoch, m_lCurrentTxid, m_oCurrentAddr);
		m_pComm->tcpSend(pe.m_oFrmAddr, acceptedPkt);
	}
	else
	{
		U32 eventType;
		if (m_state == ACCEPTOR_STATE::ACCEPTED&&m_pPaxosState->m_state != PAXOS_STATE::LOOKING)
		{
			//eventType = PAXOS_EVENT::ACCEPTED_EVENT;
			eventType = PAXOS_EVENT::LEADER_EVENT;
		}
		else
		{
			eventType = PAXOS_EVENT::PROMISE_EVENT;
		}

		PPacketBase* promisePkt = (PGRFPaxosEventPkt*)new PGRFPaxosEventPkt(eventType, m_iCurrentID, m_iCurrentEpoch, m_lCurrentTxid, m_oCurrentAddr);
		m_pComm->tcpSend(pe.m_oFrmAddr, promisePkt);
	}
}

void GPaxosAcceptor::handlePrepareFrmPoposer(PaxosEvent pe)
{
	unsigned eventType;
	if(m_pPaxosState->m_state!= PAXOS_STATE::LOOKING && pe.m_iEpoch > m_iCurrentEpoch)
	{
		m_state = ACCEPTOR_STATE::ACCEPTED;
		PPacketBase* acceptedPkt = (PGRFPaxosEventPkt*)new PGRFPaxosEventPkt(PAXOS_EVENT::ACCEPTED_EVENT, m_iCurrentID, m_iCurrentEpoch, m_lCurrentTxid, m_oCurrentAddr);
		m_pComm->tcpSend(pe.m_oFrmAddr, acceptedPkt);
		return ;
	}

	if (m_iCurrentEpoch < pe.m_iEpoch||(m_iCurrentEpoch == pe.m_iEpoch && (m_lCurrentTxid < pe.m_lTxid || (m_lCurrentTxid == pe.m_lTxid && m_iCurrentID < pe.m_iMyid))))
	{
		cout << "proposal promise id: "<< pe.m_iMyid << " addr: " << pe.m_oLocAddr.toString() << endl;
		if (m_pPaxosState->m_state == PAXOS_STATE::LOOKING)
		{
			m_iCurrentEpoch = pe.m_iEpoch;
			m_lCurrentTxid = pe.m_lTxid;
			m_iCurrentID = pe.m_iMyid;
			m_oCurrentAddr = pe.m_oLocAddr;
			m_state = ACCEPTOR_STATE::PROMISE;	
		}
		else
		{
			eventType = PAXOS_EVENT::LEADER_EVENT;
		}

		PPacketBase* promisePkt = (PGRFPaxosEventPkt*)new PGRFPaxosEventPkt(eventType, m_iCurrentID, m_iCurrentEpoch, m_lCurrentTxid, m_oCurrentAddr);
		m_pComm->tcpSend(pe.m_oFrmAddr, promisePkt);
		return;
			
	}
	if (m_iCurrentEpoch >= pe.m_iEpoch&&m_state == ACCEPTOR_STATE::ACCEPTED&&m_pPaxosState->m_state != PAXOS_STATE::LOOKING)
	{
		//eventType = PAXOS_EVENT::ACCEPTED_EVENT;
		eventType = PAXOS_EVENT::LEADER_EVENT;
	}
	else
	{
		eventType = PAXOS_EVENT::PROMISE_EVENT;
	}

	PPacketBase* promisePkt = (PGRFPaxosEventPkt*)new PGRFPaxosEventPkt(eventType, m_iCurrentID, m_iCurrentEpoch, m_lCurrentTxid, m_oCurrentAddr);
	m_pComm->tcpSend(pe.m_oFrmAddr, promisePkt);
}


void GPaxosAcceptor::handleLeadingEvent(PaxosEvent pe)
{
	if (m_pPaxosState->m_state == PAXOS_STATE::LOOKING)
	{
		m_iCurrentEpoch = pe.m_iEpoch;
		m_lCurrentTxid = pe.m_lTxid;
		m_iCurrentID = pe.m_iMyid;
		m_oCurrentAddr = pe.m_oLocAddr;

		unsigned state;
		if (m_iCurrentID == m_pPaxosState->m_iMyid)
		{
			state = PAXOS_STATE::LEADING;
		}
		else
		{
			state = PAXOS_STATE::FOLLOWING;
		}

		m_pPaxosState->setLeaderOrFolowerState(state,m_iCurrentID,m_oCurrentAddr,m_iCurrentEpoch,m_lCurrentTxid);
	}
	else
	{
		m_iCurrentID = m_pPaxosState->m_iLeaderId;
		m_iCurrentEpoch = m_pPaxosState->m_iCurrentEpoch;
		m_lCurrentTxid = m_pPaxosState->m_iMaxCommitTxid;
		m_oCurrentAddr = m_pPaxosState->m_oLeaderAddr;

		if (pe.m_iEpoch >= m_iCurrentEpoch&&(pe.m_lTxid > m_lCurrentTxid||pe.m_lTxid == m_lCurrentTxid&&m_iCurrentID < pe.m_iMyid))
		{

			{
				m_iCurrentID = pe.m_iMyid;
				m_iCurrentEpoch = pe.m_iEpoch;
				m_lCurrentTxid = pe.m_lTxid;
				m_oCurrentAddr = pe.m_oLocAddr;

				unsigned state;
				if (m_iCurrentID == m_pPaxosState->m_iMyid)
				{
					state = PAXOS_STATE::LEADING;
					cout << "i am leader, epoch:"<< m_iCurrentEpoch << endl;
				}
				else
				{
					state = PAXOS_STATE::FOLLOWING;
					cout << "i am follower, epoch:" << m_iCurrentEpoch << endl;
				}

				m_pPaxosState->setLeaderOrFolowerState(state,m_iCurrentID,m_oCurrentAddr,m_iCurrentEpoch,m_lCurrentTxid);
			}			
		}
		else
		{
			PPacketBase* leaderPkt = (PGRFPaxosEventPkt*)new PGRFPaxosEventPkt(PAXOS_EVENT::LEADER_EVENT, m_iCurrentID, m_iCurrentEpoch, m_lCurrentTxid, m_oCurrentAddr);
			m_pComm->tcpSend(pe.m_oFrmAddr, leaderPkt);
		}
	}
}

void GPaxosAcceptor::handleLeadeRequest(PaxosEvent pe)
{
	if (m_pPaxosState->m_state != PAXOS_STATE::LOOKING)
	{
		cout << "handle leader location" << endl;
		PPacketBase* leaderPkt = (PGRFPaxosEventPkt*)new PGRFPaxosEventPkt(PAXOS_EVENT::LEADER_EVENT, m_iCurrentID, m_iCurrentEpoch, m_lCurrentTxid, m_oCurrentAddr);
		m_pComm->tcpSend(pe.m_oFrmAddr, leaderPkt);
	}
	else
	{
		cout << "it is not leader now!!" << endl;
	}
}
