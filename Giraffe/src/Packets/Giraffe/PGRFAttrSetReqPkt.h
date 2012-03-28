#ifndef __PGRF_ATTR_SET_REQ_PKT__
#define __PGRF_ATTR_SET_REQ_PKT__

#include "Base/PPktDef.h"

class PGRFAttrSetReqPkt : public PPacketBase
{
public:
	PGRFAttrSetReqPkt(U8 subtype = PMyPktDef::GRF_ATTR_SET_REQ);
	PGRFAttrSetReqPkt(U32 e, U32 sessionID, U32 handle, String attrName, String attrVal, U8 subtype = PMyPktDef::GRF_ATTR_SET_REQ);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFAttrSetReqPkt(void);

	//GRF_ATTR_SET_REQ|EVENT|HANDLE|ATTR_NAME|ATTR_VAL

public:
	U32 m_event;
	U32 m_iSessionID;
	U32 m_iHandle;
	String m_sAttrName;
	String m_sAttrVal;
};

#endif//__PGRF_ATTR_SET_REQ_PKT__

