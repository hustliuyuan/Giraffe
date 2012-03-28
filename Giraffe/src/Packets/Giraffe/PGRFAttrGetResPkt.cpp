#include "Base/GHeader.h"
#include "PGRFAttrGetResPkt.h"

PGRFAttrGetResPkt::PGRFAttrGetResPkt(U32 e, U32 handle, U32 attrCount, AttrMap attrs, U8 subtype)
	:PPacketBase( PMyPktDef::MY_MAJOR_PKT, subtype )
{
	m_event = e;
	m_iHandle = handle;
	m_iAttrCount = attrCount;
	m_attrs = attrs;

	this->setSocketType(PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
}

PGRFAttrGetResPkt::PGRFAttrGetResPkt(U8 subtype):PPacketBase( PMyPktDef::MY_MAJOR_PKT,subtype )
{
}

U32		PGRFAttrGetResPkt::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize(buf, offset);
	S8 * buff = buf + offset;

	writeU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iHandle);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iAttrCount);
	cur_pos += sizeof(U32);

	AttrMap::const_iterator cit = m_attrs.begin();

	for(unsigned int i = 0; i < m_iAttrCount; i ++, cit ++)
	{
		cur_pos += writeString(buff + cur_pos, cit->first);
		cur_pos += writeString(buff + cur_pos, cit->second);
	}
	//cur_pos += m_oAddr.serialize(buff + cur_pos,0);

	return cur_pos;
}

U32		PGRFAttrGetResPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);
	const S8 * buff = buf + offset;

	readU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iHandle);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iAttrCount);
	cur_pos += sizeof(U32);

	std::string key, val;
	for( unsigned int i = 0; i < m_iAttrCount; i ++)
	{
		cur_pos += readString(buff + cur_pos, key);
		cur_pos += readString(buff + cur_pos, val);
		m_attrs[key] = val;
	}

	//cur_pos += m_oAddr.deserialize(buff + cur_pos,0);

	return cur_pos;
}

U32 PGRFAttrGetResPkt::getLength() const 
{
	U32 iLen = COMMON_HEAD_LENGTH;

	iLen += sizeof(U32) * 3;
		
	AttrMap::const_iterator cit = m_attrs.begin();
	for(unsigned int i = 0; i < m_iAttrCount; i ++, cit ++)
	{
		iLen += (cit->first).size() +5;
		iLen += (cit->second).size() +5;
	}
	
	//iLen += sizeof(Network::PInetAddr);

	return iLen;

}

String	PGRFAttrGetResPkt::toString() const 
{
	std::ostringstream strstr;
	strstr<<" type:"<<PMyPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	strstr << "event: " << m_event;
	strstr << "handle: " <<m_iHandle;
		
	AttrMap::const_iterator cit = m_attrs.begin();
	for(unsigned int i = 0; i < m_iAttrCount; i ++, cit ++)
	{
		strstr << "attr name: " << cit->first << " attr value:" << cit->second;
	}

	return strstr.str();

}

U32	PGRFAttrGetResPkt::toString(S8 * str)const
{
	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;
}

PGRFAttrGetResPkt::~PGRFAttrGetResPkt(void)
{

}


