#ifndef __PGRF_PAXOS_EVENT_PKT__
#define __PGRF_PAXOS_EVENT_PKT__

#include "Base/PPktDef.h"

class PGRFPaxosEventPkt : public PPacketBase
{
public:
	PGRFPaxosEventPkt(U8 subtype = PMyPktDef::GRF_PAXOS_EVENT);
	PGRFPaxosEventPkt(U32 paxosType, U32 myid, U32 epoch, U64 txid, PInetAddr addr,U8 subtype = PMyPktDef::GRF_PAXOS_EVENT);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFPaxosEventPkt(void);

	//GRF_ATTR_GET_REQ|EVENT|HANDLE|ATTR_NAME
public:
	U32 m_iPaxosType;
	U32 m_iID;
	U32 m_iEpoch;
	U64 m_lTxid;
	PInetAddr m_oAddr;
};

#endif // __PGRF_PAXOS_EVENT_PKT__



