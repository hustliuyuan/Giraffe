#ifndef __PGRF_PAXOS_KEEPALIVE_RES_PKT__
#define __PGRF_PAXOS_KEEPALIVE_RES_PKT__

#include "Base/PPktDef.h"

class PGRFPaxosLeaderKeepAliveRes : public PPacketBase
{
public:
	PGRFPaxosLeaderKeepAliveRes(U8 subtype = PMyPktDef::GRF_PAXOS_KEEPALIVE_LEADER_RES);
	PGRFPaxosLeaderKeepAliveRes(U32 sessionID,U8 subtype = PMyPktDef::GRF_PAXOS_KEEPALIVE_LEADER_RES);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFPaxosLeaderKeepAliveRes(void);
	//)GRF_KEEPALIVE_RES|SESSION_ID
public:
	U32 m_iSessionID;
};

#endif//__PGRF_PAXOS_KEEPALIVE_RES_PKT__
