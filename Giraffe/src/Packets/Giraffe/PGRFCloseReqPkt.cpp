#include "Base/GHeader.h"
#include "PGRFCloseReqPkt.h"

PGRFCloseReqPkt::PGRFCloseReqPkt(U32 e, U32 sessionID, U32 handle, U8 subtype)
	:PPacketBase( PMyPktDef::MY_MAJOR_PKT, subtype )
{
	m_event = e;
	m_iSessionID = sessionID;
	m_iHandle = handle;
	//m_oAddr = addr;
	this->setSocketType(PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
}

PGRFCloseReqPkt::PGRFCloseReqPkt(U8 subtype):PPacketBase( PMyPktDef::MY_MAJOR_PKT,subtype )
{

}

U32		PGRFCloseReqPkt::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize(buf, offset);
	S8 * buff = buf + offset;

	writeU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iSessionID);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iHandle);
	cur_pos += sizeof(U32);
	//cur_pos += m_oAddr.serialize(buff + cur_pos,0);

	return cur_pos;

}

U32		PGRFCloseReqPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);
	const S8 * buff = buf + offset;

	readU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iSessionID);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iHandle);
	cur_pos += sizeof(U32);

	//cur_pos += m_oAddr.deserialize(buff + cur_pos,0);

	return cur_pos;
}

U32 PGRFCloseReqPkt::getLength() const 
{
	U32 iLen = COMMON_HEAD_LENGTH;

	iLen += sizeof(U32) * 3;

	return iLen;

}

String	PGRFCloseReqPkt::toString() const 
{
	std::ostringstream strstr;
	strstr<<" type:"<<PMyPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	strstr << "event: " << m_event;
	strstr << "session id: " << m_iSessionID;
	strstr << "handle: " << m_iHandle;
	//strstr << " PInetAddr :"<< m_oAddr.toString();

	return strstr.str();

}

U32	PGRFCloseReqPkt::toString(S8 * str)const
{
	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;
}

PGRFCloseReqPkt::~PGRFCloseReqPkt(void)
{

}


