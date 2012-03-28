#ifndef __PGRF_LEADER_LOCATION_REQ_PKT__
#define __PGRF_LEADER_LOCATION_REQ_PKT__

#include "Base/PPktDef.h"

class PGRFLeaderLocationReqPkt : public PPacketBase
{
public:
	PGRFLeaderLocationReqPkt(U8 subtype = PMyPktDef::GRF_LEADER_LOCATION_REQ);
	PGRFLeaderLocationReqPkt(U32 eventid,U8 subtype = PMyPktDef::GRF_LEADER_LOCATION_REQ);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFLeaderLocationReqPkt(void);

public:
	U32 m_iSessionId;
};

#endif//__PGRF_LEADER_LOCATION_REQ_PKT__
