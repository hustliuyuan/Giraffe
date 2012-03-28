#ifndef __PGRF_ATTR_GET_REQ_PKT__
#define __PGRF_ATTR_GET_REQ_PKT__

#include "Base/PPktDef.h"
#include <vector>

using std::vector;

class PGRFAttrGetReqPkt : public PPacketBase
{
public:
	PGRFAttrGetReqPkt(U8 subtype = PMyPktDef::GRF_ATTR_GET_REQ);
	PGRFAttrGetReqPkt(U32 e,  U32 sessionID,U32 handle, U32 attrCount, vector<String> attrNames,U8 subtype = PMyPktDef::GRF_ATTR_GET_REQ);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFAttrGetReqPkt(void);

	//GRF_ATTR_GET_REQ|EVENT|HANDLE|ATTR_NAME

public:
	U32 m_event;
	U32 m_iSessionID;
	U32 m_iHandle;
	U32 m_iAttrCount;
	vector<String> m_attrNames;
};

#endif//__PGRF_ATTR_GET_REQ_PKT__

