#ifndef __PGRF_LOCK_REQ_PKT__
#define __PGRF_LOCK_REQ_PKT__

#include "Base/PPktDef.h"

class PGRFLockReqPkt : public PPacketBase
{
public:
	PGRFLockReqPkt(U8 subtype = PMyPktDef::GRF_LOCK_REQ);
	PGRFLockReqPkt(U32 e, U32 sessionID, U32 handle, U32 lckMode, B8 tryLock,U8 subtype = PMyPktDef::GRF_LOCK_REQ);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFLockReqPkt(void);
	//GRF_LOCK_REQ|EVENT|HANDLE|LOCK_MODE|TRYLOCK
public:
	U32 m_event;
	U32 m_iSessionID;
	U32 m_iHandle;
	U32 m_iLockMode;
	B8 m_bTryLock;
};

#endif//__PGRF_LOCK_REQ_PKT__
