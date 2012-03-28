#include "Base/GHeader.h"
#include "Base/PPktDef.h"
#include "Base/WatcherEventType.h"
#include "Packets/Giraffe/PGRFPackets.h"
#include "Packets/Giraffe/paxos/PGRFPaxosPackets.h"
#include "Session.h"
#include "CallbackEvent.h"
#include "GCLPktHandler.h"

using namespace Network;

GCLPktHandler::GCLPktHandler(Network::PTransmitter	*transmiter, Session * session/*, IAppTerminate * iappquit */)
{
	PSettings & settings = PSettings::getInstance();

	m_iIP = PUtils::addr2Long( settings.getConfig("local-ip").m_value.strvalue );
	cout << "ip " << settings.getConfig("local-ip").m_value.strvalue  << endl;
	this->transmiter=transmiter;
	this->m_session = session;
	m_watcherManger = WatcherManager::getInstance();
	m_curTime = PUtils::localSeconds();

	m_iInterval_time = settings.getConfig("query_address_interval-seconds").m_value.intvalue;
}

void GCLPktHandler::handleInternalQuitApplication( const PacketINFO * packetInfo)
{
	this->m_pAppterminate->applicationTerminate();
}

GCLPktHandler::~GCLPktHandler() 
{
	
}

void GCLPktHandler::setAppTerminate(IAppTerminate * iappquit)
{
	m_pAppterminate = iappquit;
}

void	GCLPktHandler::maintain( )
{	
	m_session->keepalive();
}

void	GCLPktHandler::onRun( )
{
	maintain();
}

std::string		GCLPktHandler::getName()const
{
	return "GMSPktHandler";
}


void	GCLPktHandler::handlePkt( const PacketINFO *  packetInfo)
{

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
		case PMyPktDef::GRF_KEEPALIVE_RES:
			this->handleKeepAliveResApplication(packetInfo);
		break;
			
		case PMyPktDef::GRF_MKDIR_RES:
			this->handleMkDirResApplication(packetInfo);
			break;

		case PMyPktDef::GRF_OPEN_RES:
			this->handleOpenFileResApplication(packetInfo);
			break;
		case PMyPktDef::GRF_LOCK_RES:
			this->handleLockResPkt(packetInfo);
			break;
		case PMyPktDef::GRF_ATTR_SET_RES:
			this->handleSetAttrResApplication(packetInfo);
			break;
		case PMyPktDef::GRF_ATTR_GET_RES:
			this->handleGetAttrResApplication(packetInfo);
			break;
		case PMyPktDef::GRF_LEADER_LOCATION_RES:
			this->handleGetLeaderLocResApplication(packetInfo);
			break;
		case PMyPktDef::GRF_READ_DIR_RES:
			this->handleReadDirResApplication(packetInfo);
			break;
		case PMyPktDef::GRF_CLOSE_RES:
			this->handleCloseFileResApplication(packetInfo);
			break;
		case PMyPktDef::GRF_EXITS_RES:
			this->handleExitsResApplication(packetInfo);
			break;
		case PMyPktDef::GRF_WATCHER_EVENT:
			this->handleWatcherEventApplication(packetInfo);
			break;
		default:
			assert(0);
			break;
		}
	}
	deletePktInfo(packetInfo);
}

void GCLPktHandler::handleWatcherEventApplication(const PacketINFO * packetInfoa)
{
	PGRFWatcherEventPkt * watcherEventPkt = (PGRFWatcherEventPkt *)packetInfoa->packet;
	WatcherEvent we(watcherEventPkt->m_iWatchererEvent, watcherEventPkt->m_sNodeName);
	m_watcherManger->pushEvent(we);
}

void GCLPktHandler::handleExitsResApplication(const PacketINFO * packetInfoa)
{
	PGRFExitsResPkt * exitsResPkt = (PGRFExitsResPkt *)packetInfoa->packet;

	U32 id = exitsResPkt->m_event;
	ExitsCallbackEvent* event =new ExitsCallbackEvent(exitsResPkt->m_sName, exitsResPkt->m_bExit);
	m_session->m_conditioner->callConditioner(id,event);
}


void GCLPktHandler::handleReadDirResApplication(const PacketINFO * packetInfoa)
{
	PGRFReadDirResPkt * readDirResPkt = (PGRFReadDirResPkt *)packetInfoa->packet;

	U32 id = readDirResPkt->m_event;
	ReadDirCallbackEvent *event = new ReadDirCallbackEvent(readDirResPkt->m_iEntryCount, readDirResPkt->m_entryNameList);

	m_session->m_conditioner->callConditioner(id,event);
}

void GCLPktHandler::handleCloseFileResApplication(const PacketINFO * packetInfoa)
{
	PGRFCloseResPkt * closeResPkt = (PGRFCloseResPkt *)packetInfoa->packet;

	U32 id = closeResPkt->m_event;
	CloseCallbackEvent *event = new CloseCallbackEvent(closeResPkt->m_iHandle,closeResPkt->m_bOK);
	m_session->m_conditioner->callConditioner(id,event);
}

void GCLPktHandler::handleGetLeaderLocResApplication(const PacketINFO * packetInfoa)
{
	PGRFLeaderLocationResPkt * leaderLocResPkt = (PGRFLeaderLocationResPkt *) packetInfoa->packet;
	m_session->getClusterLeaderLocation(leaderLocResPkt->m_oAddr);
}

void GCLPktHandler::handleKeepAliveResApplication(const PacketINFO * packetInfoa)
{
	PGRFKeepAliveResPkt * keepAliveResPkt = (PGRFKeepAliveResPkt *) packetInfoa->packet;
//	cout << "get server keepalive respone!!" << endl;	
	m_session->renewState(keepAliveResPkt->m_iSessionID);
}

void GCLPktHandler::handleMkDirResApplication(const PacketINFO * packetInfoa)
{
	PGRFMkdirResPkt * mkDirResPkt = (PGRFMkdirResPkt *)packetInfoa->packet;
	if(mkDirResPkt->m_bOK)
	{
		U32 id = mkDirResPkt->m_event;
		MkdirCallbackEvent *event = new MkdirCallbackEvent(mkDirResPkt->m_bOK, mkDirResPkt->m_sDirName);
		m_session->m_conditioner->callConditioner(id,event);
	}
	else
	{
		std::cout<<"Could not make dir: "<<mkDirResPkt->m_sDirName<<" in GRF!\n";
		return;
	}
}

void GCLPktHandler::handleOpenFileResApplication(const PacketINFO * packetInfoa)
{
	PGRFOpenResPkt * openResPkt = (PGRFOpenResPkt *)packetInfoa->packet;

	U32 id = openResPkt->m_event;
	OpenCallbackEvent *event = new OpenCallbackEvent(openResPkt->m_iHandle);
	m_session->m_conditioner->callConditioner(id,event);
}

void GCLPktHandler::handleLockResPkt(const PacketINFO * packetInfoa)
{
	PGRFLockResPkt * lockResPkt = (PGRFLockResPkt *)packetInfoa->packet; 
	U32 id = lockResPkt->m_event;
	LockCallbackEvent *event = new LockCallbackEvent(lockResPkt->m_iHandle, lockResPkt->m_iLockStatus, lockResPkt->m_iLockGeneration);
	m_session->m_conditioner->callConditioner(id,event);
}

void GCLPktHandler::handleSetAttrResApplication(const PacketINFO * packetInfoa)
{
	PGRFAttrSetResPkt * resPkt = (PGRFAttrSetResPkt *)packetInfoa->packet; 
	U32 id = resPkt->m_event;
	WriteCallbackEvent *event = new WriteCallbackEvent( resPkt->m_bOk, resPkt->m_iHandle);
	m_session->m_conditioner->callConditioner(id,event);
}

void GCLPktHandler::handleGetAttrResApplication(const PacketINFO * packetInfoa)
{
	PGRFAttrGetResPkt * resPkt = (PGRFAttrGetResPkt *)packetInfoa->packet; 
	U32 id = resPkt->m_event;
	ReadCallbackEvent *event = new ReadCallbackEvent(resPkt->m_iHandle, resPkt->m_iAttrCount, resPkt->m_attrs);
	m_session->m_conditioner->callConditioner(id,event);
}
