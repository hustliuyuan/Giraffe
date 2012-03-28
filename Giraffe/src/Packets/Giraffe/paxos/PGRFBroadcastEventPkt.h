#ifndef __PGRF_BROADCAST_EVENT_PKT_H__
#define __PGRF_BROADCAST_EVENT_PKT_H__

#include "Base/PPktDef.h"

class PGRFBroadcastEventPkt : public PPacketBase
{
public:
	PGRFBroadcastEventPkt(U8 subtype = PMyPktDef::GRF_BROADCAST_EVENT);
	PGRFBroadcastEventPkt(U32 paxosType, U32 myid, U32 epoch, U64 txid, string svalue,U8 subtype = PMyPktDef::GRF_BROADCAST_EVENT);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFBroadcastEventPkt(void);

public:
	U32 m_iPaxosType;
	U32 m_iID;
	U32 m_iEpoch;
	U64 m_lTxid;
	string m_sValue;
};

#endif
