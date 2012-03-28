#ifndef __PGRF_CLOSE_REQ_PKT__
#define __PGRF_CLOSE_REQ_PKT__

#include "Base/PPktDef.h"

class PGRFCloseReqPkt : public PPacketBase
{
public:
	PGRFCloseReqPkt(U8 subtype = PMyPktDef::GRF_CLOSE_REQ);
	PGRFCloseReqPkt(U32 e, U32 sessionID, U32 handle,U8 subtype = PMyPktDef::GRF_CLOSE_REQ);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFCloseReqPkt(void);

	//GRF_CLOSE_REQ |EVENT|HANDLE
public:
	U32 m_event;
	U32 m_iSessionID;
	U32 m_iHandle;
};

#endif//__PGRF_CLOSE_REQ_PKT__
