#ifndef __PGRF_LOCK_RES_PKT__
#define __PGRF_LOCK_RES_PKT__

#include "Base/PPktDef.h"

class PGRFLockResPkt : public PPacketBase
{
public:
	PGRFLockResPkt(U8 subtype = PMyPktDef::GRF_LOCK_RES);
	PGRFLockResPkt(U32 e, U32 handle, U32 lckStat, U32 lckGen = 0,U8 subtype = PMyPktDef::GRF_LOCK_RES);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFLockResPkt(void);

	/*
	*(11006)GRF_LOCK_RES|EVENT|LOCK_STATUS
	*(11006)GRF_LOCK_RES|EVENT|LOCK_STATUS_GRANTED|LOCK_GENERATION
	*(11006)GRF_LOCK_RES|EVENT|LOCK_STATUS_CONFLICT
	*/
public:
	U32 m_event;
	U32 m_iHandle;
	U32 m_iLockStatus;
	U32 m_iLockGeneration;
};

#endif//__PGRF_LOCK_RES_PKT__
