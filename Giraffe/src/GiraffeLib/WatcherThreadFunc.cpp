#include "Base/GHeader.h"
#include "WatcherThreadFunc.h"


WatcherThreadFunc::WatcherThreadFunc(WatcherManager* wm, U32 id)
{
	m_watcherMgr = wm;
	m_threadId = id;
}


WatcherThreadFunc::~WatcherThreadFunc(void)
{
}

void WatcherThreadFunc::operator()()
{
	m_watcherMgr->handleWatcherEvent();
}
