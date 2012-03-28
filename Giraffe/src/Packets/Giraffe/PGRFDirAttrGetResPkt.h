#ifndef __PGRF_DIR_ATTR_GET_RES_PKT__
#define __PGRF_DIR_ATTR_GET_RES_PKT__

#include "Base/PPktDef.h"

typedef std::map<String, String> AttrValMap;

class PGRFDirAttrGetResPkt : public PPacketBase
{
public:
	PGRFDirAttrGetResPkt(U8 subtype = PMyPktDef::GRF_DIR_ATTR_GET_RES);
	PGRFDirAttrGetResPkt(U32 e, String dirname, U32 attrCount, AttrValMap atts, U8 subtype = PMyPktDef::GRF_DIR_ATTR_GET_RES);

	virtual String					toString() const ;

	virtual U32					    toString(S8 * str)const;

	virtual U32						getLength() const;

	virtual U32						serialize( S8 *  buf,  S32 offset ) const ;

	virtual U32						deserialize( const S8 * buf, S32 offset );

public:

	virtual ~PGRFDirAttrGetResPkt(void);

	//GRF_ATTR_SET_REQ|EVENT|HANDLE|ATTR_NAME|ATTR_VAL

public:
	U32 m_event;
	String m_sDirName;
	U32 m_iAttrCount;
	AttrValMap m_dirAttrs;
};

#endif//__PGRF_DIR_ATTR_GET_RES_PKT__


