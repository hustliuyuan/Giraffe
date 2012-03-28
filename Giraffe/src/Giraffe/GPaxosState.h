#ifndef _GIRAFFE_PAXOS_STATE_H_
#define _GIRAFFE_PAXOS_STATE_H_

#include "network.h"
#include "GPaxosUtils.h"
#include "logic/GPaxosComm.h"

typedef vector<Network::PInetAddr> PeerAddrContainer;

class GPaxosComm;

class GPaxosState
{
public:
	GPaxosState(GPaxosComm *comm);

	~GPaxosState(void);

	bool get_leader_location(Network::PInetAddr	&addr);
	bool setLeaderOrFolowerState(unsigned state, U32 leaderId, PInetAddr leader, U32 epoch, U64 txid);// for election instance to set leader state
	bool setLookerState();// for follower to restart election instance
	void setComm(GPaxosComm* comm);
	GPaxosComm *getComm();

public:
	int m_state;//LOOKING; FOLLOWING; LEADING
	U32 m_iMyid;//id for server

	U32 m_iLeaderEpoch;//current leader epoch
	U32 m_iLeaderId;	// current leader id = leader's my id
	U64 m_iMaxCommitTxid; // commit id increase when log commit an item, could be used for electing new leader

	U64 m_iPrevTxid;	//useless
	U32 m_iCurrentEpoch;	//current epoch
	U64 m_iCurrentTxid;	
	U32 m_iTxCounter;

	PInetAddr m_oLocalPaxosAddr;
	PInetAddr m_oLeaderAddr;

	PeerAddrContainer m_oReplicaTable;	// a dynamic table
	U32 m_iPaxosSyncLimit;
	GPaxosComm *m_oComm;
};
#endif //_GIRAFFE_PAXOS_STATE_H_

