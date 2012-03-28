#include "stdafx.h"
#include "GMSPktParser.h"
#include "Base/PPktDef.h"
#include "Packets/Giraffe/PGRFPackets.h"
#include "Packets/Giraffe/paxos/PGRFPaxosPackets.h"



GMSPktParser::GMSPktParser(void)
{
	
}

GMSPktParser::~GMSPktParser(void)
{

}

/*
	GRF_OPEN_REQ								= 101,
		GRF_OPEN_RES								= 102,
		GRF_CLOSE_REQ							= 103,
		GRF_EXITS_REQ							= 104,
		GRF_EXITS_RES							= 105,
		GRF_ATTR_GET_REQ							= 106,
		GRF_ATTR_GET_RES							= 107,
		GRF_ATTR_SET_REQ							= 108,
		GRF_ATTR_SET_RES							= 109,
		GRF_LOCK_REQ								= 110,
		GRF_LOCK_RES								= 111,
		GRF_RELEASE_REQ							= 112,
		GRF_RELEASE_RES							= 113,
		GRF_MKDIR_REQ							= 114,
		GRF_MKDIR_RES							= 115,
		GRF_READ_DIR_REQ							= 116,
		GRF_READ_DIR_RES							= 117,
		GRF_UNLINK_REQ							= 118,
		GRF_UNLINK_RES							= 119,
		GRF_KEEPALIVE_REQ						= 120,
		GRF_KEEPALIVE_RES						= 121,
		GRF_DIR_ATTR_SET_REQ						= 122,
		GRF_DIR_ATTR_SET_RES						= 123,
		GRF_DIR_ATTR_GET_REQ						= 124,
		GRF_DIR_ATTR_GET_RES						= 125,
		GRF_CLOSE_RES							= 126,
*/

const PPacketBase *	 
GMSPktParser::parsePkt( const S8 * inContent, 
					  U16 inContentLen, U32 socket_type ) const 
{
    UNREFERENCED_PARAMETER( inContentLen );
    UNREFERENCED_PARAMETER( socket_type );

	//make sure socket_type_id is DEFAULT_SOCKET_TYPE;	
	U8 subType	= (U8)inContent[PACKET_SUBTYPE_OFFSET];

	PPacketBase *pkt = NULL;

	using namespace PMyPktDef;

	switch ( subType ) 
	{


	case GRF_PAXOS_EVENT:
		pkt = (PPacketBase*) new PGRFPaxosEventPkt;
		//pkt = new A;
		pkt->deserialize(inContent,0);
		break;

	case GRF_OPEN_REQ:
		pkt = (PPacketBase*)new PGRFOpenReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_CLOSE_REQ:
		pkt = (PPacketBase*)new PGRFCloseReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_EXITS_REQ :
		pkt = (PPacketBase*)new PGRFExitsReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_ATTR_GET_REQ:
		pkt = (PPacketBase*)new PGRFAttrGetReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_ATTR_SET_REQ:
		pkt = (PPacketBase*)new PGRFAttrSetReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_LOCK_REQ:
		pkt = (PPacketBase*)new PGRFLockReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_RELEASE_REQ:
		pkt = (PPacketBase*)new PGRFReleaseReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_MKDIR_REQ:
		pkt = (PPacketBase*)new PGRFMkdirReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_READ_DIR_REQ:
		pkt = (PPacketBase*)new PGRFReadDirReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_UNLINK_REQ:
		pkt = (PPacketBase*)new PGRFUnlinkReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_KEEPALIVE_REQ:
		pkt = (PPacketBase*)new PGRFKeepAliveReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_DIR_ATTR_SET_REQ:
		pkt = (PPacketBase*)new PGRFDirAttrSetReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_DIR_ATTR_GET_REQ:
		pkt = (PPacketBase*)new PGRFDirAttrGetReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_DELETE_FILE_REQ:
		pkt = (PPacketBase*)new PGRFDeleteFileReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_PAXOS_KEEPALIVE_LEADER_REQ:
		pkt = (PPacketBase*)new PGRFPaxosLeaderKeepAliveReq;
		pkt->deserialize(inContent,0);
		break;

	case GRF_PAXOS_KEEPALIVE_LEADER_RES:
		pkt = (PPacketBase*)new PGRFPaxosLeaderKeepAliveRes;
		pkt->deserialize(inContent,0);
		break;

	case GRF_LEADER_LOCATION_REQ:
		pkt = (PPacketBase*)new PGRFLeaderLocationReqPkt;
		pkt->deserialize(inContent,0);
		break;

	default:
		assert(0);
	}
	return pkt;
}
