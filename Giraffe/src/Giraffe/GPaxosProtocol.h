#ifndef _GIRAFFE_PAXOS_PROTOCOL_H_
#define _GIRAFFE_PAXOS_PROTOCOL_H_

#include "network.h"
#include "GPaxosUtils.h"
#include "logic/GPaxosComm.h"
#include "GPaxosLeaderElection.h"
#include "GPaxosLeader.h"
#include "GPaxosFollower.h"
#include "GPaxosState.h"

class GPaxosComm;
class GPaxosLeaderElection;
class GPaxosFollower;
class GPaxosLeader;

class GPaxosProtocol
{
public:
	GPaxosProtocol();
	~GPaxosProtocol();

	void start();
	void checkState();
	void dispatchPaxosEvent(PaxosEvent pe);
	void setAppTerminate();

public:
	GPaxosComm * m_paxosComm;
	Paxos * m_paxos;
	S64 m_curTime;
	U32 m_paxosSyncLimit;
	//Network::PTransmitter	*m_transmiter;
	bool m_is_cluster;
	GPaxosState *m_pPaxosState;
	GPaxosLeaderElection *m_pElectionInstance;
	GPaxosLeader *m_pLeaderInstance;
	GPaxosFollower *m_pFollowerInstance;
	bool m_bInitialized;
};

#endif //_GIRAFFE_PAXOS_PROTOCOL_H_

