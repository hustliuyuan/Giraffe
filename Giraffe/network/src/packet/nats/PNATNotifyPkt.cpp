// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#include "common/Header.h"
#include "packet/nats/PNATNotifyPkt.h"
#include <sstream>

PNATNotifyPkt::PNATNotifyPkt()
:PPacketBase(PPktDef::PEER_TO_NAT_SERVER,PPktDef::NAT_NOTIFY)
{

}

U32 PNATNotifyPkt::getLength( ) const
{
	//return COMMON_HEAD_LENGTH + sizeof(m_oRequiredAddr);
	return COMMON_HEAD_LENGTH + m_oRequiredAddr.getLength();
}


U32 PNATNotifyPkt::serialize( S8 *  buf,  S32 offset ) const
{

	U32 cur_pos = PPacketBase::serialize( buf, offset );

	S8 * buff = buf + offset;

	this->m_oRequiredAddr.serialize(buff + cur_pos , 0 );

	cur_pos+= m_oRequiredAddr.getLength();

	return cur_pos;


}


U32 PNATNotifyPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);

	const S8 * buff = buf+ offset;

	m_oRequiredAddr.deserialize( buff +  cur_pos,0  );

	cur_pos += m_oRequiredAddr.getLength();

	return cur_pos;
}


String  PNATNotifyPkt::toString() const
{
	std::ostringstream s;
	s<< PPacketBase::toString();
	s<< " addr: "<< m_oRequiredAddr.toString();
	return s.str();

}

U32 PNATNotifyPkt::toString( S8 * str ) const
{
	std::ostringstream strstr; 
	strstr<<" type:"<<PPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	strstr<< "Required Address:"<< m_oRequiredAddr.toString();

	String str1 = strstr.str();	
	U32 len = U32( str1.length() + 1);
	memcpy(str, str1.c_str(), len );
	return len;
}
const Network::PInetAddr&  PNATNotifyPkt::getRequiredAddr() const
{
	return this->m_oRequiredAddr;
}

 const void PNATNotifyPkt::setRequireAddr(PInetAddr oRequiredAddr)
{
   this->m_oRequiredAddr = oRequiredAddr;
}

