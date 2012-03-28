#include "Base/GHeader.h"
#include "GCLPktParser.h"
#include "Base/PPktDef.h"
#include "Packets/Giraffe/PGRFPackets.h"
#include "Packets/Giraffe/paxos/PGRFPaxosPackets.h"

GCLPktParser::GCLPktParser(void)
{
}

GCLPktParser::~GCLPktParser(void)
{
}

const PPacketBase *	 
GCLPktParser::parsePkt( const S8 * inContent, 
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
		case PMyPktDef::GRF_LEADER_LOCATION_RES:
			pkt = new PGRFLeaderLocationResPkt;
		    pkt->deserialize(inContent,0);
			break;
		case PMyPktDef::GRF_KEEPALIVE_RES:
			pkt = new PGRFKeepAliveResPkt;
		    pkt->deserialize(inContent,0);
			break;
		case PMyPktDef::GRF_MKDIR_RES:
			pkt = new PGRFMkdirResPkt;
		    pkt->deserialize(inContent,0);
			break;
		case PMyPktDef::GRF_OPEN_RES:
			pkt = new PGRFOpenResPkt;
		    pkt->deserialize(inContent,0);
			break;
		case PMyPktDef::GRF_ATTR_GET_RES:
			pkt = new PGRFAttrGetResPkt;
		    pkt->deserialize(inContent,0);
			break;
		case PMyPktDef::GRF_DIR_ATTR_GET_RES:
			pkt = new PGRFDirAttrGetResPkt;
		    pkt->deserialize(inContent,0);
			break;
		case PMyPktDef::GRF_LOCK_RES:
			pkt = new PGRFLockResPkt;
		    pkt->deserialize(inContent,0);
			break;
		case PMyPktDef::GRF_EXITS_RES:
			pkt = new PGRFExitsResPkt;
		    pkt->deserialize(inContent,0);
			break;
		case PMyPktDef::GRF_DIR_ATTR_SET_RES:
			pkt = new PGRFDirAttrSetResPkt;
		    pkt->deserialize(inContent,0);
			break;
		case PMyPktDef::GRF_ATTR_SET_RES:
			pkt = new PGRFAttrSetResPkt;
		    pkt->deserialize(inContent,0);
			break;
		case PMyPktDef::GRF_READ_DIR_RES:
			pkt = new PGRFReadDirResPkt;
		    pkt->deserialize(inContent,0);
			break;
		case PMyPktDef::GRF_CLOSE_RES:
			pkt = new PGRFCloseResPkt;
		    pkt->deserialize(inContent,0);
			break;
		case PMyPktDef::GRF_WATCHER_EVENT:
			pkt = new PGRFWatcherEventPkt;
		    pkt->deserialize(inContent,0);
			break;

	default:
		assert(0);
	}
	return pkt;
}
