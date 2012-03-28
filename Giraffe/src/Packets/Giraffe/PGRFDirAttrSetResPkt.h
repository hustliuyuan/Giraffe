#ifndef __PGRF_dir_ATTR_SET_RES_PKT__
#define __PGRF_dir_ATTR_SET_RES_PKT__

#include "Base/PPktDef.h"

class PGRFDirAttrSetResPkt : public PPacketBase
{
public:
	PGRFDirAttrSetResPkt(U8 subtype = PMyPktDef::GRF_DIR_ATTR_SET_RES);
	PGRFDirAttrSetResPkt(U32 e, String dirname,B8 ok, U8 subtype = PMyPktDef::GRF_DIR_ATTR_SET_RES);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFDirAttrSetResPkt(void);

	//GRF_DIR_ATTR_SET_RES|EVENT|DIRNAME|RESULT

public:
	U32 m_event;
	String m_sDirName;
	B8 m_bOk;
};

#endif//__PGRF_ATTR_SET_RES_PKT__

