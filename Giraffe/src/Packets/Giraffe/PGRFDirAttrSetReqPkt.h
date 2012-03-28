#ifndef __PGRF_DIR_ATTR_SET_REQ_PKT__
#define __PGRF_DIR_ATTR_SET_REQ_PKT__

#include "Base/PPktDef.h"

class PGRFDirAttrSetReqPkt : public PPacketBase
{
public:
	PGRFDirAttrSetReqPkt(U8 subtype = PMyPktDef::GRF_DIR_ATTR_SET_REQ);
	PGRFDirAttrSetReqPkt(U32 e, U32 sessionID, String dirname, String key, String val, U8 subtype = PMyPktDef::GRF_DIR_ATTR_SET_REQ);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFDirAttrSetReqPkt(void);

	//GRF_ATTR_SET_REQ|EVENT|DIR_NAME|ATTR_NAME|ATTR_VAL

public:
	U32 m_event;
	U32 m_iSessionID;
	String m_sDirName;
	String m_sAttrName;
	String m_sAttrVal;
};

#endif//__PGRF_ATTR_SET_REQ_PKT__

