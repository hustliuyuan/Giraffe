#ifndef __PGRF_MKDIR_REQ_PKT__
#define __PGRF_MKDIR_REQ_PKT__

#include "Base/PPktDef.h"

class PGRFMkdirReqPkt : public PPacketBase
{
public:
	PGRFMkdirReqPkt(U8 subtype = PMyPktDef::GRF_MKDIR_REQ);
	PGRFMkdirReqPkt(U32 e, U32 sessionID, String dirName,U8 subtype = PMyPktDef::GRF_MKDIR_REQ);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFMkdirReqPkt(void);
	//GRF_MKDIR_REQ|EVENT|NAME
public:
	U32 m_event;
	U32 m_iSessionID;
	String m_sDirName;
	String m_test;

};

#endif//__PGRF_MKDIR_REQ_PKT__

