#ifndef __PGRF_ATTR_GET_RES_PKT__
#define __PGRF_ATTR_GET_RES_PKT__

#include "Base/PPktDef.h"

typedef std::map<String, String> AttrMap;

class PGRFAttrGetResPkt : public PPacketBase
{
public:
	PGRFAttrGetResPkt(U8 subtype = PMyPktDef::GRF_ATTR_GET_RES);
	PGRFAttrGetResPkt(U32 e, U32 handle,U32 attrCount, AttrMap attrs ,U8 subtype = PMyPktDef::GRF_ATTR_GET_RES);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFAttrGetResPkt(void);

	//GRF_ATTR_GET_RES|EVENT|ATTR_VALUE
public:
	U32 m_event;
	U32 m_iHandle;
	U32 m_iAttrCount;
	AttrMap m_attrs;
};

#endif//__PGRF_ATTR_GET_RES_PKT__

