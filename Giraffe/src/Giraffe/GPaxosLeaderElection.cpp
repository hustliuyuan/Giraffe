#include "GPaxosLeaderElection.h"

GPaxosLeaderElection::GPaxosLeaderElection(GPaxosState* paxos_state)
{
	assert(paxos_state);
	m_pPaxosState = paxos_state;
	m_pComm = m_pPaxosState->getComm();
	m_oLearner = new GPaxosLearner(paxos_state);
	m_oAcceptor = new GPaxosAcceptor(paxos_state);
	m_oProposer = new GPaxosProposer(paxos_state);
}

GPaxosLeaderElection::~GPaxosLeaderElection()
{

}

void GPaxosLeaderElection::handlePaxosEvent(PaxosEvent pe)
{
	unsigned eventType = pe.m_iEventType;

	switch(eventType)
	{
	case PAXOS_EVENT::PREPARE_EVENT:
		m_oAcceptor->handlePrepareFrmPoposer(pe);
		break;

	case PAXOS_EVENT::PROMISE_EVENT:
		m_oProposer->handlePromiseFrmAcceptor(pe);
		break;

	case PAXOS_EVENT::ACCEPT_EVENT:
		m_oAcceptor->handleAcceptFrmProposer(pe);
		break;

	case PAXOS_EVENT::ACCEPTED_EVENT:
		m_oProposer->handleAcceptedFrmAcceptor(pe);
		break;

	case PAXOS_EVENT::LEADER_EVENT:
		m_oAcceptor->handleLeadingEvent(pe);
		break;
	case PAXOS_EVENT::IS_LEADER_EVENT:
		cout << "handle leader event" <<endl;
		m_oAcceptor->handleLeadeRequest(pe);
		break;
	default:
		assert(0);
	}
}

void GPaxosLeaderElection::electing()
{
	//cout << "propose issue!!" << endl;
	m_oProposer->prepareIssue();
}

void GPaxosLeaderElection::Initialize()
{
	m_oProposer->initialize();
}
