#include "Base/GHeader.h"
#include "PGRFPaxosLeaderKeepAliveReq.h"

PGRFPaxosLeaderKeepAliveReq::PGRFPaxosLeaderKeepAliveReq(U32 id, U32 epoch, U8 subtype)
	:PPacketBase( PMyPktDef::MY_MAJOR_PKT, subtype )
{
	m_iServerID = id;
	m_iEpoch = epoch;
	this->setSocketType(PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
}

PGRFPaxosLeaderKeepAliveReq::PGRFPaxosLeaderKeepAliveReq(U8 subtype):PPacketBase( PMyPktDef::MY_MAJOR_PKT,subtype )
{

}

U32		PGRFPaxosLeaderKeepAliveReq::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize(buf, offset);
	S8 * buff = buf + offset;

	writeU32(buff + cur_pos, m_iServerID);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iEpoch);
	cur_pos += sizeof(U32);

	return cur_pos;

}

U32		PGRFPaxosLeaderKeepAliveReq::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);
	const S8 * buff = buf + offset;

	readU32(buff + cur_pos, m_iServerID);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iEpoch);
	cur_pos += sizeof(U32);

	return cur_pos;
}

U32 PGRFPaxosLeaderKeepAliveReq::getLength() const 
{
	U32 iLen = COMMON_HEAD_LENGTH;

	iLen += sizeof(U32)*2;

	return iLen;

}

String	PGRFPaxosLeaderKeepAliveReq::toString() const 
{
	std::ostringstream strstr;
	strstr<<" type:"<<PMyPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	strstr << "id: "<< m_iServerID;
	strstr << "epoch: "<< m_iEpoch;

	return strstr.str();

}

U32	PGRFPaxosLeaderKeepAliveReq::toString(S8 * str)const
{
	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;
}

PGRFPaxosLeaderKeepAliveReq::~PGRFPaxosLeaderKeepAliveReq(void)
{

}



