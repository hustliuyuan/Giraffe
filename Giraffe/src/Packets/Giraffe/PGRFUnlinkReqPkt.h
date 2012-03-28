#ifndef __PGRF_UNLINK_REQ_PKT__
#define __PGRF_UNLINK_REQ_PKT__

#include "Base/PPktDef.h"

class PGRFUnlinkReqPkt : public PPacketBase
{
public:
	PGRFUnlinkReqPkt(U8 subtype = PMyPktDef::GRF_UNLINK_REQ);
	PGRFUnlinkReqPkt(U32 e, U32 sessionID, String dirname ,U8 subtype = PMyPktDef::GRF_UNLINK_REQ);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFUnlinkReqPkt(void);
	//GRF_UNLINK_REQ|EVENT|DIRNAME
public:
	U32 m_iEvent;
	U32 m_iSessionID;
	String m_sDirName;
};

#endif//__PGRF_UNLINK_REQ_PKT__
