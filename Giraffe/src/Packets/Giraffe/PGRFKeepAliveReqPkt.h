#ifndef __PGRF_KEEPALIVE_REQ_PKT__
#define __PGRF_KEEPALIVE_REQ_PKT__

#include "Base/PPktDef.h"

class PGRFKeepAliveReqPkt : public PPacketBase
{
public:
	PGRFKeepAliveReqPkt(U8 subtype = PMyPktDef::GRF_KEEPALIVE_REQ);
	PGRFKeepAliveReqPkt(U32 sessionID, const Network::PInetAddr& addr,U8 subtype = PMyPktDef::GRF_KEEPALIVE_REQ);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFKeepAliveReqPkt(void);

	//GRF_KEEPALIVE_REQ|SESSION_ID|IP|PORT
public:
	U32 m_iSessionID;
	Network::PInetAddr    m_oAddr;
};

#endif//__PGRF_KEEPALIVE_REQ_PKT__
