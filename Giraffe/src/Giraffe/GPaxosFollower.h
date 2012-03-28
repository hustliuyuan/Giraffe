#ifndef _GIRAFFE_PAXOS_FOLLOWER_H_
#define _GIRAFFE_PAXOS_FOLLOWER_H_

class GPaxosState;
class GPaxosComm;
class GPaxosLeaderElection;

class GPaxosFollower
{
public:
	GPaxosFollower(GPaxosState* paxos_state, GPaxosLeaderElection *election);
	~GPaxosFollower();

	void handleEvent(PaxosEvent pe);
	void pingToLeader();
	void handleProposeEvent(PaxosEvent& pe);
	void handleCommitEvent(PaxosEvent& pe);
private:
	void startLeaderElection();


private:
	GPaxosState *m_pPaxosState;
	GPaxosComm *m_pComm;
	GPaxosLeaderElection *m_pElectionInstance;

	S64 m_sLastPingFromLeader;
	S64 m_sLeaderLeasePeriod;

	bool m_bStop;//true when leader timeout
};

#endif //_GIRAFFE_PAXOS_FOLLOWER_H_
