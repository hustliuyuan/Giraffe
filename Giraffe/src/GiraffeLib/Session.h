#ifndef __GIRAFFE_CLIENT_SESSION_H__
#define __GIRAFFE_CLIENT_SESSION_H__

#include<boost/date_time/posix_time/posix_time.hpp>
#include "logic/GCLComm.h"
#include "GCLFileCache.h"
#include "WatcherManager.h"
#include "Watcher.h"
#include "CallbackEvent.h"

enum SESSIONSTATE
{
	CONNECTING,
	CONNECTED,
	JEOPARDY,
	EXPIRED
};

class SessionState
{

};

class ConditionMutex
{
public:
	ConditionMutex()
	{
		m_pkt = NULL;
		event = NULL;
		trigger = false;
	}

	boost::mutex m_mutex;
	boost::condition_variable m_condition;
	const Network::PacketINFO* m_pkt;
	CallbackEvent *	event;
	bool trigger;
};

class Conditioner
{
public:
	Conditioner()
	{
		m_index = 0;
	}

	U32 addConditioner()
	{
		U32  id;
		{
			boost::mutex::scoped_lock lock(index_mutex);
			id = ++ m_index;
		}

		boost::mutex::scoped_lock lock(contain_mutex);
		ConditionMutex* cm = new ConditionMutex();
		lckContainer.insert(pair<U32,ConditionMutex*>(id, cm));

		return id;
	}

	void callConditioner(U32 id, CallbackEvent *event)
	{
		cout << "Calling the conditioner!!" << endl;
		ConditionMutex* cm;
		{
			boost::mutex::scoped_lock lock(contain_mutex);
			map<U32, ConditionMutex*>::iterator it = lckContainer.find(id);
			cm = it->second;
		    cout << "get condition" << endl;
			if(!cm) return;
			boost::mutex::scoped_lock mlock(cm->m_mutex);
			cout << "lock mutex" << endl;
			cm->event = event;//there maybe some problem
			cm->trigger = true;
			cm->m_condition.notify_one();
		}
		cout << "calling back" << endl;
	}

	void callConditioner(U32 id, const Network::PacketINFO* pktInfo)
	{
		cout << "Calling the conditioner!!" << endl;
		ConditionMutex* cm;
		{
			boost::mutex::scoped_lock lock(contain_mutex);
			map<U32, ConditionMutex*>::iterator it = lckContainer.find(id);
			if (it == lckContainer.end()) return;
			cm = it->second;
		    cout << "get condition" << endl;
			if(!cm) return;
			boost::mutex::scoped_lock mlock(cm->m_mutex);
			cout << "lock mutex" << endl;
			cm->m_pkt = pktInfo;//there maybe some problem
			cm->m_condition.notify_one();
		}
		cout << "calling back" << endl;
	}

	const CallbackEvent* waitingForConditioner(U32 id)
	{
		ConditionMutex* cm;
		{
			boost::mutex::scoped_lock lock(contain_mutex);
			map<U32, ConditionMutex*>::iterator it = lckContainer.find(id);
			cm = it->second;
		}
		
		if(!cm) return NULL;
		boost::mutex::scoped_lock lock(cm->m_mutex);
		bool flag =false;
		if(!cm->trigger)
		{
			flag = cm->m_condition.timed_wait(lock,boost::get_system_time() + boost::posix_time::seconds(3));
		}

		if (flag)
		{
			cout << "get respone!!" << endl;
			return cm->event;
		}
		else
		{
			cout << "error: wait for respone timeout!!" << endl;
			return NULL;
		}
		
	}
	/*
	const Network::PacketINFO* waitingForConditioner(U32 id)
	{
		ConditionMutex* cm;
		{
			boost::mutex::scoped_lock lock(contain_mutex);
			map<U32, ConditionMutex*>::iterator it = lckContainer.find(id);
			cm = it->second;
		}
		
		if(!cm) return NULL;
		boost::mutex::scoped_lock lock(*cm->m_mutex);
		if(cm->m_pkt == NULL)
		{
			cm->m_condition->wait(lock);
		}
		cout << "get respone!!" << endl;
		return cm->m_pkt;
	}*/


public:
	map<U32, ConditionMutex*> lckContainer;
	boost::mutex index_mutex;
	boost::mutex contain_mutex;
	U32 m_index;
};

class Session
{
public:
	Session();
	~Session(void);
	int open(string filename, U32 flags, string content,Watcher *w = NULL);
	int open(string filename,Watcher *w = NULL);
	void close(string filename);
	string getData(string filename,Watcher *w = NULL);
	bool setData(string filename, string content,Watcher *w = NULL);
	bool mkdir(string dir);
	vector<String> getChildren(string dirName);
	bool lock(string filename,Watcher *w = NULL);
	bool release(string filename);
	bool exits(string filename);
	bool connect();

	bool invalidateCache(string node);

	void getGiraffeCluster();
	bool reconnected();
	void keepalive(U32 id);


	void keepalive();
	void renewState(int id);
	void flush_file();
	void sessionTerminate();
	void getClusterLeaderLocation(Network::PInetAddr addr);
	void queryClusterLeaderLocation();

	U32 m_sessionId;
	boost::mutex m_session_mutex;
	boost::mutex m_leader_location_mutex;
	boost::condition m_leader_location_condition;

	
	boost::condition m_session_condition;
	map<string,U32> fileIndex;
	map<string,U32> filelock;
	GCLFileCache m_fileCache;

	S64 m_lease_interval;
	S64 m_lastkeepalive_time;
	bool m_connected;
	PInetAddr m_leader_addr;
	PInetAddr m_replica_addr;
	PInetAddr m_loc_addr;
	Conditioner *m_conditioner;
	GCLComm* m_pComm;
	WatcherManager* m_watcherMgr;
};
#endif //__GIRAFFE_CLIENT_SESSION_H__
