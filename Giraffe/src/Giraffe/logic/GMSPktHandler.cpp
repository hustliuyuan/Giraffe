#include "Base/GHeader.h"
#include "GMSPktHandler.h"
#include "Base/PPktDef.h"
#include "Packets/Giraffe/PGRFPackets.h"
#include "Packets/Giraffe/paxos/PGRFPaxosPackets.h"
#include "GPaxosUtils.h"

typedef Network::PacketINFO PacketINFO;
typedef Network::PAddressable PAddressable;
typedef Network::PInetAddr  PInetAddr;

using namespace Network;

GMSPktHandler::GMSPktHandler(Network::PTransmitter	*transmiter/*, IAppTerminate * iappquit */)
{
	PSettings & settings = PSettings::getInstance();

	m_iIP = PUtils::addr2Long( settings.getConfig("local-ip").m_value.strvalue );
	cout << "ip:" <<  settings.getConfig("local-ip").m_value.strvalue << endl;
	//m_curTime = PUtils::seconds();

	this->transmiter=transmiter;

	m_iInterval_time = settings.getConfig("query_address_interval-seconds").m_value.intvalue;
}

GMSPktHandler::GMSPktHandler(Network::PTransmitter	*transmiter, SessionManager* pMs/*,OraDBConnectPool* connPool*/)
{
	PSettings & settings = PSettings::getInstance();

	m_iIP = PUtils::addr2Long( settings.getConfig("local-ip").m_value.strvalue );

	cout << "ip:" <<  settings.getConfig("local-ip").m_value.strvalue << endl;
	this->transmiter=transmiter;

	m_iInterval_time = settings.getConfig("query_address_interval-seconds").m_value.intvalue;

	m_pMaster = pMs;
}



void GMSPktHandler::handleInternalQuitApplication( const PacketINFO * packetInfo)
{
	this->m_pAppterminate->applicationTerminate();
}

GMSPktHandler::~GMSPktHandler() 
{
	
}

void GMSPktHandler::setAppTerminate(IAppTerminate * iappquit)
{
	m_pAppterminate = iappquit;
}

void	GMSPktHandler::maintain( )
{
	/*
	U32 time = PUtils::seconds();
	if(time - m_curTime >=10)
	{
		m_pMaster->start();
		m_curTime = time;
	}*/
	m_pMaster->check_state();

	
}

void	GMSPktHandler::onRun( )
{
	maintain();
}

std::string		GMSPktHandler::getName()const
{
	return "GMSPktHandler";
}


void	GMSPktHandler::handlePkt( const PacketINFO *  packetInfo)
{
//	cout << "handle application" << endl;
	U8 type = packetInfo->packet->getType();
	U8 subtype = packetInfo->packet->getSubType();

	char temp[2046] = {0};
	packetInfo->toString(temp);
	LOG_DEBUG("GMSPktHandler","handlePkt():"<<temp);
	
	using namespace PMyPktDef;
	
	if( type == MY_MAJOR_PKT )
	{
		switch(subtype)
		{
		case GRF_OPEN_REQ:
			this->handleFileOpenReq(packetInfo);
			break;

		case GRF_CLOSE_REQ:
			this->handleFileCloseReq(packetInfo);
			break;

		case GRF_EXITS_REQ :
			this->handleNodeExitsReq(packetInfo);
			break;

		case GRF_ATTR_GET_REQ:
			this->handleFileAttrGetReq(packetInfo);
			break;

		case GRF_ATTR_SET_REQ:
			this->handleFileAttrSetReq(packetInfo);
			break;

		case GRF_LOCK_REQ:
			this->handleFileLockReq(packetInfo);
			break;

		case GRF_RELEASE_REQ:
			this->handleUnlockReq(packetInfo);
			break;

		case GRF_MKDIR_REQ:
			this->handleDirMakeReq(packetInfo);
			break;

		case GRF_READ_DIR_REQ:
			this->handleDirReadReq(packetInfo);
			break;

		case GRF_UNLINK_REQ:
			this->handleNodeDelReq(packetInfo);
			break;

		case GRF_KEEPALIVE_REQ:
			this->handleKeepAliveReq(packetInfo);
			break;

		case GRF_DIR_ATTR_SET_REQ:
			this->handleDirAttrSetReq(packetInfo);
			break;

		case GRF_DIR_ATTR_GET_REQ:
			this->handleDirAttrGetReq(packetInfo);
			break;

		case GRF_DELETE_FILE_REQ:
			this->handleDelFileReq(packetInfo);
			break;

		case GRF_LEADER_LOCATION_REQ:
			this->handleLeaderLocationReq(packetInfo);
			break;

		default:
			assert(0);
			break;
		}
	}
	deletePktInfo(packetInfo);
}

void GMSPktHandler::handleLeaderLocationReq( const PacketINFO * pktinfo)
{
	Network::PInetAddr addrFrom = pktinfo->addr;
	S8 temp[10240] = {0};
	pktinfo->packet->toString(temp);
	const PGRFLeaderLocationReqPkt *pkt =  (const PGRFLeaderLocationReqPkt *)pktinfo->packet;
	m_pMaster->get_leader_location(pkt->m_iSessionId, addrFrom);
}

void GMSPktHandler::handleFileAttrGetReq( const PacketINFO * pktinfo)
{
	S8 temp[10240] = {0};
	pktinfo->packet->toString(temp);
	const PGRFAttrGetReqPkt *pkt =  (const PGRFAttrGetReqPkt *)pktinfo->packet;

	int sessionid = pkt->m_iSessionID;
	unsigned handle = pkt->m_iHandle;
	int cnt = pkt->m_iAttrCount;
	AttrKeyMap attrkey = pkt->m_attrNames;
	unsigned e = pkt->m_event;

	string name;
	m_pMaster->read(sessionid, name,handle,cnt,attrkey,e,false);

}

void GMSPktHandler::handleFileAttrSetReq( const PacketINFO * pktinfo)
{
	S8 temp[10240] = {0};
	pktinfo->packet->toString(temp);
	const PGRFAttrSetReqPkt *pkt =  (const PGRFAttrSetReqPkt *)pktinfo->packet;


	int sessionid = pkt->m_iSessionID;
	unsigned handle = pkt->m_iHandle;
	//int cnt = pkt->m_iAttrCount;
	AttrMap attrs;
	attrs[pkt->m_sAttrName] = pkt->m_sAttrVal;
	unsigned e = pkt->m_event;

	string name;
	m_pMaster->write(sessionid, name,handle,attrs, e, false);
}

void GMSPktHandler::handleFileCloseReq( const PacketINFO * pktinfo)
{
	S8 temp[10240] = {0};
	pktinfo->packet->toString(temp);
	const PGRFCloseReqPkt *pkt =  (const PGRFCloseReqPkt *)pktinfo->packet;

	int sessionid = pkt->m_iSessionID;
	unsigned handle = pkt->m_iHandle;
	//int cnt = pkt->m_iAttrCount;
	unsigned e = pkt->m_event;

	string name;
	m_pMaster->close(sessionid,handle,e);
}

void GMSPktHandler::handleDirAttrGetReq( const PacketINFO * pktinfo)
{
	S8 temp[10240] = {0};
	pktinfo->packet->toString(temp);
	const PGRFDirAttrGetReqPkt *pkt =  (const PGRFDirAttrGetReqPkt *)pktinfo->packet;

	int sessionid = pkt->m_iSessionID;
	string name = pkt->m_sDirName;
	unsigned e = pkt->m_event;
	m_pMaster->getlist(sessionid,name,e);
}

void GMSPktHandler::handleDirAttrSetReq( const PacketINFO * pktinfo)
{
	S8 temp[10240] = {0};
	pktinfo->packet->toString(temp);
	const PGRFDirAttrSetReqPkt *pkt =  (const PGRFDirAttrSetReqPkt *)pktinfo->packet;

	int sessionid = pkt->m_iSessionID;
	unsigned handle = 0;
	string name = pkt->m_sDirName;

	//int cnt = pkt->m_iAttrCount;
	AttrMap attrs;
	attrs[pkt->m_sAttrName] = pkt->m_sAttrVal;
	unsigned e = pkt->m_event;

	m_pMaster->write(sessionid, name,handle,attrs, e, true);
}

void GMSPktHandler::handleUnlockReq( const PacketINFO * pktinfo)
{
	S8 temp[10240] = {0};
	pktinfo->packet->toString(temp);
	const PGRFReleaseReqPkt *pkt =  (const PGRFReleaseReqPkt *)pktinfo->packet;

	int sessionid = pkt->m_iSessionID;
	unsigned handle = pkt->m_iHandle;
	U32 lock = pkt->m_iLockGeneration;
	m_pMaster->unlock(sessionid, handle, lock);
}

void GMSPktHandler::handleNodeExitsReq( const PacketINFO * pktinfo)
{
	S8 temp[10240] = {0};
	pktinfo->packet->toString(temp);
	const PGRFExitsReqPkt *pkt =  (const PGRFExitsReqPkt *)pktinfo->packet;

	int sessionid = pkt->m_iSessionID;
	string name = pkt->m_sFileName;
	unsigned e = pkt->m_event;
	m_pMaster->exits(sessionid, name, e);
}

void GMSPktHandler::handleKeepAliveReq( const PacketINFO * pktinfo)
{
	S8 temp[10240] = {0};
	pktinfo->packet->toString(temp);
	const PGRFKeepAliveReqPkt *pkt =  (const PGRFKeepAliveReqPkt *)pktinfo->packet;

	int sessionid = pkt->m_iSessionID;
	Network::PInetAddr addr = pkt->m_oAddr;
	cout << "client keepalive: id " << sessionid << endl;
	m_pMaster->keepalive(sessionid, addr);
}

void GMSPktHandler::handleFileLockReq( const PacketINFO * pktinfo)
{
	S8 temp[10240] = {0};
	pktinfo->packet->toString(temp);
	const PGRFLockReqPkt *pkt =  (const PGRFLockReqPkt *)pktinfo->packet;

	int sessionid = pkt->m_iSessionID;
	int handle = pkt->m_iHandle;
	int mode = pkt->m_iLockMode;
	bool trylck = pkt->m_bTryLock;
	int e = pkt->m_event;

	m_pMaster->lock(sessionid, handle, mode, trylck, e);
}

void GMSPktHandler::handleDirMakeReq( const PacketINFO * pktinfo)
{
	S8 temp[10240] = {0};
	pktinfo->packet->toString(temp);
	const PGRFMkdirReqPkt *pkt =  (const PGRFMkdirReqPkt *)pktinfo->packet;

	int sessionid = pkt->m_iSessionID;
	string name = pkt->m_sDirName;
	int e = pkt->m_event;

	m_pMaster->mkDir(sessionid, name, e);
}

void GMSPktHandler::handleFileOpenReq( const PacketINFO * pktinfo)
{
	S8 temp[10240] = {0};
	pktinfo->packet->toString(temp);
	const PGRFOpenReqPkt *pkt =  (const PGRFOpenReqPkt *)pktinfo->packet;

	int sessionid = pkt->m_iSessionID;
	string name = pkt->m_sName;
	int flags = pkt->m_iFlags;
	AttrMap attrs = pkt->m_attrs;

	int e = pkt->m_event;

	m_pMaster->open(sessionid, name, flags,attrs, e);
}

void GMSPktHandler::handleDirReadReq( const PacketINFO * pktinfo)
{
	S8 temp[10240] = {0};
	pktinfo->packet->toString(temp);
	const PGRFReadDirReqPkt *pkt =  (const PGRFReadDirReqPkt *)pktinfo->packet;

	int sessionid = pkt->m_iSessionID;
	string name = pkt->m_sDirName;

	int e = pkt->m_event;

	m_pMaster->getlist(sessionid, name, e);
}

void GMSPktHandler::handleLockReleaseReq( const PacketINFO * pktinfo)
{
	S8 temp[10240] = {0};
	pktinfo->packet->toString(temp);
	const PGRFReleaseReqPkt *pkt =  (const PGRFReleaseReqPkt *)pktinfo->packet;

	int sessionid = pkt->m_iSessionID;
	unsigned handle = pkt->m_iHandle;

	int e = pkt->m_event;

	m_pMaster->unlock(sessionid, handle, e);
}

void GMSPktHandler::handleNodeDelReq( const PacketINFO * pktinfo)
{

}


void GMSPktHandler::handleDelFileReq( const PacketINFO * pktinfo)
{
	S8 temp[10240] = {0};
	pktinfo->packet->toString(temp);
	const PGRFDeleteFileReqPkt *pkt =  (const PGRFDeleteFileReqPkt *)pktinfo->packet;

	int sessionid = pkt->m_iSessionID;
	unsigned handle = pkt->m_iHandle;

	int e = pkt->m_iEvent;

	m_pMaster->del(sessionid, handle, e);
}
