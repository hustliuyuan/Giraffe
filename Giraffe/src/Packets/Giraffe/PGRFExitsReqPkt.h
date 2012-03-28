#ifndef __PGRF_EXITS_REQ_PKT__
#define __PGRF_EXITS_REQ_PKT__

#include "Base/PPktDef.h"

class PGRFExitsReqPkt : public PPacketBase
{
public:
	PGRFExitsReqPkt(U8 subtype = PMyPktDef::GRF_EXITS_REQ);
	PGRFExitsReqPkt(U32 e, U32 sessionID, String name,U8 subtype = PMyPktDef::GRF_EXITS_REQ);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFExitsReqPkt(void);

	//GRF_EXITS_REQ|EVENT|SESSIONid|NAME
public:
	U32 m_event;
	U32 m_iSessionID;
	String m_sFileName;
};

#endif//__PGRF_EXITS_REQ_PKT__
