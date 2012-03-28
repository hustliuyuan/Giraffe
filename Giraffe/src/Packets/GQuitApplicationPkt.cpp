#include "Base/GHeader.h"
#include "GQuitApplicationPkt.h"

GQuitApplicationPkt::GQuitApplicationPkt(U8 subtype):PPacketBase( PMyPktDef::MY_MAJOR_PKT, subtype )
{
	this->setSocketType(PSocketTypeFactory::DEFAULT_SOCKET_TYPE);

}

U32 GQuitApplicationPkt::getLength() const 
{
	U32 iLen = COMMON_HEAD_LENGTH;

	return iLen;
}
U32	GQuitApplicationPkt::serialize( S8 *  buf,  S32 offset ) const 
{
	U32 cur_pos = PPacketBase::serialize(buf, offset);

	return cur_pos;
}

U32	GQuitApplicationPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);

	return cur_pos;
}

String GQuitApplicationPkt::toString() const
{
	std::ostringstream strstr;
	strstr<<" type:"<<PMyPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	return strstr.str();
}

U32	GQuitApplicationPkt::toString(S8 * str)const
{
	//String  str1 = toString();
	std::ostringstream strstr;

	U32 len = strstr.str().size() +1 ;

	memcpy(str, strstr.str().c_str(),len  );

	return len  ;
}


GQuitApplicationPkt::~GQuitApplicationPkt(void)
{

}


