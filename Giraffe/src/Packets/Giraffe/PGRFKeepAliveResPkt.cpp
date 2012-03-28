#include "Base/GHeader.h"
#include "PGRFKeepAliveResPkt.h"

PGRFKeepAliveResPkt::PGRFKeepAliveResPkt(U32 sessionID, U8 subtype)
	:PPacketBase( PMyPktDef::MY_MAJOR_PKT, subtype )
{
	m_iSessionID = sessionID;
//	m_oAddr = addr;
	this->setSocketType(PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
}

PGRFKeepAliveResPkt::PGRFKeepAliveResPkt(U8 subtype):PPacketBase( PMyPktDef::MY_MAJOR_PKT,subtype )
{

}

U32		PGRFKeepAliveResPkt::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize(buf, offset);
	S8 * buff = buf + offset;

	writeU32(buff + cur_pos, m_iSessionID);
	cur_pos += sizeof(U32);
//	cur_pos += m_oAddr.serialize(buff + cur_pos,0);

	return cur_pos;

}

U32		PGRFKeepAliveResPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);
	const S8 * buff = buf + offset;

	readU32(buff + cur_pos, m_iSessionID);
	cur_pos += sizeof(U32);
//	cur_pos += m_oAddr.deserialize(buff + cur_pos,0);

	return cur_pos;
}

U32 PGRFKeepAliveResPkt::getLength() const 
{
	U32 iLen = COMMON_HEAD_LENGTH;

	iLen += sizeof(U32);

	return iLen;

}

String	PGRFKeepAliveResPkt::toString() const 
{
	std::ostringstream strstr;
	strstr<<" type:"<<PMyPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	strstr << "session: " << m_iSessionID;
//	strstr << " PInetAddr :"<< m_oAddr.toString();

	return strstr.str();

}

U32	PGRFKeepAliveResPkt::toString(S8 * str)const
{
	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;
}

PGRFKeepAliveResPkt::~PGRFKeepAliveResPkt(void)
{

}


