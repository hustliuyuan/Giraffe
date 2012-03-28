#include "Base/GHeader.h"
#include "logic/GCLCommLogger.h"
#include "logic/GCLComm.h"
#include "Packets/Giraffe/PGRFPackets.h"
#include "Packets/Giraffe/paxos/PGRFPaxosPackets.h"
#include "GCLFileCache.h"
#include "Base/GCommon.h"
#include "Session.h"

Session::Session()
{
	m_watcherMgr = WatcherManager::getInstance();
	m_watcherMgr->start();
	m_pComm = new GCLComm(this);
	PSettings & settings = PSettings::getInstance();		
	U32 tempIp = PUtils::addr2Long( settings.getConfig("giraffe-server-ip").m_value.strvalue );
	U16 tempPort = settings.getConfig("giraffe-server-tcp-port").m_value.intvalue;
	m_replica_addr.set(tempIp,tempPort);

	tempIp = PUtils::addr2Long( settings.getConfig("local-ip").m_value.strvalue );
	tempPort = settings.getConfig("local-tcp-server-port").m_value.intvalue;
	m_loc_addr.set(tempIp,tempPort);

	m_conditioner = new Conditioner();
	m_lastkeepalive_time = PUtils::localMilliSeconds();
	m_connected = false;
	m_sessionId = 0;
	m_lease_interval = settings.getConfig("giraffe-lease-interval").m_value.intvalue;
}

Session::~Session(void)
{

}

void Session::queryClusterLeaderLocation()
{
	U32 id = 0;
	PPacketBase* acceptedPkt = (PGRFLeaderLocationReqPkt*)new PGRFLeaderLocationReqPkt(id);
	m_pComm->tcpSend(m_replica_addr, acceptedPkt);

	boost::mutex::scoped_lock lck(m_leader_location_mutex);
	if(m_sessionId == 0) 
		m_leader_location_condition.wait(lck);
}


void Session::getClusterLeaderLocation(Network::PInetAddr leaderLoc)
{
	boost::mutex::scoped_lock lck(m_leader_location_mutex);
	m_leader_addr = leaderLoc;
	m_leader_location_condition.notify_one();
}

bool Session::connect()
{
	PPacketBase* acceptedPkt = (PGRFKeepAliveReqPkt*)new PGRFKeepAliveReqPkt(0, m_loc_addr);
	m_pComm->tcpSend(m_replica_addr, acceptedPkt);
	m_lastkeepalive_time = PUtils::localMilliSeconds();
	boost::mutex::scoped_lock lck(m_session_mutex);
	if(m_sessionId == 0) 
	{
		cout << "try connecting!!" << endl;
		m_session_condition.wait(lck);
	}
	return true;
}

int Session::open(string filename, U32 flags, string content,Watcher *w)
{
	return open(filename,w);
}

int Session::open(string filename,Watcher *w)
{
	U32 id = m_conditioner->addConditioner();

	std::map<String, String> attr;
	PGRFOpenReqPkt * openRootFileReqPkt = new PGRFOpenReqPkt(id,m_sessionId,OPEN_FLAG_CREATE,filename,0,attr);
	m_pComm->tcpSend(m_replica_addr,openRootFileReqPkt);
	
	const OpenCallbackEvent *event = (const OpenCallbackEvent *) m_conditioner->waitingForConditioner(id);

	if(event)
	{
		cout << "get open res!!" << endl;
		GCLFileNode node(filename, event->m_iHandle, false);
		cout << "insert" << endl;
		m_fileCache.insertFileNode(filename, node);
		cout << "get open res!!" << endl;
		if(w)
			m_watcherMgr->insertWatcher(w);
		return event->m_iHandle;
	}
	else
	{
		return -1;
	}
}

bool Session::mkdir(string dirname)
{
	U32 id = m_conditioner->addConditioner();

	PGRFMkdirReqPkt * mkdirReqPkt = new PGRFMkdirReqPkt(id,m_sessionId,dirname);
	m_pComm->tcpSend(m_replica_addr,mkdirReqPkt);
	const MkdirCallbackEvent *event = (const MkdirCallbackEvent*)m_conditioner->waitingForConditioner(id);
	//const Network::PacketINFO * packetInfo = m_conditioner->waitingForConditioner(id);
	//PGRFMkdirResPkt mkDirResPkt = *(PGRFMkdirResPkt *)packetInfo->packet;
	if (event)
	{
		cout << "dir name: " << event->m_sDirName << endl;
		return event->m_bOk;
	}
	return false;	
}

bool Session::lock(string filename,Watcher *w)
{
	GCLFileNode node(filename, 0, false);
	bool flag = m_fileCache.getFileNode(filename, node);
	int fileHandle;
	if(flag)
		fileHandle = node.m_handle;
	else
		fileHandle = open(filename);

	U32 id = m_conditioner->addConditioner();

	PGRFLockReqPkt * lckReqPkt = new PGRFLockReqPkt(id,m_sessionId,fileHandle,LOCK_EXCLUSIVE_MODE,false);
	m_pComm->tcpSend(m_replica_addr,lckReqPkt);
	cout << "apply lock to server" << endl;
	const LockCallbackEvent *event = (const LockCallbackEvent*)m_conditioner->waitingForConditioner(id);
	if (!event)
	{
		return  false;
	}
	//const Network::PacketINFO * packetInfo = m_conditioner->waitingForConditioner(id);
	cout << "get n res!!" << endl;
	//PGRFLockResPkt lckResPkt = *(PGRFLockResPkt *)packetInfo->packet;
	//assert(lckResPkt != NULL);
	if(event->m_iLockStatus == LOCK_STATUS_GRANTED)
	{
		cout << "lock success!! lock generater number ::" << event->m_iLockGen << endl;
		filelock.insert(pair<string, U32>(filename, event->m_iLockGen));

		m_fileCache.getFileNode(filename, node);
		node.setLock(event->m_iLockGen);
		m_fileCache.insertFileNode(filename, node);

		return true;
	}
	else
	{
		if(w)
			m_watcherMgr->insertWatcher(w);
		return false;
	}
}

void Session::close(string filename)
{
	int fileHandle = fileIndex[filename];

	if(fileHandle == 0) return;
	U32 id = m_conditioner->addConditioner();

	PGRFCloseReqPkt * closeFileReqPkt = new PGRFCloseReqPkt(id,m_sessionId,fileHandle);

	m_pComm->tcpSend(m_replica_addr,closeFileReqPkt);
	const CloseCallbackEvent *event = (const CloseCallbackEvent*)m_conditioner->waitingForConditioner(id);
	//const Network::PacketINFO * packetInfo = m_conditioner->waitingForConditioner(id);
	//PGRFCloseResPkt resPkt = *(PGRFCloseResPkt *)packetInfo->packet;
}

string Session::getData(string filename,Watcher *w)
{
	string content;
	int fileHandle;
	GCLFileNode node(filename, 0, false);
	bool flag = m_fileCache.getFileNode(filename, node);
	if(flag)
	{
		bool readflag = node.getData(content);
		if(readflag) 
			return content;
		else
			fileHandle = node.m_handle;
		cout << "can't read data from cache!!" <<endl;
	}
	else
	{
		cout << "node is not in the cache!" << endl;
		fileHandle = open(filename);
	}

	U32 id = m_conditioner->addConditioner();
	
	std::vector<String> attr;
	attr.push_back("content");

	PGRFAttrGetReqPkt * getdataReqPkt = new PGRFAttrGetReqPkt(id,m_sessionId,fileHandle,1,attr);
	m_pComm->tcpSend(m_replica_addr,getdataReqPkt);
	const ReadCallbackEvent *event = (const ReadCallbackEvent*)m_conditioner->waitingForConditioner(id);
	//const Network::PacketINFO * packetInfo = m_conditioner->waitingForConditioner(id);
	if (!event)
	{
		cout << "fail read" << endl;
		return "";
	}
	cout << "success read!" <<endl;	
	//PGRFAttrGetResPkt resPkt = *(PGRFAttrGetResPkt *)packetInfo->packet;
	m_fileCache.getFileNode(filename, node);
	//string value = event->m_oAttrs["content"];
	node.setData(event->m_sContent);
	//node.setData(event->m_oAttrs["content"]);
	m_fileCache.insertFileNode(filename, node);

	if(w)
		m_watcherMgr->insertWatcher(w);

	return event->m_sContent;
	//return event->m_oAttrs["content"];
}

bool Session::setData(string filename, string content,Watcher *w)
{
	int fileHandle;
	GCLFileNode node(filename, 0, false);
	bool flag = m_fileCache.getFileNode(filename, node);
	if(flag)
	{
		fileHandle = node.m_handle;
	}
	else
	{
		fileHandle = open(filename);
	}

	U32 id = m_conditioner->addConditioner();
	//int fileHandle = fileIndex[filename];
	std::map<String, String> attr;
	attr["content"] = content;

	PGRFAttrSetReqPkt * setdataReqPkt = new PGRFAttrSetReqPkt(id,m_sessionId,fileHandle,"content",content);

	m_pComm->tcpSend(m_replica_addr,setdataReqPkt);
	const WriteCallbackEvent *event = (const WriteCallbackEvent*)m_conditioner->waitingForConditioner(id);
	//const Network::PacketINFO * packetInfo = m_conditioner->waitingForConditioner(id);
	if (!event)
	{
		cout << "write fail!" <<endl;	
		return false;
	}
	cout << "success write!" <<endl;	
	//PGRFAttrSetResPkt resPkt = *(PGRFAttrSetResPkt *)packetInfo->packet;
	if(event->m_bOk)
	{
		m_fileCache.getFileNode(filename, node);
		node.setData(content);
		m_fileCache.insertFileNode(filename, node);

		return true;
	}
	else
	{
		return false;
	}
	
}

vector<String> Session::getChildren(string dirname)
{
	U32 id = m_conditioner->addConditioner();
	PGRFReadDirReqPkt * getDirListReqPkt = new PGRFReadDirReqPkt(id,m_sessionId,dirname);

	m_pComm->tcpSend(m_replica_addr,getDirListReqPkt);
	const ReadDirCallbackEvent * event = (const ReadDirCallbackEvent*)m_conditioner->waitingForConditioner(id);
	//const Network::PacketINFO * packetInfo = m_conditioner->waitingForConditioner(id);
	//PGRFReadDirResPkt resPkt = *(PGRFReadDirResPkt *)packetInfo->packet;
	if (!event)
	{
		vector<String> vv;
		return vv;
	}
	return event->m_oFiles;
}

bool Session::exits(string filename)
{
	U32 id = m_conditioner->addConditioner();
	
	PGRFExitsReqPkt * exitsReqPkt = new PGRFExitsReqPkt(id,m_sessionId,filename);
	m_pComm->tcpSend(m_replica_addr,exitsReqPkt);
	const ExitsCallbackEvent *event = (const ExitsCallbackEvent*)m_conditioner->waitingForConditioner(id);
	//const Network::PacketINFO * packetInfo = m_conditioner->waitingForConditioner(id);
	//PGRFExitsResPkt resPkt = *(PGRFExitsResPkt *)packetInfo->packet;
	if (!event)
	{
		return false;
	}

	if(event->m_bExits)
		return true;
	else
		return false;
}


bool Session::release(string filename)
{
	U32 lock;
	int fileHandle;
	GCLFileNode node(filename, 0, false);
	bool flag = m_fileCache.getFileNode(filename, node);
	if(flag)
	{
		fileHandle = node.m_handle;
		bool islock = node.getLock(lock);
		if(!islock) 
			return true;
	}
	else
	{
		return false;
	}

	U32 id = 0;
	PGRFReleaseReqPkt * unlckReqPkt = new PGRFReleaseReqPkt(id,m_sessionId,fileHandle,lock);
	m_pComm->tcpSend(m_replica_addr,unlckReqPkt);

	node.m_isLock = false;
	m_fileCache.insertFileNode(filename, node);
	return true;
}

bool Session::invalidateCache(string nodename)
{
	cout << "invalidate Cache in session part" <<endl;
	GCLFileNode node(nodename, 0, false);
	bool flag = m_fileCache.getFileNode(nodename, node);
	if(flag)
	{
		node.inValidate();
		m_fileCache.insertFileNode(nodename, node);
		return true;
	}
	return false;
	
}

void Session::getGiraffeCluster()
{

}

bool Session::reconnected()
{
	return true;
}

void Session::keepalive(U32 id)
{
	m_sessionId = id;
	cout << "id: " << id << endl;
}

void Session::flush_file()
{
	map<string,U32>::iterator it = fileIndex.begin();
	for( ; it != fileIndex.end(); it ++)
	{
		cout << "file name: " << it->first << "  file handle: " << it->second << endl;
	}
}

void Session::keepalive()
{	
	S64 time = PUtils::localMilliSeconds();
	if((time - m_lastkeepalive_time > m_lease_interval))
	{
		if(!m_connected)
			cout << "try connecting!!" << endl;
		PPacketBase* heatbeatPkt = (PGRFKeepAliveReqPkt*)new PGRFKeepAliveReqPkt(m_sessionId, m_loc_addr);
		m_pComm->tcpSend(m_replica_addr, heatbeatPkt);
		m_lastkeepalive_time = time;
	}	
}

void Session::renewState(int id)
{
	if(!m_connected)
	{
		m_connected = true;
		boost::mutex::scoped_lock lck(m_session_mutex);
		m_sessionId = id;
		m_session_condition.notify_one();
		m_lastkeepalive_time = PUtils::localMilliSeconds();	
	}
	else if(id == m_sessionId)
	{
		m_lastkeepalive_time = PUtils::localMilliSeconds();
	}
}

void Session::sessionTerminate()
{
	m_pComm->setAppTerminate();
}
