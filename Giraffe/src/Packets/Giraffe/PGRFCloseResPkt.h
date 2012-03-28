#ifndef __PGRF_CLOSE_RES_PKT__
#define __PGRF_CLOSE_RES_PKT__

#include "Base/PPktDef.h"

class PGRFCloseResPkt : public PPacketBase
{
public:
	PGRFCloseResPkt(U8 subtype = PMyPktDef::GRF_CLOSE_RES);
	PGRFCloseResPkt(U32 e, U32 handle, B8 ok,U8 subtype = PMyPktDef::GRF_CLOSE_RES);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFCloseResPkt(void);

	//GRF_CLOSE_REQ |EVENT|HANDLE
public:
	U32 m_event;
	U32 m_iHandle;
	B8 m_bOK;
};

#endif//__PGRF_CLOSE_RES_PKT__

