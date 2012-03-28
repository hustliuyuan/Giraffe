#ifndef __GIRAFFE_CLIENT_FILE_CACHE__
#define __GIRAFFE_CLIENT_FILE_CACHE__

class GCLFileNode
{
public:
	GCLFileNode(string nodename, U32 handle, bool isDir)
	{
		m_nodename = nodename;
		m_handle = handle;
		m_isDir = isDir;
		m_isValid = false;
		m_isLock = false;
	}

	void setData(string content)
	{
		m_nodeContent = content;
		m_isValid = true;
	}

	bool getData(string &content)
	{
		if(m_isValid)
		{
			content = m_nodeContent;
			return true;
		}

		return false;
	}

	void inValidate()
	{
		m_isValid = false;
	}

	void setLock(U32 lockid)
	{
		m_filelock = lockid;
		m_isLock = true;
	}

	bool getLock(U32 &lock)
	{
		if(!m_isLock)
			return false;
		lock = m_filelock;
		return true;
	}

	/*
	void operator =(GCLFileNode node)
	{

	}*/


public:
	string m_nodename;
	U32 m_handle;
	bool m_isLock;
	U32 m_filelock;
	bool m_isDir;
	string m_nodeContent;
	bool m_isValid;
};

class GCLFileCache
{
public:
	GCLFileCache(void);
	~GCLFileCache(void);
	void insertFileNode(string nodename, GCLFileNode node);
	bool getFileNode(string nodename, GCLFileNode& node);
	bool removeFileNode(string nodename, GCLFileNode& node);
private:
	map<string, GCLFileNode> m_fileMap;
	boost::mutex m_fileMapMutex;
};
#endif //__GIRAFFE_CLIENT_FILE_CACHE__
