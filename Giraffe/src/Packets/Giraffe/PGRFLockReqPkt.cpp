#include "Base/GHeader.h"
#include "PGRFLockReqPkt.h"

PGRFLockReqPkt::PGRFLockReqPkt(U32 e, U32 sessionID, U32 handle, U32 lckMode, B8 tryLck, U8 subtype)
	:PPacketBase( PMyPktDef::MY_MAJOR_PKT, subtype )
{
	m_event = e;
	m_iSessionID = sessionID;
	m_iHandle = handle;
	m_iLockMode = lckMode;
	m_bTryLock = tryLck;
//	m_oAddr = addr;
	this->setSocketType(PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
}

PGRFLockReqPkt::PGRFLockReqPkt(U8 subtype):PPacketBase( PMyPktDef::MY_MAJOR_PKT,subtype )
{

}

U32		PGRFLockReqPkt::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize(buf, offset);
	S8 * buff = buf + offset;

	writeU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iSessionID);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iHandle);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iLockMode);
	cur_pos += sizeof(U32);

	writeB8(buff + cur_pos, m_bTryLock);
	cur_pos += sizeof(B8);

	return cur_pos;

}

U32		PGRFLockReqPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);
	const S8 * buff = buf + offset;

	readU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iSessionID);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iHandle);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iLockMode);
	cur_pos += sizeof(U32);

	m_bTryLock = readB8(buff + cur_pos);
	cur_pos += sizeof(B8);
//	cur_pos += m_oAddr.deserialize(buff + cur_pos,0);

	return cur_pos;
}

U32 PGRFLockReqPkt::getLength() const 
{
	U32 iLen = COMMON_HEAD_LENGTH;

	iLen += sizeof(U32) * 4;
	iLen += sizeof(U8);

	return iLen;

}

String	PGRFLockReqPkt::toString() const 
{
	std::ostringstream strstr;
	strstr<<" type:"<<PMyPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	strstr << "event: " << m_event;
	strstr << "session id:" << m_iSessionID;
	strstr << "handle: " << m_iHandle;
	strstr << "lock mode: " << m_iLockMode;
	strstr << "try lock: " << (m_bTryLock? "yes" : "no") ;

//	strstr << " PInetAddr :"<< m_oAddr.toString();

	return strstr.str();

}

U32	PGRFLockReqPkt::toString(S8 * str)const
{
	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;
}

PGRFLockReqPkt::~PGRFLockReqPkt(void)
{

}


