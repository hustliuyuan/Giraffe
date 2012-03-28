#ifndef __PGRF_PAXOS_KEEPALIVE_REQ_PKT__
#define __PGRF_PAXOS_KEEPALIVE_REQ_PKT__

#include "Base/PPktDef.h"

class PGRFPaxosLeaderKeepAliveReq : public PPacketBase
{
public:
	PGRFPaxosLeaderKeepAliveReq(U8 subtype = PMyPktDef::GRF_PAXOS_KEEPALIVE_LEADER_REQ);
	PGRFPaxosLeaderKeepAliveReq(U32 id, U32 epoch,U8 subtype = PMyPktDef::GRF_PAXOS_KEEPALIVE_LEADER_REQ);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFPaxosLeaderKeepAliveReq(void);

	//GRF_KEEPALIVE_REQ|SESSION_ID|IP|PORT
public:
	U32 m_iServerID;
	U32 m_iEpoch;
};

#endif//__PGRF_PAXOS_KEEPALIVE_REQ_PKT__
