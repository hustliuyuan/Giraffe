#ifndef  __GIRAFFE_PAXOS_UTILS_H__
#define  __GIRAFFE_PAXOS_UTILS_H__
#include "Base/GHeader.h"

class PaxosEvent;
class Paxos;
class AcceptedCounter;

typedef vector<Network::PInetAddr> WaitForSntContainer;
typedef vector<Network::PInetAddr> PeerAddrContainer;
//typedef Network::PInetAddr  PInetAddr;


namespace PAXOS_STATE
{
	enum 
	{
		LOOKING,LEADING,FOLLOWING
	};
};

namespace PAXOS_EVENT
{
	enum 
	{
		//leader election event
		PREPARE_EVENT,
		PROMISE_EVENT,
		ACCEPT_EVENT,
		ACCEPTED_EVENT,
		LEADER_EVENT,
		IS_LEADER_EVENT,

		PING_LEADER_EVENT,
		PING_FOLLOWER_EVENT,
		
		//broadcast event
		UAB_PROPREPARE_EVENT,
		UAB_PROPOSAL_EVENT,
		UAB_ACK_EVENT,
		UAB_COMMIT_EVENT,
	};
};


class TXID
{
public :
	static U64 encodeID(U32 epoch, U32 counter)
	{
		U64 txid = 0;
		txid = epoch;
		txid = (txid << 32 ) + counter;
		return txid;
	}

	static TXID decodeID(U64 txid)
	{
		TXID tmp;
		int mask = ~0;
		tmp.m_counter = (txid & mask);
		tmp.m_epoch = txid >> 32;

		return tmp;
	}

public :
	U32 m_epoch;
	U32 m_counter;

};

class AcceptedCounter
{
public:
	AcceptedCounter(U32 id, U64 txid, PInetAddr addr)
	{
		m_iServerId = id;
		m_lZxid =txid;
		m_oServerAddr = addr;
		m_iCounter = 0;
	};

	U32 addAccepted(PInetAddr acceptor)
	{
		vector<PInetAddr>::iterator it = m_oAcceptors.begin();

		for( ; it != m_oAcceptors.end(); it ++ )
		{
			if(*it == acceptor)
			{
				break;
			}
		}

		if(it == m_oAcceptors.end())
		{
			m_oAcceptors.push_back(acceptor);
			m_oWaitForSntAcceptors.push_back(acceptor);
			m_iCounter ++;
		}

		return m_iCounter;
	}
	WaitForSntContainer getAcceptors()
	{
		WaitForSntContainer table;
		table = m_oWaitForSntAcceptors;
		m_oWaitForSntAcceptors.clear();
		return table;
	}

	void reset(U64 txid)
	{
		m_oAcceptors.clear();
		m_iCounter = 0;
		m_lZxid = txid;
	}

public:
	U32 m_iServerId;
	U64 m_lZxid;
	PInetAddr m_oServerAddr;
	U32 m_iCounter;
	vector<PInetAddr> m_oAcceptors;
	WaitForSntContainer m_oWaitForSntAcceptors;

};

class PaxosEvent
{
public:
	PaxosEvent(U32 type, U32 id, U32 epoch, U64 txid, PInetAddr addr, PInetAddr frmAddr = NULL)
	{
		m_iEventType = type;
		m_iMyid = id;
		m_iEpoch = epoch;
		m_lTxid = txid;
		m_oLocAddr = addr;
		m_oFrmAddr	= frmAddr;
	};

	PaxosEvent(U32 type, U32 id, U32 epoch, U64 txid, PInetAddr frmAddr,string svalue)
	{
		m_iEventType = type;
		m_iMyid = id;
		m_iEpoch = epoch;
		m_lTxid = txid;
		m_oFrmAddr = frmAddr;
		m_sValue = svalue;
	};

public:
	U32 m_iEventType;
	U32 m_iMyid;
	U32 m_iEpoch;
	U64 m_lTxid;

	string m_sValue;
	PInetAddr m_oLocAddr;

	PInetAddr m_oFrmAddr;
};

class GPaxosUtils
{
public :
	static string strEventType(unsigned type)
	{
		string strType;
		switch(type)
		{
		case PAXOS_EVENT::ACCEPT_EVENT:
			strType = "ACCEPT_EVENT";
			break;
		case PAXOS_EVENT::ACCEPTED_EVENT:
			strType = "ACCEPTED_EVENT";
			break;
		case PAXOS_EVENT::IS_LEADER_EVENT:
			strType = "IS_LEADER_EVENT";
			break;
		case PAXOS_EVENT::LEADER_EVENT:
			strType = "LEADER_EVENT";
			break;
		case PAXOS_EVENT::PING_FOLLOWER_EVENT:
			strType = "PING_FOLLOWER_EVENT";
			break;
		case PAXOS_EVENT::PREPARE_EVENT:
			strType = "PREPARE_EVENT";
			break;
		case PAXOS_EVENT::PROMISE_EVENT:
			strType = "PROMISE_EVENT";
			break;
		/*
		case PAXOS_EVENT::UPDATE_COMMIT_EVENT:
			strType = "UPDATE_COMMIT_EVENT";
			break;
		case PAXOS_EVENT::UPDATE_PEPARE_EVENT:
			strType = "UPDATE_PEPARE_EVENT";
			break;
		case PAXOS_EVENT::UPDATE_PROMISE_EVENT:
			strType = "UPDATE_PROMISE_EVENT";
			break;
			*/
		default:
			strType = "UNKNOW TYPE";
			break;
		}
		return strType;
	}

};

#endif // __GIRAFFE_PAXOS_UTILS_H__
