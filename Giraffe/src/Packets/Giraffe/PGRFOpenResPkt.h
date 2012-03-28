#ifndef __PGRF_OPEN_RES_PKT__
#define __PGRF_OPEN_RES_PKT__

#include "Base/PPktDef.h"

class PGRFOpenResPkt : public PPacketBase
{
public:
	PGRFOpenResPkt(U8 subtype = PMyPktDef::GRF_OPEN_RES);
	PGRFOpenResPkt(U32 e, U32 handle, U8 subtype = PMyPktDef::GRF_OPEN_RES);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFOpenResPkt(void);
	//GRF_OPEN_RES|EVENT|HANDLE
public:
	U32 m_event;
	U32 m_iHandle;
};

#endif//__PGRF_OPEN_RES_PKT__
