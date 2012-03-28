#ifndef _GIRAFFE_CLIENT_CALLBACK_EVENT_H_
#define _GIRAFFE_CLIENT_CALLBACK_EVENT_H_
#include "Base/PPktDef.h"
#include "Base/GHeader.h"
namespace CallbackEventDef
{
	enum MajorType
	{
		DEFAULT,
	};

	enum SubType
	{
		MKDIR,
		LOCK,
		UNLOCK,
		READ,
		WRITE,
		OPEN,
		CLOSE,
		EXITS,
		READDIR,
	};
};

class CallbackEvent
{
public:
	CallbackEvent(U32 type, U32 subtype)
		:m_bType(type),m_bSubType(subtype)
	{}
	~CallbackEvent()
	{
	
	}
public:
	U32 m_bType;
	U32 m_bSubType;
};

class MkdirCallbackEvent:public CallbackEvent 
{
public:
	MkdirCallbackEvent(B8 ok, string dirname)
		:CallbackEvent(CallbackEventDef::DEFAULT, CallbackEventDef::MKDIR)
	{
		m_bOk = ok;
		m_sDirName = dirname;
	}
	~MkdirCallbackEvent(){}

public:
	B8 			m_bOk;
	string 		m_sDirName;
};
class ReadDirCallbackEvent:public CallbackEvent 
{
public:
	ReadDirCallbackEvent(U32 count, std::vector<string> files)
		:CallbackEvent(CallbackEventDef::DEFAULT, CallbackEventDef::READDIR)
	{
		m_iCount = count;
		m_oFiles = files;
	}
	~ReadDirCallbackEvent(){}

public:
	U32 			m_iCount;
	std::vector<string> 		m_oFiles;
};

class OpenCallbackEvent:public CallbackEvent 
{
public:
	OpenCallbackEvent(U32 handle)
		:CallbackEvent(CallbackEventDef::DEFAULT, CallbackEventDef::OPEN)
	{
		m_iHandle = handle;
	}
	~OpenCallbackEvent(){}
public:
	U32 m_iHandle;
};

class LockCallbackEvent:public CallbackEvent 
{
public:
	LockCallbackEvent(U32 handle, U32 lckState, U32 lckGen)
		:CallbackEvent(CallbackEventDef::DEFAULT, CallbackEventDef::LOCK)
	{
		m_iHandle = handle;
		m_iLockStatus = lckState;
		m_iLockGen = lckGen;
	}
	~LockCallbackEvent(){}
public:
	U32 m_iHandle;
	U32 m_iLockStatus;
	U32 m_iLockGen;

};

typedef std::map<String, String> AttrMap;
class WriteCallbackEvent:public CallbackEvent 
{
public:
	WriteCallbackEvent(B8 ok, U32 handle)
		:CallbackEvent(CallbackEventDef::DEFAULT, CallbackEventDef::WRITE)
	{
		m_bOk = ok;
		m_iHandle = handle;
	}
	~WriteCallbackEvent(){}
public:
	B8 		 	m_bOk;
	U32 		m_iHandle;
};

class ReadCallbackEvent:public CallbackEvent 
{
public:
	ReadCallbackEvent(U32 handle, U32 num, AttrMap attrs)
		:CallbackEvent(CallbackEventDef::DEFAULT, CallbackEventDef::READ)
	{
		m_iHandle = handle;
		m_iAttrNum = num;
		//m_oAttrs = attrs;
		m_sContent = attrs["content"];
	}
	~ReadCallbackEvent(){}
public:
	U32 	m_iHandle;
	U32 	m_iAttrNum;
	AttrMap m_oAttrs;
	string m_sContent;
	
};

class CloseCallbackEvent:public CallbackEvent 
{
public:
	CloseCallbackEvent(U32 handle, B8 ok)
		:CallbackEvent(CallbackEventDef::DEFAULT, CallbackEventDef::CLOSE)
	{
		m_iHandle = handle;
		m_bOK = ok;
	}
	~CloseCallbackEvent(){}
public:
	U32 	m_iHandle;
	B8 		m_bOK;
};

class ExitsCallbackEvent:public CallbackEvent 
{
public:
	ExitsCallbackEvent(string name, B8 ok)
		:CallbackEvent(CallbackEventDef::DEFAULT, CallbackEventDef::MKDIR)
	{
		m_sName = name;
		m_bExits = ok;
	}
	~ExitsCallbackEvent(){}
public:
	string 	m_sName;
	B8 		m_bExits;
};

#endif //_GIRAFFE_CLIENT_CALLBACK_EVENT_H_
