#include "SessionManager.h"
#include "Packets/Giraffe/PGRFPackets.h"
#include "Base/WatcherEvent.h"
#include "Base/WatcherEventType.h"

class GMSComm;

SessionManager::SessionManager()
{
	//m_pComm = new GMSComm(this);
	PSettings & settings = PSettings::getInstance();	
	m_lease_interval = settings.getConfig("giraffe-lease-interval").m_value.intvalue;
	cout << "m_lease_interval " << m_lease_interval  << endl;
	m_base_dir = "./BerkelyDB";
	m_fs = new BerkelyDBFileSystem(m_base_dir);
	m_sessionCurrentID = 0;
	m_handleCurrentID = 0;
	m_lockGenerater = 0;
	m_expired_session_counter = 0;
	m_pComm = new GMSComm(this);

	m_db_run_time = 0;
	func_run_time = 0;
	m_curTime = PUtils::localMilliSeconds();
	
	m_begin_time = PUtils::localSeconds();
	m_operations = 0;
		
}

SessionManager::~SessionManager()
{
	//delete m_fs;
}

void SessionManager::check_state()
{
	static long l = 0;
	S64 time = PUtils::localMilliSeconds();
	if(time - m_curTime >=  m_lease_interval)
	{
		cout << "timely renew" << endl;
		m_curTime = time;
		this->timely_renew();
	}	
}

void SessionManager::get_leader_location(U32 id, Network::PInetAddr addrfrom)
{
	/*
	Network::PInetAddr addr;
	if(m_paxos->get_leader_location(addr))
	{
		PGRFLeaderLocationResPkt *pkt = new PGRFLeaderLocationResPkt(id,addr);

		m_pComm->tcpSend(addrfrom, pkt);
	}
	else
	{
		//send leader electing!!
	}*/
}

void SessionManager::open(unsigned int sessionid, const string name, int flags, AttrMap attrs, int e)
{
	SessionDataPtr sessionptr;
	HandleDataPtr handleptr;
	NodeDataPtr nodeptr;
	string normal_name;
	int handleid ;
	if (getSession(sessionid, sessionptr))
	{
		//get the handle of name
		normalize(name, normal_name);
		//if (!sessionptr->exitsHandle(handleid) || flags&OPEN_FLAG_CREATE)
		{
			//handleptr = HandleDataPtr(new HandleData(sessionid, normal_name));
			handleptr = new HandleData(sessionid, normal_name);
			handleid = createHandle(handleptr);
			sessionptr->addHandle(handleid);

			if(!getNode(normal_name, nodeptr))
			{
				//nodeptr = NodeDataPtr(new NodeData(normal_name));
				nodeptr = new NodeData(normal_name);
				nodeptr->addHandle(handleptr);//maybe error;
				createNode(normal_name,nodeptr);
			}
			else
			{
				nodeptr->addHandle(handleptr);
			}
			//if(flags & OPEN_FLAG_CREATE)
			if(!m_fs->exits(normal_name))
				m_fs->create_node(normal_name, attrs);

			Network::PInetAddr addr = sessionptr->getAddr();

			PGRFOpenResPkt *pkt = new PGRFOpenResPkt(e, handleid);
			m_pComm->tcpSend(addr, pkt);
			return ;
		}
	}
	//respone error to the session;
}

void SessionManager::del(unsigned int sessionid, unsigned int handleid, int e)
{
	SessionDataPtr sessionptr;
	HandleDataPtr handleptr;
	NodeDataPtr nodeptr;
	string normal_name;
	if (getSession(sessionid, sessionptr))
	{
		//get the handle of name
		if (sessionptr->exitsHandle(handleid))
		{
			sessionptr->rmHandle(handleid);	
			delHandle(handleid, handleptr);
		
			normal_name = handleptr->getNodeName();

			if(getNode(normal_name, nodeptr))
			{
				m_fs->del(normal_name);
				delNode(normal_name, nodeptr);
			}
			else 
			{
				//error the invalidate file
			}	

		}
		else
		{
			//error invalidate handle
		}
		//respone ok to the session;		
	}
	//respone error to the session;

}

void SessionManager::mkDir(unsigned int sessionid, const string name, int e)
{
	SessionDataPtr sessionptr;
	HandleDataPtr handleptr;
	NodeDataPtr nodeptr;
	string normal_name;
	unsigned int handleid;

	if (getSession(sessionid, sessionptr))
	{
		//get the handle of name
		normalize(name, normal_name);
		if(normal_name[normal_name.length() - 1] != '/')normal_name += '/';
		//handleid = ++ m_handleCurrentID;
		//if (!sessionptr->exitsHandle(handleid))
		{
			handleptr = HandleDataPtr(new HandleData(sessionid, normal_name));
			handleid = createHandle(handleptr);
			sessionptr->addHandle(handleid);
	
			if(!getNode(normal_name, nodeptr))
			{
				nodeptr = NodeDataPtr(new NodeData(normal_name));
				nodeptr->addHandle(handleptr);//maybe error;
				createNode(normal_name,nodeptr);
			}
			
			m_fs->mkdir(normal_name);

			Network::PInetAddr addr = sessionptr->getAddr();

			PGRFMkdirResPkt *pkt = new PGRFMkdirResPkt(e, name, true);

			m_pComm->tcpSend(addr, pkt);
			return;
		}
	}
}

void SessionManager::close(unsigned int sessionid, unsigned int handleid, unsigned e )
{
	SessionDataPtr sessionptr;
	HandleDataPtr handleptr;
	//NodeDataPtr nodeptr;
	string normal_name;
	if (getSession(sessionid, sessionptr))
	{
		if (sessionptr->exitsHandle(handleid))
		{
			sessionptr->rmHandle(handleid);
			delHandle(handleid,handleptr);
		}
		
		Network::PInetAddr addr = sessionptr->getAddr();
		PGRFCloseResPkt *pkt = new PGRFCloseResPkt(e, handleid, true);
		m_pComm->tcpSend(addr, pkt);

	}
}

void SessionManager::write(unsigned int sessionid, const string name, unsigned int handleid, AttrMap &attrs, int e, bool dir)
{
	SessionDataPtr sessionptr;
	HandleDataPtr handleptr;
	NodeDataPtr nodeptr;
	string normal_name;
	m_operations ++; // for throughput test
	if (getSession(sessionid, sessionptr))
	{
		if(getHandle(handleid, handleptr) )
		{
			normal_name = handleptr->getNodeName();
		//	cout << "name::" << normal_name << endl;
		}
		else
			normalize(name, normal_name);

		Network::PInetAddr addr = sessionptr->getAddr();	
		{			
			if(dir)
			{
				
				m_fs->set_node_attr(normal_name, attrs);
				PGRFDirAttrSetResPkt *pkt = new PGRFDirAttrSetResPkt(e, name, true);
				m_pComm->tcpSend(addr, pkt);
			}		
			else
			{
				
				PGRFAttrSetResPkt *pkt;
				getNode(normal_name, nodeptr);
				if(nodeptr->getMode() == UNLOCKED_MODE||nodeptr->m_exclusiveHandleId == handleid)
				{
					
					m_fs->set_node_attr(normal_name, attrs);
					nodeptr->rmHandle(handleptr);
					notify_watcher_event(WatcherEventType::NODE_MODIFIED, normal_name);
					nodeptr->addHandle(handleptr);
					pkt = new PGRFAttrSetResPkt(e, handleid, true);
				}
				else
				{
					pkt = new PGRFAttrSetResPkt(e, handleid, false);
				}
				m_pComm->tcpSend(addr, pkt);
			}
			return;

		}	
		//getNode(normal_name, nodeptr);
		//respone ok to the session;
	}
}

void SessionManager::read(unsigned int sessionid, const string name, unsigned int handleid, int cnt, AttrKeyMap &keys, int e, bool isdir)
{
	SessionDataPtr sessionptr;
	HandleDataPtr handleptr;
	string normal_name;
	AttrMap attrs;
	if (getSession(sessionid, sessionptr))
	{
		//get the handle of name
		if (sessionptr->exitsHandle(handleid))
		{
			if(getHandle(handleid, handleptr) )
			{
				normal_name = handleptr->getNodeName();
			}
			else
				normalize(name, normal_name);

			Network::PInetAddr addr = sessionptr->getAddr();
			{
				m_fs->get_node_attr(normal_name,keys, attrs);

				if(isdir)
				{
					PGRFDirAttrGetResPkt *pkt = new PGRFDirAttrGetResPkt(e, name, cnt, attrs);
					m_pComm->tcpSend(addr, pkt);
				}
				else
				{
					PGRFAttrGetResPkt *pkt = new PGRFAttrGetResPkt(e, handleid, cnt, attrs);
					m_pComm->tcpSend(addr, pkt);
				}
				return;
			}
		}	
		else
			cout << "Error" << endl;//handle does not exit;
		//getNode(normal_name, nodeptr);
		//respone context to the session;
	}
}

void SessionManager::getlist(unsigned int sessionid, const string &dirname, int e)
{
	SessionDataPtr sessionptr;
	//HandleDataPtr handleptr;
	//NodeDataPtr nodeptr;
	string normal_name;
	AttrKeyMap attrs;
	if (getSession(sessionid, sessionptr))
	{
		//get the handle of name
		normalize(dirname, normal_name);
		Network::PInetAddr addr = sessionptr->getAddr();
		if(normal_name[normal_name.length() - 1] != '/')
			normal_name += '/';
		
		m_fs->getChildren(normal_name,attrs);
		int cnt = attrs.size();
		PGRFReadDirResPkt *pkt = new PGRFReadDirResPkt(e, cnt, attrs);
		m_pComm->tcpSend(addr, pkt);
		return;
	}
}

void SessionManager::exits(unsigned int sessionid, string name, int e)
{
	SessionDataPtr sessionptr;
	bool exits;
	if (getSession(sessionid, sessionptr))
	{
		exits = m_fs->exits(name);

		Network::PInetAddr addr = sessionptr->getAddr();
		PGRFExitsResPkt *pkt = new PGRFExitsResPkt(e, name, exits);
		m_pComm->tcpSend(addr, pkt);
	}
	else
	{
	}
	
}

bool SessionManager::lock(unsigned int sessionid, unsigned int handleid, int mode, int trylck,  int e)
{
	SessionDataPtr sessionptr;
	HandleDataPtr handleptr;
	NodeDataPtr nodeptr;
	string normal_name;
	bool trylock = false;
	if (getSession(sessionid, sessionptr))
	{
		//get the handle of name
		//normalize(name, normal_name);
		Network::PInetAddr addr = sessionptr->getAddr();
		PGRFLockResPkt *pkt;
		
		if(getHandle(handleid, handleptr))
		{
			normal_name = handleptr->getNodeName();
			if (getNode(normal_name, nodeptr))
			{
				if (mode != handleptr->getLockedState())
				{	
					if(LOCK_STATUS_GRANTED == nodeptr->lockNode(handleptr, mode, trylock))
					{
						handleptr->setLockedState(mode);	
						pkt = new PGRFLockResPkt(e, handleid, LOCK_STATUS_GRANTED, ++m_lockGenerater);
						
					}
					else 
					{
						pkt = new PGRFLockResPkt(e, handleid, LOCK_STATUS_CONFLICTED);
					}
					m_pComm->tcpSend(addr, pkt);
				}
				//RESPONE OK TO SESSION;
			}
		}				
		//getNode(normal_name, nodeptr);
		//respone ok to the session;
	}
	//respone error to the session;
	return true;
}

bool SessionManager::tryLock(unsigned int sessionid, unsigned int handleid, int mode, int e)
{
	SessionDataPtr sessionptr;
	HandleDataPtr handleptr;
	NodeDataPtr nodeptr;
	string normal_name;
	bool trylock = true;
	if (getSession(sessionid, sessionptr))
	{
		//get the handle of name
		//normalize(name, normal_name);
		
		if(getHandle(handleid, handleptr))
		{
			normal_name = handleptr->getNodeName();
			if (getNode(normal_name, nodeptr))
			{
				if (mode != handleptr->getLockedState())
				{	
					if(LOCK_STATUS_GRANTED == nodeptr->lockNode(handleptr, mode, trylock))
					{
						handleptr->setLockedState(mode);	
					}
				}
				//RESPONE OK TO SESsiON;
			}
		}				
		//getNode(normal_name, nodeptr);
		//respone ok to the session;
	}
	//respone error to the session;
	return true;
}

bool SessionManager::unlock(unsigned int sessionid, unsigned int handleid, int e)
{
	SessionDataPtr sessionptr;
	HandleDataPtr handleptr;
	NodeDataPtr nodeptr;
	string normal_name;
	bool trylock = true;;
	if (getSession(sessionid, sessionptr))
	{		
		Network::PInetAddr addr = sessionptr->getAddr();
		if(getHandle(handleid, handleptr))
		{
			normal_name = handleptr->getNodeName();
			if (getNode(normal_name, nodeptr))
			{
				nodeptr->unlockNode(handleptr);
				handleptr->setLockedState(UNLOCKED_MODE);
				nodeptr->rmHandle(handleptr);
				notify_watcher_event(WatcherEventType::LOCK_RELEASED,normal_name);
				nodeptr->addHandle(handleptr);			
				//PGRFDirAttrGetResPkt *pkt = new PGRFDirAttrGetResPkt(e, dirname, cnt, attrs);
				//m_transmiter->reliableSend(addr, pkt);
			}
		}				
		//getNode(normal_name, nodeptr);
		//respone ok to the session;
	}
	//respone error to the session;
	return true;
}

void SessionManager::notify_watcher_event(U32 event_type, string node_name)
{
	SessionDataPtr sessionptr;
	HandleDataPtr handleptr;
	NodeDataPtr nodeptr;
	getNode(node_name, nodeptr);
	if(nodeptr == NULL) return;

	vector<PInetAddr> addrmap;
	HandleMap::const_iterator it = nodeptr->m_handleMap.begin();

	for( ; it != nodeptr->m_handleMap.end(); it ++)
	{
		handleptr = it->second;
		getSession(handleptr->getSessonID(), sessionptr);
		PGRFWatcherEventPkt *pkt = new PGRFWatcherEventPkt(event_type,node_name);
		cout<< "watcher broadcast" << event_type << "  node name:" << node_name << endl;
		m_pComm->tcpSend(sessionptr->getAddr(), pkt);
		//addrmap.push_back(sessionptr->getAddr());
	}
	/*
	vector<PInetAddr>::const_iterator addr_it = addrmap.begin();
	for( ; addr_it != addrmap.end(); addr_it ++)
	{
		
	}
	*/
}

void SessionManager::timely_renew()
{
	cout << "timely renew!! expired session counter:" << m_expired_session_counter << "db run time " << m_db_run_time << "write func run time " << func_run_time << endl;
	SessionDataPtr session;
	NodeDataPtr nodeptr;
	HandleDataPtr handleptr;
	map<U32,WatcherEvent> watcherQueue;
	U32 watcherCounter = 0;
	SessionMap::iterator sit = m_sessionMap.begin();
	S64 current_time = PUtils::localMilliSeconds();//may need some modification
	cout << "operations total: " << m_operations << " operations per second:" << m_operations/(double)(PUtils::localSeconds() - m_begin_time) << endl;
	m_operations  = 0 ;
	m_begin_time = PUtils::localSeconds();
	while (sit != m_sessionMap.end())
	{
		session = sit->second;
		if(!session) 
		{
			cout << "error invalidated session  handle!!" << endl;
			m_sessionMap.erase((sit++)->first);
			continue;
		}

		if(session->diff_time(current_time))
		{
			if (session->get_jeo_time() <= 0)
			{
				cout << "session expired! session id: " << session->getID() << endl;
				m_expired_session_counter ++;
				vector<unsigned int> handles;
				session->getHandles(handles);
				vector<unsigned int> ::iterator hit = handles.begin();
				while (hit != handles.end())
				{
					
					getHandle(* hit,handleptr);
					string name = handleptr->getNodeName();
					getNode(name, nodeptr);
					bool flag = nodeptr->unlockNode(handleptr);
					if(flag)
					{
						watcherCounter ++;
						WatcherEvent e(WatcherEventType::LOCK_RELEASED,name);
						watcherQueue.insert(pair<U32, WatcherEvent>(watcherCounter,e));
					}
					nodeptr->rmHandle(handleptr);
					m_handleMap.erase(*hit);
					delete handleptr;
					hit++;
				}
				//SessionMap::iterator hit = sit;
				m_sessionMap.erase((sit++)->first);
				continue;
			}
			else
			{
				session->dec_jeo_time();
			}
		}
		sit ++;
	}
	map<U32,WatcherEvent>::iterator it = watcherQueue.begin();
	for( ; it != watcherQueue.end(); it ++)
	{
		notify_watcher_event(it->second.m_watcherType, it->second.m_nodeName);
	}
}

void SessionManager::keepalive(unsigned int id, Network::PInetAddr addr)
{
	SessionDataPtr session;
	if (0 == id)
	{
		cout << "generation client session for " << addr.toString() << endl;
		session = new SessionData(addr, m_lease_interval*1.5);
		cout << "create session!!" << endl;
		id = createSession(session);

	}
	else if (getSession(id, session))
	{
		cout << "validate session!!" << endl;
		session->reset_jeo_time();
		session->renew_expire_time();
	}
	else 
	        return;//send session_expired

    PGRFKeepAliveResPkt *pkt = new PGRFKeepAliveResPkt(id);
	m_pComm->tcpSend(addr, pkt);
	//m_pComm->tcpSend(session->getAddr(), pkt);
}

void SessionManager::setAppTerminate()
{
	m_pComm->setAppTerminate();
}
