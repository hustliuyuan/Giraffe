#ifndef _GIRAFFE_PAXOS_ACCEPTOR_H_
#define _GIRAFFE_PAXOS_ACCEPTOR_H_

#include "GPaxosState.h"
#include "logic/GPaxosComm.h"
class GPaxosState;
class GPaxosComm;

namespace ACCEPTOR_STATE
{
	enum
	{
		INITIAL,
		PROMISE,
		ACCEPTED,
	};
};

class GPaxosAcceptor
{
public:
	GPaxosAcceptor(GPaxosState* paxos_state);
	~GPaxosAcceptor();

	void handlePrepareFrmPoposer(PaxosEvent pe);
	void handleAcceptFrmProposer(PaxosEvent pe);

	void handleLeadingEvent(PaxosEvent pe);
	void handleLeadeRequest(PaxosEvent pe);

private:
	GPaxosState *m_pPaxosState;
	GPaxosComm *m_pComm;

	unsigned m_state;

	U32 m_iCurrentEpoch;
	U64 m_lCurrentTxid;
	U32 m_iCurrentID;
	Network::PInetAddr m_oCurrentAddr;
	U64 m_iRequestCounter;
};
#endif//_GIRAFFE_PAXOS_ACCEPTOR_H_
