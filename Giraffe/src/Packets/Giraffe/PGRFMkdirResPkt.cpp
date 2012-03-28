#include "Base/GHeader.h"
#include "PGRFMkdirResPkt.h"

PGRFMkdirResPkt::PGRFMkdirResPkt(U32 e, String dirName, B8 ok, U8 subtype)
	:PPacketBase( PMyPktDef::MY_MAJOR_PKT, subtype )
{
	m_event = e;
	m_sDirName = dirName;
	m_bOK = ok;

	this->setSocketType(PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
}

PGRFMkdirResPkt::PGRFMkdirResPkt(U8 subtype):PPacketBase( PMyPktDef::MY_MAJOR_PKT,subtype )
{

}

U32		PGRFMkdirResPkt::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize(buf, offset);
	S8 * buff = buf + offset;

	writeU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	writeB8(buff + cur_pos, m_bOK);
	cur_pos += sizeof(B8);

	cur_pos += writeString(buff + cur_pos, m_sDirName);

	

//	cur_pos += m_oAddr.serialize(buff + cur_pos,0);

	return cur_pos;

}

U32		PGRFMkdirResPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);
	const S8 * buff = buf + offset;

	readU32(buff + cur_pos, m_event);
	cur_pos += sizeof(U32);

	m_bOK = readB8(buff + cur_pos);
	cur_pos += sizeof(B8);

	cur_pos += readString(buff + cur_pos, m_sDirName);
//	cur_pos += m_oAddr.deserialize(buff + cur_pos,0);

	return cur_pos;
}

U32 PGRFMkdirResPkt::getLength() const 
{
	U32 iLen = COMMON_HEAD_LENGTH;

	iLen += sizeof(U32);
	iLen += m_sDirName.size() + 5;
	iLen += sizeof(B8);

	return iLen;

}

String	PGRFMkdirResPkt::toString() const 
{
	std::ostringstream strstr;
	strstr<<" type:"<<PMyPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	strstr << "event:" << m_event;
	strstr << "dir name: " << m_sDirName;
	strstr << "mk dir result: " << m_bOK?"ok":"fail";

	return strstr.str();
}

U32	PGRFMkdirResPkt::toString(S8 * str)const
{
	String  str1 = toString();
	U32 len = str1.size() +1 ;
	memcpy(str, str1.c_str(),len  );

	return len  ;
}

PGRFMkdirResPkt::~PGRFMkdirResPkt(void)
{

}


