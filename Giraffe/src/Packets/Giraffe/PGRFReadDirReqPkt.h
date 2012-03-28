#ifndef __PGRF_READ_DIR_REQ_PKT__
#define __PGRF_READ_DIR_REQ_PKT__

#include "Base/PPktDef.h"

class PGRFReadDirReqPkt : public PPacketBase
{
public:
	PGRFReadDirReqPkt(U8 subtype = PMyPktDef::GRF_READ_DIR_REQ);
	PGRFReadDirReqPkt(U32 e, U32 sessionID, String dirName,U8 subtype = PMyPktDef::GRF_READ_DIR_REQ);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFReadDirReqPkt(void);
	//GRF_READ_DIR_REQ|EVENT|SESSIONID|NAME
public:
	U32 m_event;
	U32 m_iSessionID;
	String m_sDirName;
};

#endif//__PGRF_READ_DIR_REQ_PKT__
