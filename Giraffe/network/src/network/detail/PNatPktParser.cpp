// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.



#include "common/Header.h"
#include "network/detail/PNatPktParser.h"
#include "packet/nats/NATPacket.h"

PNatPktParser::PNatPktParser()
{

}

PNatPktParser::~PNatPktParser()
{

}

const PPacketBase *	 PNatPktParser::parsePkt(const S8 * inContent, 
							U16 inContentLen, 
							U32 socket_type ) const 
{

	// TODO: make sure socket_type_id is DEFAULT_SOCKET_TYPE;	

	U8 subType	= (U8)inContent[PACKET_SUBTYPE_OFFSET];

	PPacketBase *pkt = NULL;

	switch ( subType ) 
	{
	case PPktDef::NAT_ACK:

		pkt = new PNATAckPkt( );
		pkt->deserialize(inContent, 0 );
		break;

	case PPktDef::NAT_HEARTBEAT:

		pkt = new PNATHeartBeatPkt( );
		pkt->deserialize(inContent, 0 );
		break;

	case PPktDef::NAT_NOTIFY:

		pkt = new PNATNotifyPkt( );
		pkt->deserialize(inContent, 0 );
		break;

	case PPktDef::NAT_REQ:

		pkt = new PNATReqPkt( );
		pkt->deserialize(inContent, 0 );
		break;

	case PPktDef::NAT_TEST_ACK:

		pkt = new PNATTestAckPkt( );
		pkt->deserialize(inContent, 0 );
		break;

	case PPktDef::NAT_TEST_REQ:

		pkt = new PNATTestReqPkt( );
		pkt->deserialize(inContent, 0 );
		break;

	case PPktDef::NAT_RELAY:

		pkt = new PNATRelayPkt( );
		pkt->deserialize(inContent, 0 );
		break;

	default:
		assert(0);// imposssible.
	}
	return pkt;

}
