#include "Base/GHeader.h"
#include "PGRFPaxosEventPkt.h"

PGRFPaxosEventPkt::PGRFPaxosEventPkt(U8 subtype):PPacketBase( PMyPktDef::MY_MAJOR_PKT,subtype )
{

}

PGRFPaxosEventPkt::PGRFPaxosEventPkt(U32 paxosType, U32 myid, U32 epoch, U64 txid, PInetAddr addr, U8 subtype)
	:PPacketBase( PMyPktDef::MY_MAJOR_PKT, subtype )
{
	m_iPaxosType = paxosType;
	m_iID = myid;
	m_iEpoch = epoch;
	m_lTxid = txid;
	m_oAddr = addr;
	this->setSocketType(PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
}



U32		PGRFPaxosEventPkt::serialize( S8 *  buf,  S32 offset ) const 
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

	m_oAddr.serialize(buff + cur_pos,0);
	cur_pos += m_oAddr.getLength();

	return cur_pos;

}

U32		PGRFPaxosEventPkt::deserialize( const S8 * buf, S32 offset )
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

	cur_pos += m_oAddr.deserialize(buff + cur_pos,0);

	return cur_pos;
}

U32 PGRFPaxosEventPkt::getLength() const 
{
	U32 iLen = COMMON_HEAD_LENGTH;

	iLen += sizeof(U32) * 3 + sizeof(U64) * 2;
	return iLen;

}

String	PGRFPaxosEventPkt::toString() const 
{
	std::ostringstream strstr;
	strstr<<" type:"<<PMyPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	//strstr << "paxos type: " << m_iPaxosType;
	//strstr << "paxos id: " << m_iID;
	//strstr << "paxos epoch:" << m_iepoch;
	//strstr << "paxos log id: " << m_lZxid ;
	//strstr << "paxos address: " << m_lAddr ;
	//strstr << " PInetAddr :"<< m_oAddr.toString();

	return strstr.str();

}

U32	PGRFPaxosEventPkt::toString(S8 * str)const
{
	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;
}

PGRFPaxosEventPkt::~PGRFPaxosEventPkt(void)
{

}


