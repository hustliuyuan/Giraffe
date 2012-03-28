// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

//张亚霏2008-09-09加



/*
测试代码



#include "PAddressable.h"
#include "PNATRelayPkt.h"
#include "PNATHeartBeatPkt.h"

void test()
{

Network::PAddressable addr;
addr.setGIP(48759485);
addr.setRIP(48759485);
addr.setGPort(20001);
addr.setRPort(20001);
addr.setNetStatus(0);


PNATHeartBeatPkt hb_pkt;


PNATRelayPkt relay_pkt(addr, hb_pkt);
PNATRelayPkt relay_pkt2;

char buf[4096];
U32 len1 = relay_pkt.serialize(buf, 0);
U32 len2 = relay_pkt2.deserialize(buf, 0);


boost::asio::io_service ios;
boost::asio::ip::udp::endpoint ep_local
(boost::asio::ip::address_v4::loopback(), 30000);
boost::asio::ip::udp::endpoint ep_remote
(boost::asio::ip::address_v4::loopback(), 40098);
boost::asio::ip::udp::socket s(ios, ep_local);
s.send_to(boost::asio::buffer(buf, len1), ep_remote);
}




*/


#include "common/Header.h"
#include "packet/nats/PNATRelayPkt.h"
#include <sstream>

PNATRelayPkt::PNATRelayPkt()
:PPacketBase(PPktDef::PEER_TO_NAT_SERVER,PPktDef::NAT_RELAY)
{

}

PNATRelayPkt::PNATRelayPkt(const Network::PAddressable& SrcAddr,
						   const Network::PAddressable& DestAddr,
						   const PPacketBase& pkt
						   )
:PPacketBase(PPktDef::PEER_TO_NAT_SERVER,PPktDef::NAT_RELAY),
m_oDestAddress(DestAddr),
m_oSrcAddress (SrcAddr)
{
	m_oData.resize(pkt.getLength());
	pkt.serialize(&m_oData[0], 0);
}





U32 PNATRelayPkt::getLength() const
{
	return COMMON_HEAD_LENGTH + m_oSrcAddress.getLength()
		+m_oDestAddress.getLength() +
		sizeof(U32) + (U32)m_oData.size();
}




U32 PNATRelayPkt::serialize( S8 *  buf,  S32 offset ) const
{
	U32 size = PPacketBase::serialize(buf, offset);
	S8 * buffer = buf + offset + size;
	S8 * buffer2 = buffer;

	buffer += m_oSrcAddress.serialize(buffer, 0);
	buffer += m_oDestAddress.serialize(buffer, 0);


	U32 datalen = (U32)m_oData.size();
	writeU32(buffer, datalen);
	buffer += sizeof(U32);

	writeBytes(buffer, &m_oData[0], datalen);
	buffer += datalen;
	return (U32)(buffer - buffer2) + size;
}


U32 PNATRelayPkt::deserialize( const S8 * buf, S32 offset )
{
	U32 size = PPacketBase::deserialize(buf, offset);
	const S8 * buffer = buf + offset + size;
	const S8 * buffer2 = buffer;

	buffer += m_oSrcAddress.deserialize(buffer, 0);
	buffer += m_oDestAddress.deserialize(buffer, 0);

	U32 datalen = (U32)m_oData.size();
	readU32(buffer, datalen);
	buffer += sizeof(U32);

	m_oData.resize(datalen);
	readBytes(buffer, &m_oData[0], datalen);
	buffer += datalen;
	return (U32)(buffer - buffer2) + size;
}

String  PNATRelayPkt::toString() const
{
	std::ostringstream s;
	s<< PPacketBase::toString();
	s<< " from:"<< m_oSrcAddress.toString();
	s<< " to:"<< m_oDestAddress.toString();
	s<< " payloadsize:"<< m_oData.size();
	return s.str();
}

U32 PNATRelayPkt::toString( S8 * str ) const
{
	std::ostringstream strstr; 
	strstr<<" type:"<<PPktDef::type2String(this->m_cType,this->m_cSubType)	
		<<" v:"<<this->m_sVersion;

	strstr<< "Destination Address:"<< m_oDestAddress.toString();
	strstr<< "Source Address:" <<m_oSrcAddress.toString();


	String str1 = strstr.str();	
	U32 len = U32( str1.length() + 1);
	memcpy(str, str1.c_str(), len );
	return len;
}
U32	PNATRelayPkt::getRelayedPktSize()const
{
	return (U32)m_oData.size();
}

U32	PNATRelayPkt::getRelayedPktPos()const
{
	return COMMON_HEAD_LENGTH + m_oSrcAddress.getLength()
		+ m_oDestAddress.getLength() +
		sizeof(U32);
}

const Network::PAddressable	& PNATRelayPkt::getDstAddressable() const
{
	return m_oDestAddress;
}
const Network::PAddressable	& PNATRelayPkt::getSrcAddressable() const
{
	return m_oSrcAddress;
}



