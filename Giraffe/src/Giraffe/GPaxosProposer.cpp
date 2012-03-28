#include "GPaxosProposer.h"
#include "Packets/Giraffe/paxos/PGRFPaxosPackets.h"

GPaxosProposer::GPaxosProposer(GPaxosState* paxos_state)
{
	assert(paxos_state);
	m_pPaxosState = paxos_state;
	m_pComm = m_pPaxosState->getComm();
	m_oPeersContainer = m_pPaxosState->m_oReplicaTable;
	m_pMyAcceptedContainer = NULL;
	initialize();
}

GPaxosProposer::~GPaxosProposer()
{
}

void GPaxosProposer::initialize()
{
	m_iMyID = m_pPaxosState->m_iMyid;
	m_lTxid = m_pPaxosState->m_iMaxCommitTxid;
	m_oMyAddr = m_pPaxosState->m_oLocalPaxosAddr;
	m_iEpoch = m_pPaxosState->m_iCurrentEpoch;
	if (m_pMyAcceptedContainer == NULL)
	{
		m_pMyAcceptedContainer = new AcceptedCounter(m_iMyID, m_lTxid, m_oMyAddr);
	}
	else
	{
		m_pMyAcceptedContainer->reset(m_lTxid);
	}
	
}

void GPaxosProposer::prepareIssue()
{
	if (m_pPaxosState->m_state == PAXOS_STATE::LOOKING)
	{
		cout << "PROPOSER: propose issue ,epoch: " << m_iEpoch << " txid: "<< m_lTxid << " server id: " << m_iMyID << endl;
		PeerAddrContainer::iterator it = m_oPeersContainer.begin();
		PInetAddr addrSentTo;
		for(; it != m_oPeersContainer.end(); it ++)
		{
			addrSentTo = *it;
			PPacketBase* preparePkt = (PGRFPaxosEventPkt*) new PGRFPaxosEventPkt(PAXOS_EVENT::PREPARE_EVENT, m_iMyID, m_iEpoch, m_lTxid, m_oMyAddr);
			m_pComm->tcpSend(addrSentTo, preparePkt);				
		}	
		
		{
			PaxosEvent pe(PAXOS_EVENT::PROMISE_EVENT, m_iMyID, m_iEpoch, m_lTxid, m_oMyAddr);
			//m_oPromiseContainer.insert(pair<PInetAddr, PaxosEvent>(m_oMyAddr, pe));
		}
	}
}

void GPaxosProposer::handlePromiseFrmAcceptor(PaxosEvent pe)
{
	if (m_pPaxosState->m_state == PAXOS_STATE::LOOKING)
	{
		if (pe.m_iMyid == m_iMyID)
		{
			cout << "PROPOSER: get promise from accptor ,epoch: " << m_iEpoch << " txid: "<< m_lTxid << " server id: " << m_iMyID << endl;
			PromiseContainer::iterator it = m_oPromiseContainer.find(pe.m_oFrmAddr);
			if (it == m_oPromiseContainer.end())
			{
				m_oPromiseContainer.insert(pair<PInetAddr, PaxosEvent>(pe.m_oFrmAddr, pe));
				m_oWaitForSntContainer.push_back(pe.m_oFrmAddr);
			}
			else
			{
				if (it->second.m_iEpoch < pe.m_iEpoch || it->second.m_lTxid < pe.m_lTxid)
				{
					it->second = pe;
				}
			}

			if (m_oPromiseContainer.size() >= m_oPeersContainer.size()/2)
			{
				//m_pMyAcceptedContainer->addAccepted(pe.m_oFrmAddr);
				cout << "PROPOSER: post accept request to acceptor ,epoch: " << m_iEpoch << " txid: "<< m_lTxid << " server id: " << m_iMyID << endl;
				m_iProposerState = PROPOSER_STATE::PROPOSING;
				PeerAddrContainer::iterator peer_it = m_oWaitForSntContainer.begin();

				for(; peer_it != m_oWaitForSntContainer.end(); peer_it ++)
				{
					PInetAddr addrSentTo = *peer_it;
					PPacketBase* preparePkt = (PGRFPaxosEventPkt*)new PGRFPaxosEventPkt(PAXOS_EVENT::ACCEPT_EVENT, m_iMyID, m_iEpoch, m_lTxid, m_oMyAddr);
					m_pComm->tcpSend(addrSentTo, preparePkt);
				}

				m_oWaitForSntContainer.clear();
			}

			//add paxos event to its acceptor

		}
		else if (pe.m_iEpoch > m_iEpoch)
		{
			cout << "PROPOSER: NACK from acceptor ,epoch: " << pe.m_iEventType << " txid: "<< pe.m_iEventType << " server id: " << pe.m_iMyid << endl;
			m_iEpoch = pe.m_iEpoch;
			m_oWaitForSntContainer.clear();
			m_oPromiseContainer.clear();
			m_pMyAcceptedContainer->reset(m_lTxid);
		}
	}
}

void GPaxosProposer::handleAcceptedFrmAcceptor(PaxosEvent pe)
{
	if (m_pPaxosState->m_state == PAXOS_STATE::LOOKING)
	{
		if (pe.m_iMyid == m_iMyID)
		{
			if (pe.m_iEpoch == m_iEpoch)
			{
				unsigned cnt = m_pMyAcceptedContainer->addAccepted(pe.m_oFrmAddr);
				if (cnt >= m_pPaxosState->m_oReplicaTable.size()/2)
				{
					cout << "i am leader now!!epoch: " << m_iEpoch + 1 << "txid:" << m_lTxid<< endl;
					m_pPaxosState->setLeaderOrFolowerState(PAXOS_STATE::LEADING, m_iMyID, m_oMyAddr, m_iEpoch + 1, m_lTxid);
					WaitForSntContainer container = m_pMyAcceptedContainer->getAcceptors();
					WaitForSntContainer::iterator it = container.begin();

					for(; it != container.end(); it ++)
					{
						PInetAddr addrSentTo = *it;
						PPacketBase* leaderPkt = (PGRFPaxosEventPkt*)new PGRFPaxosEventPkt(PAXOS_EVENT::LEADER_EVENT, m_iMyID, m_iEpoch + 1, m_lTxid, m_oMyAddr);
						m_pComm->tcpSend(addrSentTo, leaderPkt);
					}
				}
			}
		}
		else
		{
			if (m_iEpoch < pe.m_iEpoch && m_lTxid > pe.m_lTxid)
			{
				m_iEpoch = pe.m_iEpoch;
			}
			else
			{
				cout << "leader location request!!! id: " << pe.m_iMyid << "txid:" << pe.m_lTxid << endl;
				PPacketBase* isLeaderPkt = (PGRFPaxosEventPkt*)new PGRFPaxosEventPkt(PAXOS_EVENT::IS_LEADER_EVENT, pe.m_iMyid, pe.m_iEpoch, pe.m_lTxid, pe.m_oLocAddr);
				m_pComm->tcpSend(pe.m_oLocAddr, isLeaderPkt);
				cout << pe.m_oLocAddr.toString()<<endl;;
			}
		}
	}
}



