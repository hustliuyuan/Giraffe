#ifndef __PGRF_ATTR_SET_RES_PKT__
#define __PGRF_ATTR_SET_RES_PKT__

#include "Base/PPktDef.h"

class PGRFAttrSetResPkt : public PPacketBase
{
public:
	PGRFAttrSetResPkt(U8 subtype = PMyPktDef::GRF_ATTR_SET_RES);
	PGRFAttrSetResPkt(U32 e, U32 handle,B8 ok, U8 subtype = PMyPktDef::GRF_ATTR_SET_RES);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFAttrSetResPkt(void);

	//GRF_ATTR_SET_REQ|EVENT|HANDLE|ATTR_NAME|ATTR_VAL

public:
	U32 m_event;
	U32 m_iHandle;
	B8 m_bOk;
};

#endif//__PGRF_ATTR_SET_RES_PKT__

