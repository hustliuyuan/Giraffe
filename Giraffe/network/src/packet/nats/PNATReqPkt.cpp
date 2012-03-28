// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#include "common/Header.h"
#include "packet/nats/PNATReqPkt.h"
#include <sstream>

PNATReqPkt::PNATReqPkt()
:PPacketBase(PPktDef::PEER_TO_NAT_SERVER,PPktDef::NAT_REQ)
{

}

PNATReqPkt::PNATReqPkt( const PInetAddr & selfaddr, const PInetAddr & destaddr )
:PPacketBase(PPktDef::PEER_TO_NAT_SERVER,PPktDef::NAT_REQ)
{
	this->m_oSelfAddr = selfaddr;
	this->m_oDestAddr = destaddr;
}

U32 PNATReqPkt::getLength( ) const
{
	return COMMON_HEAD_LENGTH + m_oSelfAddr.getLength()
		+m_oDestAddr.getLength();
}



U32 PNATReqPkt::serialize( S8 *  buf,  S32 offset ) const
{
	U32 cur_pos = PPacketBase::serialize( buf, offset );

	S8 * buff = buf + offset;

	this->m_oSelfAddr.serialize(buff + cur_pos , 0 );

	cur_pos+= m_oSelfAddr.getLength();

	this->m_oDestAddr.serialize(buff + cur_pos , 0 );

	cur_pos+= m_oDestAddr.getLength();

	return cur_pos;
}


U32 PNATReqPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 cur_pos = PPacketBase::deserialize(buf, offset);

	const S8 * buff = buf+ offset;

	m_oSelfAddr.deserialize( buff +  cur_pos,0 );

	cur_pos += m_oSelfAddr.getLength();

	m_oDestAddr.deserialize( buff +  cur_pos,0 );

	cur_pos += m_oDestAddr.getLength();

	return cur_pos;
}

String  PNATReqPkt::toString() const
{
	std::ostringstream s;
	s<< PPacketBase::toString();
	s<< " selfaddr: "<< m_oSelfAddr.toString();
	s<< " destaddr: "<< m_oDestAddr.toString();
	return s.str();

}

U32 PNATReqPkt::toString( S8 * str ) const
{
	std::ostringstream strstr; 
	strstr<<" type:"<<PPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	strstr<< "Self Address:"<< m_oSelfAddr.toString();
	strstr<< "Dest Address:" <<m_oDestAddr.toString();


	String str1 = strstr.str();	
	U32 len = U32( str1.length() + 1);
	memcpy(str, str1.c_str(), len );
	return len;
}
const Network::PInetAddr	&	PNATReqPkt::getSelfAddr() const
{
	return this->m_oSelfAddr;
}
const Network::PInetAddr	&	PNATReqPkt::getDestAddr() const
{
	return this->m_oDestAddr;
}

