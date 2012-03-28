#include "Base/GHeader.h"
#include "PGRFDeleteFileReqPkt.h"

PGRFDeleteFileReqPkt::PGRFDeleteFileReqPkt(U32 e, U32 sessionID, U32 handle, U8 subtype)
	:PPacketBase( PMyPktDef::MY_MAJOR_PKT, subtype )
{

	m_iEvent = e;
	m_iSessionID = sessionID;
	m_iHandle = handle;
	this->setSocketType(PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
}

PGRFDeleteFileReqPkt::PGRFDeleteFileReqPkt(U8 subtype):PPacketBase( PMyPktDef::MY_MAJOR_PKT,subtype )
{

}

U32		PGRFDeleteFileReqPkt::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize(buf, offset);
	S8 * buff = buf + offset;

	writeU32(buff + cur_pos, m_iEvent);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iSessionID);
	cur_pos += sizeof(U32);

	writeU32(buff, m_iHandle);
	cur_pos += sizeof(U32);
	//cur_pos += m_oAddr.serialize(buff + cur_pos,0);

	return cur_pos;

}

U32		PGRFDeleteFileReqPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);
	const S8 * buff = buf + offset;

	readU32(buff + cur_pos, m_iEvent);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iSessionID);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iHandle);
	cur_pos += sizeof(U32);

	//cur_pos += m_oAddr.deserialize(buff + cur_pos,0);

	return cur_pos;
}

U32 PGRFDeleteFileReqPkt::getLength() const 
{
	U32 iLen = COMMON_HEAD_LENGTH;


	iLen += sizeof(U32) * 3;

	return iLen;

}

String	PGRFDeleteFileReqPkt::toString() const 
{
	std::ostringstream strstr;
	strstr<<" type:"<<PMyPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;
	strstr << "event:" << m_iEvent;
	strstr << "session id: " << m_iSessionID; 
	strstr << "handle: " << m_iHandle;
	//strstr << " PInetAddr :"<< m_oAddr.toString();

	return strstr.str();

}

U32	PGRFDeleteFileReqPkt::toString(S8 * str)const
{
	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;
}

PGRFDeleteFileReqPkt::~PGRFDeleteFileReqPkt(void)
{

}
