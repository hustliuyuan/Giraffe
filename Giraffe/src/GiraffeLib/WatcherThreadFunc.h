#ifndef __GIRAFFE_CLIENT_WATCHER_THREAD_FUNCTION_H__
#define __GIRAFFE_CLIENT_WATCHER_THREAD_FUNCTION_H__
#include "WatcherManager.h"
class WatcherThreadFunc
{
public:
	WatcherThreadFunc(WatcherManager* wm, U32 id);
	~WatcherThreadFunc(void);

	void operator ()();

public:
	WatcherManager* m_watcherMgr;
	U32 m_threadId;

};
#endif //__GIRAFFE_CLIENT_WATCHER_THREAD_FUNCTION_H__
