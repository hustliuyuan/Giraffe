#ifndef __GIRAFFE_HANDLE_DATA_H__
#define __GIRAFFE_HANDLE_DATA_H__
#include <string>
#include "Base/GCommon.h"

using std::string;

class HandleData
{
public:
	HandleData(const int sessionId, const string node_name)
		:m_handleId(0), m_sessionId(sessionId), m_nodeName(node_name),m_lockStatus(2)
	{
	}

	~HandleData()
	{
	}

	inline int getSessonID()
	{
		return m_sessionId;
	}

	inline string getNodeName()
	{
		return m_nodeName;
	}

	inline void setLockedState(int lockedState)
	{
		m_lockStatus = lockedState;
	}

	inline int getLockedState()
	{
		return m_lockStatus;
	}

	inline int getHandleID()
	{
		return m_handleId;
	}

	inline void setHandleID(unsigned int id)
	{
		m_handleId = id;
	}

	inline void setMask(int mask)
	{
		m_mask = mask;
	}

	inline int getMask()
	{
		return m_mask;
	}

private:
	int m_handleId;
	int m_sessionId;
	string m_nodeName;
	int m_lockStatus;
	int m_mask;

};

typedef HandleData* HandleDataPtr;

#endif //__GIRAFFE_HANDLE_DATA_H__
