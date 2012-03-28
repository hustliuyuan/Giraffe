#ifndef __PGRF_OPEN_REQ_PKT__
#define __PGRF_OPEN_REQ_PKT__

#include "Base/PPktDef.h"

using namespace std;

enum{
	/** Open file for reading */
    OPEN_FLAG_READ           = 0x00001,
    /** Open file for writing */
    OPEN_FLAG_WRITE          = 0x00002,
    /** Open file for locking */
    OPEN_FLAG_LOCK           = 0x00004,
    /** Create file if it does not exist */
    OPEN_FLAG_CREATE         = 0x00008
};

typedef std::map<String, String> AttrMap;

class PGRFOpenReqPkt : public PPacketBase
{
public:
	PGRFOpenReqPkt(U8 subtype = PMyPktDef::GRF_OPEN_REQ);
	PGRFOpenReqPkt(U32 e, U32 sessionid, U32 flags, String name, U32 attrsNum, AttrMap attrs,U8 subtype = PMyPktDef::GRF_OPEN_REQ);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFOpenReqPkt(void);

	//GRF_OPEN_REQ|EVENT|FLAGS|NAME|ATTR_COUNT|ATTRNAME|ATTRVAL|...

public:
	U32		m_event;
	U32		m_iSessionID;
	U32		m_iFlags;
	String		m_sName;
	U32		m_iAttrNum;
	AttrMap m_attrs;
};

#endif//__PGRF_OPEN_REQ_PKT__
