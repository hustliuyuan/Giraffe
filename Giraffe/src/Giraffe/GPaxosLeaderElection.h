#ifndef _GIRAFFE_PAXOS_LEADER_ELECTION_H_
#define _GIRAFFE_PAXOS_LEADER_ELECTION_H_

#include "GPaxosState.h"
#include "logic/GPaxosComm.h"
#include "GPaxosAcceptor.h"
#include "GPaxosLearner.h"
#include "GPaxosProposer.h"

class GPaxosAcceptor;
class GPaxosLearner;
class GPaxosProposer;

class GPaxosLeaderElection
{
public:
	GPaxosLeaderElection(GPaxosState* paxos_state);
	~GPaxosLeaderElection();

	void handlePaxosEvent(PaxosEvent pe);
	void electing();
	void Initialize();
private:
	GPaxosState *m_pPaxosState;
	GPaxosComm *m_pComm;
	GPaxosAcceptor * m_oAcceptor;
	GPaxosLearner * m_oLearner;
	GPaxosProposer * m_oProposer;
};

#endif //_GIRAFFE_PAXOS_LEADER_ELECTION_H_
