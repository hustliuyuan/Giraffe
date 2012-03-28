#include "Base/GHeader.h"
#include "PGRFLockResPkt.h"

PGRFLockResPkt::PGRFLockResPkt(U32 e, U32 handle, U32 lckStat, U32 lckGen, U8 subtype)
	:PPacketBase( PMyPktDef::MY_MAJOR_PKT, subtype )
{
	m_event = e;
	m_iHandle = handle;
	m_iLockStatus = lckStat;
	m_iLockGeneration = lckGen;

	this->setSocketType(PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
}

PGRFLockResPkt::PGRFLockResPkt(U8 subtype):PPacketBase( PMyPktDef::MY_MAJOR_PKT,subtype )
{

}

U32		PGRFLockResPkt::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize(buf, offset);
	S8 * buff = buf + offset;

	writeU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iHandle);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos, m_iLockStatus);
	cur_pos += sizeof(U32);

	writeU32(buff + cur_pos,m_iLockGeneration);
	cur_pos += sizeof(U32);
//	cur_pos += m_oAddr.serialize(buff + cur_pos,0);

	return cur_pos;

}

U32		PGRFLockResPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);
	const S8 * buff = buf + offset;

	readU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iHandle);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iLockStatus);
	cur_pos += sizeof(U32);

	readU32(buff + cur_pos, m_iLockGeneration);
	cur_pos += sizeof(U32);
//	cur_pos += m_oAddr.deserialize(buff + cur_pos,0);

	return cur_pos;
}

U32 PGRFLockResPkt::getLength() const 
{
	U32 iLen = COMMON_HEAD_LENGTH;

	iLen += sizeof(U32) * 4;

	return iLen;

}

String	PGRFLockResPkt::toString() const 
{
	std::ostringstream strstr;
	strstr<<" type:"<<PMyPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	strstr << "event: " << m_event;
	strstr << "session id: " << m_iHandle;
	strstr << "lock status:" << m_iLockStatus;
	strstr << "lock generation: " << m_iLockGeneration;
//	strstr << " PInetAddr :"<< m_oAddr.toString();

	return strstr.str();

}

U32	PGRFLockResPkt::toString(S8 * str)const
{
	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;
}

PGRFLockResPkt::~PGRFLockResPkt(void)
{

}


