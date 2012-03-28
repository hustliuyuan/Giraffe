#include "Base/GHeader.h"
#include "PGRFAttrSetReqPkt.h"

PGRFAttrSetReqPkt::PGRFAttrSetReqPkt(U32 e, U32 sessionID, U32 handle, String attrName, String attrVal, U8 subtype)
	:PPacketBase( PMyPktDef::MY_MAJOR_PKT, subtype )
{

	m_event = e;
	m_iSessionID = sessionID;
	m_iHandle = handle;
	m_sAttrName = attrName;
	m_sAttrVal = attrVal;
	this->setSocketType(PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
}

PGRFAttrSetReqPkt::PGRFAttrSetReqPkt(U8 subtype):PPacketBase( PMyPktDef::MY_MAJOR_PKT,subtype )
{

}

U32		PGRFAttrSetReqPkt::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize(buf, offset);
	S8 * buff = buf + offset;

	writeU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iSessionID);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iHandle);
	cur_pos += sizeof(U32);

	cur_pos += writeString(buff + cur_pos, m_sAttrName);
	cur_pos += writeString(buff + cur_pos, m_sAttrVal);
	//cur_pos += m_oAddr.serialize(buff + cur_pos,0);

	return cur_pos;

}

U32		PGRFAttrSetReqPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);
	const S8 * buff = buf + offset;

	readU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iSessionID);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iHandle);
	cur_pos += sizeof(U32);

	cur_pos += readString(buff + cur_pos, m_sAttrName);
	cur_pos += readString(buff + cur_pos, m_sAttrVal);
	//cur_pos += m_oAddr.deserialize(buff + cur_pos,0);

	return cur_pos;
}

U32 PGRFAttrSetReqPkt::getLength() const 
{
	U32 iLen = COMMON_HEAD_LENGTH;

	iLen += sizeof(U32) * 3;
	iLen += m_sAttrName.size() + 5;
	iLen += m_sAttrVal.size() + 5;

	return iLen;

}

String	PGRFAttrSetReqPkt::toString() const 
{
	std::ostringstream strstr;
	strstr<<" type:"<<PMyPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	strstr << "event: " << m_event;
	strstr << "session id" << m_iSessionID;
	strstr << "handle:" << m_iHandle;
	strstr << "attr name: " << m_sAttrName;
	strstr << "attr value: " << m_sAttrName;
	//strstr << " PInetAddr :"<< m_oAddr.toString();

	return strstr.str();

}

U32	PGRFAttrSetReqPkt::toString(S8 * str)const
{
	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;
}

PGRFAttrSetReqPkt::~PGRFAttrSetReqPkt(void)
{

}


