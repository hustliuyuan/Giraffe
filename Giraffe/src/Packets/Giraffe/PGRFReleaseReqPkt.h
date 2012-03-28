#ifndef __PGRF_RELEASE_REQ_PKT__
#define __PGRF_RELEASE_REQ_PKT__

#include "Base/PPktDef.h"

class PGRFReleaseReqPkt : public PPacketBase
{
public:
	PGRFReleaseReqPkt(U8 subtype = PMyPktDef::GRF_RELEASE_REQ);
	PGRFReleaseReqPkt(U32 e, U32 sessionID, U32 handle, U32 generation,U8 subtype = PMyPktDef::GRF_RELEASE_REQ);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFReleaseReqPkt(void);

	//GRF_RELEASE_REQ|EVENT|HANDLE|LOCK_GENERATION
public:
	U32 m_event;
	U32 m_iSessionID;
	U32 m_iHandle;
	U32 m_iLockGeneration;
};

#endif//__PGRF_Release_REQ_PKT__
