#include "Base/GHeader.h"
#include "GCLFileCache.h"


GCLFileCache::GCLFileCache(void)
{
}


GCLFileCache::~GCLFileCache(void)
{
}

void GCLFileCache::insertFileNode(string nodename, GCLFileNode node)
{
	boost::mutex::scoped_lock lck(m_fileMapMutex);
	map<string, GCLFileNode>::iterator it = m_fileMap.find(nodename);
	if( it == m_fileMap.end())
	{
		cout << "insert node to the cache!" << endl;
		m_fileMap.insert(pair<string, GCLFileNode>(nodename, node));
		cout << "inserted node to cache!" << endl;
	}	
	else
	{
		it->second = node;
	}
}

bool GCLFileCache::getFileNode(string nodename, GCLFileNode& node)
{
	boost::mutex::scoped_lock lck(m_fileMapMutex);
	map<string, GCLFileNode>::iterator it = m_fileMap.find(nodename);
	if( it == m_fileMap.end())
	{
		return false;
	}	
	else
	{
		node = it->second;
		return true;
	}
}

bool GCLFileCache::removeFileNode(string nodename, GCLFileNode& node)
{
	boost::mutex::scoped_lock lck(m_fileMapMutex);
	map<string, GCLFileNode>::iterator it = m_fileMap.find(nodename);
	if( it == m_fileMap.end())
	{
		return false;
	}	
	else
	{
		node = it->second;
		m_fileMap.erase(it);
		return true;
	}
}
