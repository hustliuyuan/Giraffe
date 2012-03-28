#include "Base/GHeader.h"
#include "PGRFAttrGetReqPkt.h"

PGRFAttrGetReqPkt::PGRFAttrGetReqPkt(U32 e, U32 sessionID, U32 handle, U32 attrCount, vector<String> attrNames, U8 subtype)
	:PPacketBase( PMyPktDef::MY_MAJOR_PKT, subtype )
{

	m_event = e;
	m_iSessionID = sessionID;
	m_iHandle = handle;
	m_iAttrCount = attrCount;
	m_attrNames = attrNames;
	this->setSocketType(PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
}

PGRFAttrGetReqPkt::PGRFAttrGetReqPkt(U8 subtype):PPacketBase( PMyPktDef::MY_MAJOR_PKT,subtype )
{

}

U32		PGRFAttrGetReqPkt::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize(buf, offset);
	S8 * buff = buf + offset;

	writeU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iSessionID);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iHandle);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iAttrCount);
	cur_pos += sizeof(U32);

	//vector<String>::const_iterator cit = m_attrNames.begin();

	for(unsigned int i = 0; i < m_iAttrCount; i ++)
	{
		cur_pos += writeString(buff + cur_pos, m_attrNames[i]);
	}
	
	//cur_pos += m_oAddr.serialize(buff + cur_pos,0);

	return cur_pos;

}

U32		PGRFAttrGetReqPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);
	const S8 * buff = buf + offset;

	readU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iSessionID);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iHandle);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iAttrCount);
	cur_pos += sizeof(U32);

	for(unsigned int i = 0; i < m_iAttrCount; i ++)
	{
		String attrname;
		cur_pos += readString(buff + cur_pos, attrname);
		m_attrNames.push_back(attrname);
	}
	//cur_pos += m_oAddr.deserialize(buff + cur_pos,0);

	return cur_pos;
}

U32 PGRFAttrGetReqPkt::getLength() const 
{
	U32 iLen = COMMON_HEAD_LENGTH;

	iLen += sizeof(U32) * 4;
	String attrname;
	for(unsigned int i = 0; i < m_iAttrCount; i ++)
	{
		
		attrname = m_attrNames[i];
		iLen += attrname.size() + 5;
	}

	return iLen;

}

String	PGRFAttrGetReqPkt::toString() const 
{
	std::ostringstream strstr;
	strstr<<" type:"<<PMyPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	strstr << "event: " << m_event;
	strstr << "session id: " << m_iSessionID;
	strstr << "handle:" << m_iHandle;
	strstr << "attr names: " ;
	for(unsigned int i = 0; i < m_iAttrCount; i ++)
	{
		strstr << m_attrNames[i] << " ";
	}
	//strstr << " PInetAddr :"<< m_oAddr.toString();

	return strstr.str();

}

U32	PGRFAttrGetReqPkt::toString(S8 * str)const
{
	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;
}

PGRFAttrGetReqPkt::~PGRFAttrGetReqPkt(void)
{

}


