#include "Base/GHeader.h"
#include "PGRFDirAttrGetResPkt.h"

PGRFDirAttrGetResPkt::PGRFDirAttrGetResPkt(U32 e, String dirname, U32 attrCount, AttrValMap attrs, U8 subtype)
	:PPacketBase( PMyPktDef::MY_MAJOR_PKT, subtype )
{

	m_event = e;
	m_sDirName = dirname;
	m_iAttrCount = attrCount;
	m_dirAttrs = attrs;
	
	this->setSocketType(PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
}

PGRFDirAttrGetResPkt::PGRFDirAttrGetResPkt(U8 subtype):PPacketBase( PMyPktDef::MY_MAJOR_PKT,subtype )
{

}

U32		PGRFDirAttrGetResPkt::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize(buf, offset);
	S8 * buff = buf + offset;

	writeU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	cur_pos += writeString(buff + cur_pos, m_sDirName);

	writeU32(buff + cur_pos, m_iAttrCount);
	cur_pos += sizeof(U32);

	AttrValMap::const_iterator cit = m_dirAttrs.begin();

	for(unsigned int i = 0; i < m_iAttrCount; i ++, cit++)
	{
		cur_pos += writeString(buff + cur_pos, cit->first);
		cur_pos += writeString(buff + cur_pos, cit->second);
	}

	//cur_pos += m_oAddr.serialize(buff + cur_pos,0);

	return cur_pos;
}

U32		PGRFDirAttrGetResPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);
	const S8 * buff = buf + offset;

	readU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	cur_pos += readString(buff + cur_pos, m_sDirName);

	readU32(buff + cur_pos, m_iAttrCount);
	cur_pos += sizeof(U32);

	String attrkey, attrval;
	for(unsigned int i = 0; i < m_iAttrCount; i ++)
	{
		cur_pos += readString(buff + cur_pos, attrkey);
		cur_pos += readString(buff + cur_pos, attrval);
		m_dirAttrs[attrkey] =  attrval;
	}

	return cur_pos;
}

U32 PGRFDirAttrGetResPkt::getLength() const 
{
	U32 iLen = COMMON_HEAD_LENGTH;

	iLen += sizeof(U32) * 2;
	iLen += m_sDirName.size() + 5;

	AttrValMap::const_iterator cit = m_dirAttrs.begin();
	for (unsigned int i = 0; i < m_iAttrCount; i ++, cit ++)
	{
		iLen += (cit->first).size() + 5;
		iLen += (cit->second).size() + 5;
	}


	return iLen;

}

String	PGRFDirAttrGetResPkt::toString() const 
{
	std::ostringstream strstr;
	strstr<<" type:"<<PMyPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	strstr << "event: " << m_event;
	strstr << "dirname:" << m_sDirName;
	strstr << "attr get number: " <<m_iAttrCount;
	AttrValMap::const_iterator cit = m_dirAttrs.begin();

	for(; cit != m_dirAttrs.end(); cit ++)
	{
		strstr << "attr name: " << cit->first << "attr value: "<< cit->second;
	}
	//strstr << " PInetAddr :"<< m_oAddr.toString();

	return strstr.str();

}

U32	PGRFDirAttrGetResPkt::toString(S8 * str)const
{
	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;
}

PGRFDirAttrGetResPkt::~PGRFDirAttrGetResPkt(void)
{

}
