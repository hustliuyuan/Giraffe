#ifndef __GIRAFFE_CLIENT_WATCHER_MANAGER_H__
#define __GIRAFFE_CLIENT_WATCHER_MANAGER_H__
#include <boost/threadpool.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include "WatcherEvent.h"
#include "Watcher.h"

class WatcherManager
{
public:
	WatcherManager(U32 size);
	~WatcherManager(void);

	static WatcherManager* getInstance();
	void start();
	void stop();

	WatcherEvent popEvent();
	void pushEvent(WatcherEvent we);

	Watcher* getWatcher(WatcherEvent we);
	bool insertWatcher(Watcher* w);

	void handleWatcherEvent();


public:
	static WatcherManager* m_pInstance;
	bool m_bIsStop;
	std::deque<WatcherEvent> m_eventQueue;
	boost::mutex m_mutexForEvents;
	boost::condition m_condForEvents;
	U32 m_eventCounter;

	std::vector<Watcher*> m_watcherQueue;
	boost::mutex m_mutexForWatchers;
	//boost::condition m_condForWatchers;

	U32 m_threadPoolSize;
	boost::threadpool::pool           m_oThreadpool;
};

#endif //__GIRAFFE_CLIENT_WATCHER_MANAGER_H__