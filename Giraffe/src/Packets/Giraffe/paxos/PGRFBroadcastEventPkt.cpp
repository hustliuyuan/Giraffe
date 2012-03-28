#include "Base/GHeader.h"
#include "PGRFBroadcastEventPkt.h"

PGRFBroadcastEventPkt::PGRFBroadcastEventPkt(U8 subtype):PPacketBase( PMyPktDef::MY_MAJOR_PKT,subtype )
{

}

PGRFBroadcastEventPkt::PGRFBroadcastEventPkt(U32 paxosType, U32 myid, U32 epoch, U64 txid, String svalue, U8 subtype)
	:PPacketBase( PMyPktDef::MY_MAJOR_PKT, subtype )
{
	m_iPaxosType = paxosType;
	m_iID = myid;
	m_iEpoch = epoch;
	m_lTxid = txid;
	m_sValue = svalue;
	this->setSocketType(PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
}



U32		PGRFBroadcastEventPkt::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize(buf, offset);
	S8 * buff = buf + offset;

	writeU32(buff + cur_pos, m_iPaxosType);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iID);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iEpoch);
	cur_pos += sizeof(U32);

	writeU64(buff + cur_pos, m_lTxid);
	cur_pos += sizeof(U64);

	cur_pos += writeString(buff + cur_pos, m_sValue);

	return cur_pos;

}

U32		PGRFBroadcastEventPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);
	const S8 * buff = buf + offset;

	readU32(buff + cur_pos, m_iPaxosType);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iID);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iEpoch);
	cur_pos += sizeof(U32);

	readU64(buff + cur_pos, m_lTxid);
	cur_pos += sizeof(U64);

	cur_pos += readString(buff + cur_pos, m_sValue);

	return cur_pos;
}

U32 PGRFBroadcastEventPkt::getLength() const 
{
	U32 iLen = COMMON_HEAD_LENGTH;

	iLen += sizeof(U32) * 3 + sizeof(U64) + m_sValue.size() + 5;
	return iLen;

}

String	PGRFBroadcastEventPkt::toString() const 
{
	std::ostringstream strstr;
	strstr<<" type:"<<PMyPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	strstr << "paxos type: " << m_iPaxosType;
	strstr << "paxos id: " << m_iID;
	strstr << "paxos epoch:" << m_iEpoch;
	strstr << "paxos log id: " << m_lTxid ;
	strstr << "paxos value: " << m_sValue ;

	return strstr.str();

}

U32	PGRFBroadcastEventPkt::toString(S8 * str)const
{
	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;
}

PGRFBroadcastEventPkt::~PGRFBroadcastEventPkt(void)
{

}


