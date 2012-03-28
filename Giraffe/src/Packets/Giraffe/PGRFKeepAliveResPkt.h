#ifndef __PGRF_KEEPALIVE_RES_PKT__
#define __PGRF_KEEPALIVE_RES_PKT__

#include "Base/PPktDef.h"

class PGRFKeepAliveResPkt : public PPacketBase
{
public:
	PGRFKeepAliveResPkt(U8 subtype = PMyPktDef::GRF_KEEPALIVE_RES);
	PGRFKeepAliveResPkt(U32 sessionID,U8 subtype = PMyPktDef::GRF_KEEPALIVE_RES);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFKeepAliveResPkt(void);
	//)GRF_KEEPALIVE_RES|SESSION_ID
public:
	U32 m_iSessionID;
};

#endif//__PGRF_KEEPALIVE_RES_PKT__
