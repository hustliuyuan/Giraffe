#ifndef _GIRAFFE_PAXOS_PROPOSOR_H_
#define _GIRAFFE_PAXOS_PROPOSOR_H_

#include "GPaxosUtils.h"
#include "GPaxosState.h"
#include "logic/GPaxosComm.h"

class GPaxosState;
class GPaxosComm;

namespace PROPOSER_STATE
{
	enum
	{
		PREPARING,PROPOSING,ACCEPTING,
	};
};

typedef map<Network::PInetAddr,PaxosEvent> PromiseContainer;
typedef map<Network::PInetAddr,PaxosEvent> AcceptedContainer;



class GPaxosProposer
{
public:
	GPaxosProposer(GPaxosState* paxos_state);
	~GPaxosProposer();

	//void resetPaxosState();
	void initialize();
	void prepareIssue();
	void handlePromiseFrmAcceptor(PaxosEvent pe);
	void handleAcceptedFrmAcceptor(PaxosEvent pe);

private:
	GPaxosState *m_pPaxosState;
	GPaxosComm *m_pComm;
	unsigned int m_iProposerState;

	U32 m_iEpoch;
	U64 m_lTxid;
	U32 m_iMyID;
	Network::PInetAddr m_oMyAddr;

	PromiseContainer m_oPromiseContainer;
	AcceptedContainer m_oAcceptedContainer;
	PeerAddrContainer m_oPeersContainer;
	PeerAddrContainer m_oWaitForSntContainer;
	AcceptedCounter *m_pMyAcceptedContainer;
};

#endif//_GIRAFFE_PAXOS_PROPOSOR_H_
