#include "GPaxosLearner.h"
#include "Packets/Giraffe/paxos/PGRFPaxosPackets.h"

GPaxosLearner::GPaxosLearner(GPaxosState* paxos_state)
{
	m_pPaxosState = paxos_state;
	m_pComm = m_pPaxosState->getComm();
	m_iLeaderEpoch = 0;
	m_iLeaderID = 0;
	m_lLeaderTxid = 0;
}

GPaxosLearner::~GPaxosLearner()
{

}

void GPaxosLearner::handleLeadingEvent(PaxosEvent pe)
{
	if (m_pPaxosState->m_state == PAXOS_STATE::LOOKING)
	{
		m_iLeaderID = pe.m_iMyid;
		m_iLeaderEpoch = pe.m_iEpoch;
		m_lLeaderTxid = pe.m_lTxid;
		m_oLeaderAddr = pe.m_oLocAddr;
		
		unsigned state;
		if (m_iLeaderID == m_pPaxosState->m_iMyid)
		{
			state = PAXOS_STATE::LEADING;
		}
		else
		{
			state = PAXOS_STATE::FOLLOWING;
		}

		m_pPaxosState->setLeaderOrFolowerState(state,m_iLeaderID,m_oLeaderAddr,m_iLeaderEpoch,m_lLeaderTxid);
	}
	else
	{
		m_iLeaderID = m_pPaxosState->m_iLeaderId;
		m_iLeaderEpoch = m_pPaxosState->m_iLeaderEpoch;
		m_lLeaderTxid = m_pPaxosState->m_iMaxCommitTxid;
		m_oLeaderAddr = m_pPaxosState->m_oLeaderAddr;

		if (pe.m_iEpoch >= m_iLeaderEpoch&&(pe.m_lTxid > m_lLeaderTxid||pe.m_lTxid == m_lLeaderTxid&&m_iLeaderID < pe.m_iMyid))
		{
			
			{
				m_iLeaderID = pe.m_iMyid;
				m_iLeaderEpoch = pe.m_iEpoch;
				m_lLeaderTxid = pe.m_lTxid;
				m_oLeaderAddr = pe.m_oLocAddr;

				unsigned state;
				if (m_iLeaderID == m_pPaxosState->m_iMyid)
				{
					state = PAXOS_STATE::LEADING;
					cout << "i am leader, epoch:"<< m_iLeaderEpoch << endl;
				}
				else
				{
					state = PAXOS_STATE::FOLLOWING;
					cout << "i am follower, epoch:" << m_iLeaderEpoch << endl;
				}

				m_pPaxosState->setLeaderOrFolowerState(state,m_iLeaderID,m_oLeaderAddr,m_iLeaderEpoch,m_lLeaderTxid);
			}			
		}
		else
		{
			PPacketBase* leaderPkt = (PGRFPaxosEventPkt*)new PGRFPaxosEventPkt(PAXOS_EVENT::LEADER_EVENT, m_iLeaderID, m_iLeaderEpoch, m_lLeaderTxid, m_oLeaderAddr);
			m_pComm->tcpSend(pe.m_oFrmAddr, leaderPkt);
		}
	}
}

void GPaxosLearner::handleLeadeRequest(PaxosEvent pe)
{
	if (m_pPaxosState->m_state != PAXOS_STATE::LOOKING)
	{
		cout << "handle leader location" << endl;
		PPacketBase* leaderPkt = (PGRFPaxosEventPkt*)new PGRFPaxosEventPkt(PAXOS_EVENT::LEADER_EVENT, m_iLeaderID, m_iLeaderEpoch, m_lLeaderTxid, m_oLeaderAddr);
		m_pComm->tcpSend(pe.m_oFrmAddr, leaderPkt);
	}
	else
	{
		cout << "it is not leader now!!" << endl;
	}
}


