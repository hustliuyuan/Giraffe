#ifndef __GIRAFFE_SESSION_MANAGER_H__
#define __GIRAFFE_SESSION_MANAGER_H__

#include "SessionData.h"
#include "HandleData.h"
#include "NodeData.h"
#include "BerkelyFileSystem.h"
#include "logic/GMSComm.h"
#include "logic/GPaxosComm.h"

class SessionManager
{
	public:
		SessionManager();
		~SessionManager();

		void setAppTerminate();

		void open(unsigned int sessionid, const string name, int flags, AttrMap attrs, int e);
		void del(unsigned int sessionid, unsigned int handleid, int e);
		void mkDir(unsigned int sessionid, const string name, int e);
		void write(unsigned int sessionid, const string name, unsigned int handleid, AttrMap &attrs, int e, bool dir);
		void read(unsigned int sessionid, const string name, unsigned int handleid, int cnt,  AttrKeyMap &key, int e, bool isdir);
		void exits(unsigned int sessionid, string name, int e);
		void getlist(unsigned int sessionid, const string &dirname, int e);
		void close(unsigned int sessionid, unsigned int handleid, unsigned e);

		void notify_watcher_event(U32 event_type, string node_name);

		bool lock(unsigned int sessionid, unsigned int handleid, int mode, int trylck, int e);
		bool tryLock(unsigned int sessionid, unsigned int handleid,int mode, int e);
		bool unlock(unsigned int sessionid, unsigned int handleid, int e);
	 	void keepalive(unsigned int id, Network::PInetAddr addr);
		void timely_renew();
		void check_state();
		void get_leader_location(U32 id,Network::PInetAddr addrFom);

		BerkelyDBFileSystem* getFileSystem()
		{
			return m_fs;
		}

	private:
		void normalize(const string scr, string &normal)
		{
			if(scr[0] != '/')
			{
				normal = '/';
			}
			normal += scr;
		}


		void renewSession(unsigned int id, SessionDataPtr &sessionptr)
		{
			getSession(id, sessionptr);
			sessionptr->reset_jeo_time();
			sessionptr->renew_expire_time();
		}

		bool getSession(unsigned int id, SessionDataPtr &sessionptr)
		{
			bool flag = false;
			//lock(m_sessionMap)
			SessionMap ::iterator it = m_sessionMap.find(id);
			if (m_sessionMap.end() != it)
			{
				sessionptr = it->second;
				flag = true;
			}
			//unlock(m_sessionMap)
			return flag;
		}

		unsigned int  createSession(SessionDataPtr &sessionptr)
		{
			//Lock(sessionMap);
			unsigned int id = ++ m_sessionCurrentID;//可能存在问题，m_scid可能溢出
			cout << "session id : " << id << endl;
			sessionptr->renew_expire_time();
			sessionptr->setID(id);
			cout << "add session into map" << endl;
			m_sessionMap.insert(make_pair(id, sessionptr));
			//unlock(sessionMap);			
			return id;
		}

		bool delSession(unsigned int id, SessionDataPtr &sessionptr)
		{
			bool flag = false;
			//Lock(sessionMap);
			SessionMap ::iterator it = m_sessionMap.find(id);
			if (m_sessionMap.end() != it)
			{
				sessionptr = it->second;
				m_sessionMap.erase(id);
				flag = true;
			}
			//unlock(sessionMap);
			return flag;
		}

		bool getHandle(unsigned int id, HandleDataPtr &handleptr)
		{
			bool flag = false;
			//lock(m_handleMap)
			HandleMap::iterator it = m_handleMap.find(id);
			
			if(m_handleMap.end() != it)
			{
				handleptr = it->second;
				flag = true;
			}
			//unlock(m_handleMap)
			return flag;
		}

		unsigned int createHandle(HandleDataPtr &handleptr)
		{
			int id = ++ m_handleCurrentID;
			//lock(m_handleMap)
			m_handleMap.insert(make_pair(id, handleptr));
			handleptr->setHandleID(id);
			//unlock(m_handleMap)
			return id;
		}

		bool delHandle(unsigned int id, HandleDataPtr &handleptr)
		{
			bool flag = false;
			//lock(m_handleMap)
			HandleMap ::iterator it = m_handleMap.find(id);
			if(m_handleMap.end() != it)
			{
				handleptr = it->second;
				m_handleMap.erase(it);
				flag =true;
			}
			//unlock(m_handleMap)
			return flag;
		}

		bool getNode(const string fname, NodeDataPtr &nodeptr)
		{
			bool flag = false;
			//lock(m_nodeMap)
			NodeMap::iterator it = m_nodeMap.find(fname);
			if(m_nodeMap.end() != it)
			{
				nodeptr = it->second;
				flag = true;
			}
			//unlock(m_nodeMap)
			return flag;
		}

		bool createNode(const string fname, NodeDataPtr &nodeptr)
		{
			bool flag = false;
			m_nodeMap.insert(make_pair(fname, nodeptr));
			flag = true;
		
			return flag;
		}

		bool delNode(const string fname, NodeDataPtr &nodeptr)
		{
			bool flag = false;
			//lock(m_nodeMap)
			NodeMap::iterator it = m_nodeMap.find(fname);
			if(m_nodeMap.end() != it)
			{
				nodeptr = it->second;
				m_nodeMap.erase(it);
				flag = true;
			}
			//unlock(m_nodeMap)
			return flag;
		}

		typedef map<unsigned int, SessionDataPtr> SessionMap;
		typedef map<unsigned int, HandleDataPtr> HandleMap;
		typedef map<string, NodeDataPtr> NodeMap;
		typedef vector<unsigned int> HandleVector;

		SessionMap m_sessionMap;
		unsigned int m_sessionCurrentID;

		U32			m_expired_session_counter;

		HandleMap m_handleMap;
		unsigned int m_handleCurrentID;

		unsigned int m_lckCurrentGen;

		NodeMap m_nodeMap;

		string m_base_dir;
		S64 m_curTime;
		U32 m_lease_interval;
		BerkelyDBFileSystem *m_fs;
		GMSComm * m_pComm;
		U32 m_lockGenerater;
		S64 m_db_run_time;
		S64 func_run_time;
		S64 m_operations;
		U32 m_begin_time;
};
	
#endif //__GIRAFFE_SESSION_MANAGER_H__
