#ifndef __PGRF_READ_DIR_RES_PKT__
#define __PGRF_READ_DIR_RES_PKT__

#include "Base/PPktDef.h"

using namespace std;

class PGRFReadDirResPkt : public PPacketBase
{
public:
	PGRFReadDirResPkt(U8 subtype = PMyPktDef::GRF_READ_DIR_RES);
	PGRFReadDirResPkt(U32 e, U32 entryCount, vector<String>& entryNameList,U8 subtype = PMyPktDef::GRF_READ_DIR_RES);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFReadDirResPkt(void);
	//GRF_READ_DIR_RES|EVENT|ENTRY_COUNT|ENTRY_NAME|...
public:
	
	U32 m_event;
	U32 m_iEntryCount;
	vector<String> m_entryNameList;
};

#endif//__PGRF_READ_DIR_RES_PKT__
