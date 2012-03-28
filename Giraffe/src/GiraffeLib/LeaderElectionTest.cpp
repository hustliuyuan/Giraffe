#include "Base/GHeader.h"
#include "Session.h"
#include "logic/GCLCommLogger.h"
#include "WatcherEventType.h"
#include "Watcher.h"

class LockReleaseWatcher:Watcher
{
public :
	LockReleaseWatcher(string node, Session *s)
		:Watcher(WatcherEventType::LOCK_RELEASED, node)
	{
		m_session = s;
	};

	virtual void work()
	{
		cout << "Leader Lock has been released!" << endl;
		bool flag = m_session->lock(m_nodeName);

		if(flag)
		{
			cout << "I Am Leader!!!!!!!" << endl;
			m_session->setData(m_nodeName,"192.168.0.1:43002");
			cout << " set data:  192.168.0.1:43002" << endl;
			PUtils::sleep(10000);
			m_session->setData(m_nodeName,"00.0.1:10000");
			cout << "set data: 0.0.0.1:10000" << endl;
		} 
		else
		{	
			string data = m_session->getData(m_nodeName);
			cout << "Current Leader is " << data << endl;
		}
	};

private:
	Session *m_session;
};

class NodeModifyWatcher:Watcher
{
public :
	NodeModifyWatcher(string node, Session *s)
		:Watcher(WatcherEventType::NODE_MODIFIED, node)
	{
		m_session = s;
	};

	virtual void work()
	{
		cout << "Node content has been modified!!" << endl;
		m_session->invalidateCache(m_nodeName);
		string data = m_session->getData(m_nodeName);
		cout << "Current Leader is " << data << endl;
	};

private:
	Session *m_session;

};

int  main_test1()
{
	GCLCommLogger::setCommConfig();
	Session *session = new Session();
	session->connect();

	cout <<  endl;
	cout <<  endl;
	cout << "************************************************************" << endl;
	cout << "************   LeaderElection Demo   **********************" << endl;
	cout << "************************************************************" << endl;
	cout <<  endl;
	cout <<  endl;

	session->open("/test/leader");
	bool flag = session->lock("/test/leader", (Watcher*)new LockReleaseWatcher("/test/leader",session));

	if(flag)
	{
		cout << "I Am Leader!!!!!!!" << endl;
		session->setData("/test/leader","192.168.0.1:43002");
		cout << " set data:  192.168.0.1:43002" << endl;
		PUtils::sleep(10000);
		session->setData("/test/leader","0.0.0.1:10000");
		cout << "set data : 0.0.0.1:10000" << endl;
	}
	else
	{	
		string data = session->getData("/test/leader", (Watcher*)new NodeModifyWatcher("/test/leader",session));
		cout << "Current Leader is " << data << endl;
	}
	/*
	PUtils::sleep(2000);
	session->mkdir("/test/");
	cout << "mkdir /test/" << endl;
	session->open("/test/master");
	cout << "open file /test/master" << endl;

	if(session->lock("/test/master"))
	{
		cout << "lock file /test/master" << endl;
		session->setData("/test/master", "ip:port=192.168.0.1");
		cout << "write config info to /test/master file" << endl;
	}
	else
	{
		PUtils::sleep(3000);
		session->getData("/test/master");
		cout << "read config info from /test/master file " << endl;
	}
	cout << "read dir GRFild" << endl;
	vector<string> children = session->getchildren("/test/");
	cout << "get dir GRFild" << endl;
	vector<string>::iterator it = children.begin();

	for( ; it < children.end(); it ++)
	{
		cout<< "GRFild: " << *it << endl;
	}
	session->close("/test/master");
 	
	session->flush_file();
	*/
	session->sessionTerminate();
	return 0;
}
