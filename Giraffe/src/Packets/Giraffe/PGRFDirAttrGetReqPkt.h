#ifndef __PGRF_DIR_ATTR_GET_REQ_PKT__
#define __PGRF_DIR_ATTR_GET_REQ_PKT__

#include "Base/PPktDef.h"

typedef std::vector<String> AttrKeyMap;

class PGRFDirAttrGetReqPkt : public PPacketBase
{
public:
	PGRFDirAttrGetReqPkt(U8 subtype = PMyPktDef::GRF_DIR_ATTR_GET_REQ);
	PGRFDirAttrGetReqPkt(U32 e, U32 sessionID, String dirname, U8 subtype = PMyPktDef::GRF_DIR_ATTR_GET_REQ);


	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFDirAttrGetReqPkt(void);

public:
	U32 m_event;
	U32 m_iSessionID;
	String m_sDirName;
};

#endif//__PGRF_DIR_ATTR_GET_RES_PKT__