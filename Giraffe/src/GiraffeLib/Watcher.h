#ifndef __GIRAFFE_CLIENT_WATCHER_H__
#define __GIRAFFE_CLIENT_WATCHER_H__

#include "Base/WatcherEvent.h"

class Watcher
{
public:
	Watcher(U32 m_type, string node);
	~Watcher(void);

	bool operator==(const Watcher w);
	
	bool isEventWatching(const WatcherEvent we);

	virtual void work() ;
public:
	U32 m_watcherType;
	string m_nodeName;
};

#endif //__GIRAFFE_CLIENT_WATCHER_H__

