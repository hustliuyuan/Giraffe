#include "Base/GHeader.h"
#include "PGRFExitsResPkt.h"

PGRFExitsResPkt::PGRFExitsResPkt(U32 e, String name, B8 exit, U8 subtype)
	:PPacketBase( PMyPktDef::MY_MAJOR_PKT, subtype )
{
	m_event = e;
	m_sName = name;
	m_bExit = exit;
//	m_oAddr = addr;
	this->setSocketType(PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
}

PGRFExitsResPkt::PGRFExitsResPkt(U8 subtype):PPacketBase( PMyPktDef::MY_MAJOR_PKT,subtype )
{

}

U32		PGRFExitsResPkt::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize(buf, offset);
	S8 * buff = buf + offset;

	writeU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	cur_pos += writeString(buff + cur_pos, m_sName);

	writeB8(buff + cur_pos, m_bExit);
	cur_pos += sizeof(B8);
//	cur_pos += m_oAddr.serialize(buff + cur_pos,0);

	return cur_pos;

}

U32		PGRFExitsResPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);
	const S8 * buff = buf + offset;

	readU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	cur_pos += readString(buff + cur_pos, m_sName);

	m_bExit = readB8(buff + cur_pos);
	cur_pos += sizeof(B8);

//	cur_pos += m_oAddr.deserialize(buff + cur_pos,0);

	return cur_pos;
}

U32 PGRFExitsResPkt::getLength() const 
{
	U32 iLen = COMMON_HEAD_LENGTH;

	iLen += sizeof(U32);
	iLen += sizeof(B8);

	iLen += m_sName.size() + 5;

	return iLen;

}

String	PGRFExitsResPkt::toString() const 
{
	std::ostringstream strstr;
	strstr<<" type:"<<PMyPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	strstr << "event: " << m_event;
	strstr << "name: " << m_sName;
	strstr << "file exit :" << (m_bExit? "yes":"no");
//	strstr << " PInetAddr :"<< m_oAddr.toString();

	return strstr.str();

}

U32	PGRFExitsResPkt::toString(S8 * str)const
{
	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;
}

PGRFExitsResPkt::~PGRFExitsResPkt(void)
{

}


