#include "Base/GHeader.h"
#include "PGRFKeepAliveReqPkt.h"

PGRFKeepAliveReqPkt::PGRFKeepAliveReqPkt(U32 sessionID, const Network::PInetAddr& addr, U8 subtype)
	:PPacketBase( PMyPktDef::MY_MAJOR_PKT, subtype )
{
	m_iSessionID = sessionID;
	m_oAddr = addr;
	this->setSocketType(PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
}

PGRFKeepAliveReqPkt::PGRFKeepAliveReqPkt(U8 subtype):PPacketBase( PMyPktDef::MY_MAJOR_PKT,subtype )
{

}

U32		PGRFKeepAliveReqPkt::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize(buf, offset);
	S8 * buff = buf + offset;

	writeU32(buff + cur_pos, m_iSessionID);
	cur_pos += sizeof(U32);

	cur_pos += m_oAddr.serialize(buff + cur_pos,0);

	return cur_pos;

}

U32		PGRFKeepAliveReqPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);
	const S8 * buff = buf + offset;

	readU32(buff + cur_pos, m_iSessionID);
	cur_pos += sizeof(U32);

	cur_pos += m_oAddr.deserialize(buff + cur_pos,0);

	return cur_pos;
}

U32 PGRFKeepAliveReqPkt::getLength() const 
{
	U32 iLen = COMMON_HEAD_LENGTH;

	iLen += sizeof(Network::PInetAddr) + sizeof(U32);

	return iLen;

}

String	PGRFKeepAliveReqPkt::toString() const 
{
	std::ostringstream strstr;
	strstr<<" type:"<<PMyPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	strstr << "session: "<< m_iSessionID;
	strstr << " PInetAddr :"<< m_oAddr.toString();

	return strstr.str();

}

U32	PGRFKeepAliveReqPkt::toString(S8 * str)const
{
	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;
}

PGRFKeepAliveReqPkt::~PGRFKeepAliveReqPkt(void)
{

}


