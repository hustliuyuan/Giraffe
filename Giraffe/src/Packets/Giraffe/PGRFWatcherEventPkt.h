#ifndef __PGRF_WATCHER_EVENT_PKT__
#define __PGRF_WATCHER_EVENT_PKT__

#include "Base/PPktDef.h"

class PGRFWatcherEventPkt : public PPacketBase
{
public:
	PGRFWatcherEventPkt(U8 subtype = PMyPktDef::GRF_WATCHER_EVENT);
	PGRFWatcherEventPkt(U32 WatchererEvent, String nodeName,U8 subtype = PMyPktDef::GRF_WATCHER_EVENT);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFWatcherEventPkt(void);

public:
	U32 m_iWatchererEvent;
	String m_sNodeName;
	//TIMESTAMP
};

#endif//__PGRF_WATCHER_EVENT_PKT__
