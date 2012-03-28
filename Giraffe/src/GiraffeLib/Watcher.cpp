#include "Base/GHeader.h"
#include "Watcher.h"


Watcher::Watcher(U32 type, string node)
{
	m_watcherType = type;
	m_nodeName = node;
}


Watcher::~Watcher(void)
{
}

void Watcher::work(void)
{
	cout << "watching, watching, watch!!!" << endl;
}

bool Watcher::operator==(const Watcher w)
{
	if(this->m_watcherType == w.m_watcherType && this->m_nodeName == w.m_nodeName)
		return true;
	return false;
}

bool Watcher::isEventWatching(const WatcherEvent we)
{
	if(this->m_watcherType == we.m_watcherType && this->m_nodeName == we.m_nodeName)
		return true;
	return false;
}
