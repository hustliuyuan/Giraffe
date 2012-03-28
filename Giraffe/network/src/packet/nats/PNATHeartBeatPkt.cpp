// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#include "common/Header.h"
#include "packet/nats/PNATHeartBeatPkt.h"
#include <sstream>

PNATHeartBeatPkt::PNATHeartBeatPkt()
:PPacketBase(PPktDef::PEER_TO_NAT_SERVER,PPktDef::NAT_HEARTBEAT)
{

}
//
//U32 PNATHeartBeatPkt::getLength( ) const
//{
//	return COMMON_HEAD_LENGTH ;
//}
//
//
//U32 PNATHeartBeatPkt::serialize( S8 *  buf,  S32 offset ) const
//{
//	return PPacketBase::serialize(buf, offset);
//}
//
//
//U32 PNATHeartBeatPkt::deserialize( const S8 * buf, S32 offset )
//{
//	return PPacketBase::deserialize(buf, offset);
//}

String  PNATHeartBeatPkt::toString() const
{
	std::ostringstream s;
	s<< PPacketBase::toString();
	return s.str();
}

U32 PNATHeartBeatPkt::toString( S8 * str ) const
{
	std::ostringstream strstr; 
	strstr<<" type:"<<PPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	String str1 = strstr.str();	
	U32 len = U32( str1.length() + 1);
	memcpy(str, str1.c_str(), len );
	return len;
}
