#ifndef __PGRF_LEADER_LOCATION_RES_PKT__
#define __PGRF_LEADER_LOCATION_RES_PKT__

#include "Base/PPktDef.h"

class PGRFLeaderLocationResPkt : public PPacketBase
{
public:
	PGRFLeaderLocationResPkt(U8 subtype = PMyPktDef::GRF_LEADER_LOCATION_RES);
	PGRFLeaderLocationResPkt(U32 sessionID, const Network::PInetAddr& addr,U8 subtype = PMyPktDef::GRF_LEADER_LOCATION_RES);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFLeaderLocationResPkt(void);

	//GRF_KEEPALIVE_REQ|SESSION_ID|IP|PORT
public:
	U32 m_iSessionID;
	Network::PInetAddr    m_oAddr;
};

#endif//__PGRF_LEADER_LOCATION_RES_PKT__
