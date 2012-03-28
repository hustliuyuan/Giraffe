#ifndef __GIRAFFE_WATCHER_EVENT_H__
#define __GIRAFFE_WATCHER_EVENT_H__
#include "Base/WatcherEventType.h"
class WatcherEvent
{
public:
	WatcherEvent(U32 type, string name)
	{
		m_watcherType = type;
		m_nodeName = name;
	}

	~WatcherEvent()
	{
	
	}
public:
	U32 m_watcherType;
	string m_nodeName;
};

#endif //__GIRAFFE_WATCHER_EVENT_H__
