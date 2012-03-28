#ifndef _GIRAFFE_PAXOS_LEADER_H_
#define _GIRAFFE_PAXOS_LEADER_H_

#include "GPaxosUtils.h"
#include "GPaxosState.h"
#include "logic/GPaxosComm.h"
#include "GPaxosLeaderElection.h"

class GPaxosState;
class GPaxosComm;
class GPaxosLeaderElection;

class GPaxosLeader
{
public:
	GPaxosLeader(GPaxosState* paxos_state, GPaxosLeaderElection *election = NULL);
	~GPaxosLeader();

	void handleEvent(PaxosEvent pe);
	void renewFollowerMembership();
	void handlePingFrmFollower(PaxosEvent &pe);
	void handlePrepareEvent(PaxosEvent &pe);
	void handleAckEvent(PaxosEvent &pe);
private:
	GPaxosState *m_pPaxosState;
	GPaxosComm *m_pComm;
	GPaxosLeaderElection *m_pElectionInstance;
	map<U64, U32> m_oBroadcast;
};

#endif //_GIRAFFE_PAXOS_LEADER_H_
