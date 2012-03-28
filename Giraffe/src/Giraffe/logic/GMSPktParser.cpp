#include "Base/GHeader.h"
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
		pkt = new PGRFPaxosEventPkt;
		//pkt = new A;
		pkt->deserialize(inContent,0);
		break;

	case GRF_OPEN_REQ:
		pkt = new PGRFOpenReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_CLOSE_REQ:
		pkt = new PGRFCloseReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_EXITS_REQ :
		pkt = new PGRFExitsReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_ATTR_GET_REQ:
		pkt = new PGRFAttrGetReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_ATTR_SET_REQ:
		pkt = new PGRFAttrSetReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_LOCK_REQ:
		pkt = new PGRFLockReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_RELEASE_REQ:
		pkt = new PGRFReleaseReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_MKDIR_REQ:
		pkt = new PGRFMkdirReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_READ_DIR_REQ:
		pkt = new PGRFReadDirReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_UNLINK_REQ:
		pkt = new PGRFUnlinkReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_KEEPALIVE_REQ:
		pkt = new PGRFKeepAliveReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_DIR_ATTR_SET_REQ:
		pkt = new PGRFDirAttrSetReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_DIR_ATTR_GET_REQ:
		pkt = new PGRFDirAttrGetReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_DELETE_FILE_REQ:
		pkt = new PGRFDeleteFileReqPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_PAXOS_KEEPALIVE_LEADER_REQ:
		pkt = new PGRFPaxosLeaderKeepAliveReq;
		pkt->deserialize(inContent,0);
		break;

	case GRF_PAXOS_KEEPALIVE_LEADER_RES:
		pkt = new PGRFPaxosLeaderKeepAliveRes;
		pkt->deserialize(inContent,0);
		break;
	case GRF_BROADCAST_EVENT:
		pkt = new PGRFBroadcastEventPkt;
		pkt->deserialize(inContent,0);
		break;

	case GRF_LEADER_LOCATION_REQ:
		pkt = new PGRFLeaderLocationReqPkt;
		pkt->deserialize(inContent,0);
		break;

	default:
		assert(0);
	}
	return pkt;
}
