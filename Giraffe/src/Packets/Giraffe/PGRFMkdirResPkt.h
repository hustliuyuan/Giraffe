#ifndef __PGRF_MKDIR_RES_PKT__
#define __PGRF_MKDIR_RES_PKT__

#include "Base/PPktDef.h"

class PGRFMkdirResPkt : public PPacketBase
{
public:
	PGRFMkdirResPkt(U8 subtype = PMyPktDef::GRF_MKDIR_RES);
	PGRFMkdirResPkt(U32 e, String dirName,B8 ok, U8 subtype = PMyPktDef::GRF_MKDIR_RES);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFMkdirResPkt(void);
	//GRF_MKDIR_REQ|EVENT|NAME
public:
	U32 m_event;
	String m_sDirName;
	B8 m_bOK;
};

#endif//__PGRF_MKDIR_RES_PKT__

