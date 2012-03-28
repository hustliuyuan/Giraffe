#include "Base/GHeader.h"
#include "PPktDef.h"

String PMyPktDef::type2String( U8 type, U8 subtype )
{
	String s;
	switch( type )
	{

	case PMyPktDef::MY_MAJOR_PKT:
		s.append("MY_MAJOR_PKT:");
		break;
	case PMyPktDef::HTTP_PKT:
		s.append("DFS_PKT:");
		break;
	default:
		s.append ( PPktDef::type2String(type,subtype) );
		return s;			
	}
	if( type ==  PMyPktDef::MY_MAJOR_PKT )
	{
		switch( subtype ) 
		{
		case QUIT_APPLICATION:
			s.append("QUIT_APPLICATION");
			break;

		case GRF_LEADER_LOCATION_REQ:
			s.append("LEADER LOCATION REQUEST");
			break;
		case GRF_LEADER_LOCATION_RES:
			s.append("LEADER LOCATION RESPONDE");
			break;
		case GRF_PAXOS_EVENT:
			s.append("PAXOS EVENT");
			break;
		case GRF_BROADCAST_EVENT:
			s.append("PAXOS BREADCAST EVENT");
			break;
		case GRF_PAXOS_KEEPALIVE_LEADER_REQ:
			s.append("PAXOS EVENT: CONNECT TO LEADER");
			break;

		case GRF_PAXOS_KEEPALIVE_LEADER_RES:
			s.append("PAXOS EVENT: LEADER RESPOND TO FOLLOWER");
			break;
	
		case GRF_MKDIR_REQ:
			s.append("MAKE DIR REQ");
			break;
		
		case GRF_MKDIR_RES:
			s.append("MAKE DIR RES");
			break;

		case GRF_OPEN_REQ:
			s.append("OPEN FILE REQUEST");
			break;

		case GRF_OPEN_RES:
			s.append("OPEN FILE RESPONSE");
			break;

		case GRF_CLOSE_REQ:
			s.append("CLOSE FILE REQUEST");
			break;
		case GRF_CLOSE_RES:
			s.append("CLOSE FILE RES");
			break;

		case GRF_EXITS_REQ:
			s.append("FILE EXITS QUERY");
			break;
		case GRF_EXITS_RES:
			s.append("QUERY FIEL EXIT RESPONSE");
			break;
		case GRF_ATTR_GET_REQ:
			s.append("GET FILE ATTRIBUTE REQ");
			break;

		case GRF_ATTR_GET_RES:
			s.append("GET FILE ATTR RES");
			break;

		case GRF_ATTR_SET_REQ:
			s.append("SET FILE ATTTR REQ");
			break;

		case GRF_ATTR_SET_RES:
			s.append("SET FILE ATTR RES");
			break;

		case GRF_LOCK_REQ:
			s.append("LOCK FILE REQ");
			break;

		case GRF_LOCK_RES:
			s.append("LOCK FILE RES");
			break;

		case GRF_RELEASE_REQ:
			s.append("RELEASE LOCK REQ");
			break;

		case GRF_RELEASE_RES:
			s.append("RELEASE LOCK RES");
			break;


		case GRF_READ_DIR_REQ:
			s.append("READ DIR REQ");
			break;

		case GRF_READ_DIR_RES:
			s.append("READ DIR RES");
			break;

		case GRF_UNLINK_REQ:
			s.append("DELETE FILE REQ");
			break;

		case GRF_UNLINK_RES:
			s.append("DELETE FILE RES");
			break;

		case GRF_KEEPALIVE_REQ:
			s.append("KEEPALIVE REQ");
			break;

			
		case GRF_KEEPALIVE_RES:
			s.append("KEEPALIVE RES");
			break;
		
		case GRF_DIR_ATTR_SET_REQ:
			s.append("SET DIR ATTRS REQ");
			break;

		case GRF_DIR_ATTR_SET_RES:
			s.append("SET DIR ATTRS RES");
			break;

		case GRF_DIR_ATTR_GET_REQ:
			s.append("GET DIR ATTRS REQ");
			break;

		case GRF_DIR_ATTR_GET_RES:
			s.append("GET DIR ATTRS RES");
			break;

		case GRF_DELETE_FILE_REQ:
			s.append("	delete file request");
			break;

		case GRF_DELETE_FILE_RES:
			s.append("	delete file request");
			break;

		case GRF_WATCHER_EVENT:
			s.append("	delete file request");
			break;

		default:
			s.append("wrong pkt type:");
			assert(0);
			break;
		}
	}

	return s;
	//THE FOLLOWING IS REMAINED FOR FURTURE EXTENSION
	/*
	else if (type ==  PMyPktDef::HTTP_PKT )
	{
		switch( subtype ) 
		{
		case STORE_ADDRESS_REQ:
			s.append("STORE_ADDRESS_REQ");
			break;
		default:
			s.append("wrong pkt type:");
			assert(0);
			break;
		}
	}
	
	*/
}
