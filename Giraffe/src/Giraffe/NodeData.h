#ifndef __GIRAFFE_NODEDATA_H__
#define __GIRAFFE_NODEDATA_H__

#include <set>
#include <string>
#include <map>
#include <queue>
#include "HandleData.h"
#include "Base/GCommon.h"

using std::set;
using std::string;
using std::map;
using std::queue;

struct LockRequestHandle 
{
	int request_handle;
	int mode;
};

class NodeData
{
public:
	NodeData(const string filename):m_exclusiveHandleId(0),m_fileName(filename),
		m_mode(UNLOCKED_MODE)
	{}

	~NodeData()
	{}
		
	//the following algorithm is needed to valid strictly.
	int lockNode(const HandleDataPtr &handle, int lock_state, bool trylock)
	{
		int pre_lock_state = handle->getLockedState();
		unsigned int id = handle->getHandleID();

		if(getMode() == UNLOCKED_MODE)
		{
			setMode(lock_state);

			if(lock_state == LOCK_EXCLUSIVE_MODE)
				m_exclusiveHandleId = id;
			else
				m_shareHandles.insert(id);

			return LOCK_STATUS_GRANTED;
		}
		else if(getMode() == LOCK_EXCLUSIVE_MODE)
		{
			if(lock_state == LOCK_EXCLUSIVE_MODE && m_exclusiveHandleId == id)
				return LOCK_STATUS_GRANTED;
			return LOCK_STATUS_CONFLICTED;
		}
		else
		{
			if(lock_state == LOCK_SHARE_MODE)
			{
				m_shareHandles.insert(id);
				return LOCK_STATUS_GRANTED;
			}

			return LOCK_STATUS_CONFLICTED;
				
		}


		/*
		//handle has locked the node
		if (id == m_exclusive_handle_id && lock_state == SHARED_LOCKED_MODE)
		{
			//addHandle(handle);
			setMode(lock_state);
			return LOCK_STATUS_GRANTED;
		}
		else if(exitsHandle(handle) && lock_state == EXCLUSIVE_LOCKED_MODE)
		{
			//rmHandle(handle);
			if (m_share_handles.empty())
			{
				m_exclusive_handle_id = id;
				setMode(lock_state);
				return LOCK_STATUS_GRANTED;
			}
			else
			{
				//addHandle(handle);
				//if(trylock)  pushQue(id, lock_state);
				return LOCK_STATUS_CONFLICTED;
			}
		}
			
		//handle hasn't locked the node
		if(EXCLUSIVE_LOCKED_MODE == getMode())
		{
			if(trylock)  pushQue(id, lock_state);
			return LOCK_STATUS_CONFLICT;
		}
			
		if(SHARED_LOCKED_MODE == getMode())
		{
			if(EXCLUSIVE_LOCKED_MODE == lock_state)
			{
				if(trylock)  pushQue(id, lock_state);
				return LOCK_STATUS_CONFLICTED;
			}
			else
			{
				addHandle(handle);
			}

		}
		if(UNLOCKED_MODE == getMode())
		{
			setMode(lock_state);
			if(EXCLUSIVE_LOCKED_MODE == lock_state) 
				m_exclusive_handle_id = id;
			else 
				addHandle(handle);
		}*/
		//return LOCK_STATUS_GRANTED;
	}

	bool unlockNode(const HandleDataPtr &handle)
	{
		int locked_state = handle->getLockedState();
		if (LOCK_EXCLUSIVE_MODE == locked_state)
		{
			setMode(UNLOCKED_MODE);
			m_exclusiveHandleId = 0;
			return true;
		}
		else if (LOCK_SHARE_MODE == locked_state)
		{
			m_shareHandles.erase(handle->getHandleID());
			//rmHandle(handle);
			if (m_shareHandles.empty())
			{
				setMode(UNLOCKED_MODE);
			}
				
		}
		return false;
	}

	bool addHandle(const HandleDataPtr &handle)
	{
		unsigned int id = handle->getHandleID();
		m_handleMap[id] = handle;
		return true;
	}

	bool rmHandle(const HandleDataPtr &handle)//should update the lock of node
	{
		unsigned int id = handle->getHandleID();
		//handle = m_handlemap[id];
		m_handleMap.erase(id);
		return true;
	}

	bool exitsHandle(const HandleDataPtr &handle)
	{
		unsigned int id = handle->getHandleID();
		if (m_handleMap.find(id) != m_handleMap.end())
		{
			return true;
		}

		return false;
	}

	inline int getMode()
	{
		return m_mode;
	}

	inline void setMode(int mode)
	{
		m_mode = mode;
	}

	bool pushQue(const unsigned int id, const int mode)
	{
		struct LockRequestHandle lsHandle;
		lsHandle.mode = mode;
		lsHandle.request_handle = id;
		m_waitingQue.push(lsHandle);
		return true;
	}

	bool headQue(unsigned int &id, int &mode)
	{
		if (m_waitingQue.empty() == true)
		{
			return false;
		}
			
		struct LockRequestHandle lsHandle;
		lsHandle = m_waitingQue.front();
		id = lsHandle.request_handle;
		mode = lsHandle.mode;
		return true;
	}

	bool popQue(unsigned int &id, int &mode)
	{
		bool flag = headQue(id,mode);
		if(flag) m_waitingQue.pop();
		return flag;
	}

	void updateLock()//the lock should be update when a node is removed from the queue
	{

	}
		
public:
	string m_fileName;
	unsigned int m_exclusiveHandleId;
	set<unsigned int> m_shareHandles;

	map<unsigned int, HandleDataPtr> m_handleMap;
	queue<struct LockRequestHandle > m_waitingQue;
	int m_mode;
				
};
typedef NodeData* NodeDataPtr;

#endif //__GIRAFFE_NODEDATA_H__
