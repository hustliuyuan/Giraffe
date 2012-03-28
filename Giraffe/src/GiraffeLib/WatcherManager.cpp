#include "Base/GHeader.h"
#include "WatcherManager.h"
#include "WatcherThreadFunc.h"

WatcherManager* WatcherManager::m_pInstance = NULL;

WatcherManager::WatcherManager(U32 size)
{
	m_eventCounter = 0;
	m_threadPoolSize = size;
	m_bIsStop = false;
}

WatcherManager* WatcherManager::getInstance()
{
	if(!m_pInstance)
	{
		m_pInstance = new WatcherManager(6);
	}
	return m_pInstance;
}

WatcherManager::~WatcherManager(void)
{
}

void WatcherManager::start()
{
	this->m_oThreadpool.size_controller().resize(m_threadPoolSize);
	
	for(unsigned int i = 0;i< m_threadPoolSize;i++)
	{   
		WatcherThreadFunc wf(this,i);
		this->m_oThreadpool.schedule(wf);
	}
}

void WatcherManager::stop()
{
	m_bIsStop = true;
}

WatcherEvent WatcherManager::popEvent()
{
	boost::mutex::scoped_lock lc(m_mutexForEvents);
	while(m_eventCounter <= 0)
	{
		m_condForEvents.wait(lc);
	}
	WatcherEvent we = (WatcherEvent)m_eventQueue.front();
	m_eventQueue.pop_front();
	m_eventCounter --;
	return we;
}

void WatcherManager::pushEvent(WatcherEvent we)
{
	cout << "new watcher event" << endl;
	boost::mutex::scoped_lock(m_mutexForEvents);
	m_eventQueue.push_back(we);
	if(m_eventCounter == 0)
	{
		m_eventCounter ++;
		m_condForEvents.notify_one();
	}
	
}

Watcher* WatcherManager::getWatcher(WatcherEvent we)
{
	Watcher* w = NULL;
	boost::mutex::scoped_lock lc(m_mutexForWatchers);
	std::vector<Watcher*>::iterator it = m_watcherQueue.begin();
	for( ; it != m_watcherQueue.end(); it ++)
	{
		if((*it)->isEventWatching(we))
		{
			w = *it;
			break;
		}
	}
	return w;
}

bool WatcherManager::insertWatcher(Watcher* w)
{
	if(!w)
		return false;
	boost::mutex::scoped_lock lc(m_mutexForWatchers);
	m_watcherQueue.push_back(w);
	return true;
}

void WatcherManager::handleWatcherEvent()
{
	while(!m_bIsStop)
	{
		WatcherEvent we = popEvent();
		Watcher* w = getWatcher(we);
		cout << "get event" << endl;
		if(w == NULL)
		{
			cout << "w == NULL " << endl;
			continue;
		}
		else
		{
			cout << "watcher provoke! " << endl;
			w->work();
		}
	}
}
