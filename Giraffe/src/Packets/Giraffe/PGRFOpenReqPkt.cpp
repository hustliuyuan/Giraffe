#include "Base/GHeader.h"
#include "PGRFOpenReqPkt.h"

PGRFOpenReqPkt::PGRFOpenReqPkt(U32 e, U32 sessionID, U32 flags, String name, U32 attrsNum, AttrMap attrs,U8 subtype)
	:PPacketBase( PMyPktDef::MY_MAJOR_PKT, subtype )
{

	m_event = e;
	m_iSessionID = sessionID;
	m_iFlags = flags;
	m_sName = name;
	m_iAttrNum = attrsNum;
	m_attrs = attrs;

	this->setSocketType(PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
}

PGRFOpenReqPkt::PGRFOpenReqPkt(U8 subtype):PPacketBase( PMyPktDef::MY_MAJOR_PKT,subtype )
{

}

U32		PGRFOpenReqPkt::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize(buf, offset);
	S8 * buff = buf + offset;

	writeU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iSessionID);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iFlags);
	cur_pos += sizeof(U32);

	cur_pos += writeString(buff + cur_pos, m_sName);
	
	writeU32(buff + cur_pos,m_iAttrNum);
	cur_pos += sizeof(U32);

	if(m_iAttrNum && (m_iFlags&OPEN_FLAG_CREATE))
	{
		AttrMap::const_iterator it = m_attrs.begin();;
		for( unsigned int i = 0; i < m_iAttrNum; i ++)
		{
			cur_pos += writeString(buff + cur_pos, it->first);
			cur_pos += writeString(buff + cur_pos, it->second);
			it ++;
		}
	}

	return cur_pos;

}

U32		PGRFOpenReqPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);
	const S8 * buff = buf + offset;

	readU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iSessionID);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iFlags);
	cur_pos += sizeof(U32);

	cur_pos += readString(buff + cur_pos, m_sName);
	
	readU32(buff + cur_pos,m_iAttrNum);
	cur_pos += sizeof(U32);

	if(m_iAttrNum && (m_iFlags&OPEN_FLAG_CREATE))
	{
		string key, val;
		for(unsigned int i = 0; i < m_iAttrNum; i ++)
		{
			cur_pos += readString(buff + cur_pos, key);
			cur_pos += readString(buff + cur_pos, val);
			m_attrs[key] = val;
		}
	}

	return cur_pos;
}

U32 PGRFOpenReqPkt::getLength() const 
{
	U32 iLen = COMMON_HEAD_LENGTH;

	iLen += sizeof(U32) * 4;

	iLen += m_sName.size() + 5;

	if(m_iAttrNum && (m_iFlags&OPEN_FLAG_CREATE))
	{
		AttrMap::const_iterator it = m_attrs.begin();;
		for( unsigned int i = 0; i < m_iAttrNum; i ++)
		{
			iLen += (it->first).size() + 5;
			iLen += (it->second).size() + 5;
			it ++;
		}
	}

	return iLen;

}

String	PGRFOpenReqPkt::toString() const 
{
	std::ostringstream strstr;
	strstr<<" type:"<<PMyPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	strstr << "event: " <<m_event;
	strstr << "session id:" << m_iSessionID;
	strstr << "flags:" << m_iFlags;
	strstr << "name:" << m_sName;
	strstr << "attribute number: " <<m_iAttrNum;

	if(m_iAttrNum && (m_iFlags&OPEN_FLAG_CREATE))
	{
		AttrMap::const_iterator it = m_attrs.begin();;
		for( unsigned int i = 0; i < m_iAttrNum; i ++)
			strstr << "attr[" << it->first << "] = " << it->second;
	}

//	strstr << " PInetAddr :"<< m_oAddr.toString();

	return strstr.str();

}

U32	PGRFOpenReqPkt::toString(S8 * str)const
{
	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;
}

PGRFOpenReqPkt::~PGRFOpenReqPkt(void)
{

}


