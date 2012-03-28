#ifndef __PGRF_EXITS_RES_PKT__
#define __PGRF_EXITS_RES_PKT__

#include "Base/PPktDef.h"

class PGRFExitsResPkt : public PPacketBase
{
public:
	PGRFExitsResPkt(U8 subtype = PMyPktDef::GRF_EXITS_RES);
	PGRFExitsResPkt(U32 e, String name, B8 exit ,U8 subtype = PMyPktDef::GRF_EXITS_RES);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFExitsResPkt(void);

	//GRF_EXITS_RES|EVENT|YES     GRF_EXITS_RES|EVENT|NO
public:
	U32 m_event;
	String m_sName;
	B8 m_bExit;
};

#endif//__PGRF_EXITS_RES_PKT__
