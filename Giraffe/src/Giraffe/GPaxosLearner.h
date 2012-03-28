#ifndef _GIRAFFE_PAXOS_LEARNER_H_
#define _GIRAFFE_PAXOS_LEARNER_H_

#include "GPaxosState.h"
#include "GPaxosUtils.h"
#include "logic/GPaxosComm.h"

class GPaxosState;
class GPaxosComm;

class GPaxosLearner
{
public:
	GPaxosLearner(GPaxosState* paxos_state);
	~GPaxosLearner();

	void handleLeadingEvent(PaxosEvent pe);
	void handleLeadeRequest(PaxosEvent pe);
private:
	GPaxosState *m_pPaxosState;
	GPaxosComm *m_pComm;

	U32 m_iLeaderID;
	U32 m_iLeaderEpoch;
	U64 m_lLeaderTxid;
	Network::PInetAddr m_oLeaderAddr;
};

#endif //_GIRAFFE_PAXOS_LEARNER_H_
