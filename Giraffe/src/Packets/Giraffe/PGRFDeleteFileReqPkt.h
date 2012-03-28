#ifndef __PGRF_DELETE_REQ_PKT__
#define __PGRF_DELETE_REQ_PKT__

#include "Base/PPktDef.h"

class PGRFDeleteFileReqPkt : public PPacketBase
{
public:
	PGRFDeleteFileReqPkt(U8 subtype = PMyPktDef::GRF_DELETE_FILE_REQ);
	PGRFDeleteFileReqPkt(U32 e, U32 sessionID, U32 handle, U8 subtype = PMyPktDef::GRF_DELETE_FILE_REQ);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFDeleteFileReqPkt(void);

public:
	U32 m_iEvent;
	U32 m_iSessionID;
	U32 m_iHandle;
};

#endif//__PGRF_UNLINK_REQ_PKT__
