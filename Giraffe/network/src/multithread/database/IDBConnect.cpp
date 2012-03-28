#include "common/Header.h"
#include "multithread/database/IDBConnect.h"

IDBConnect::IDBConnect()
{
	m_oUsername = "";
	m_oPassword = "";
	m_oDbname   = "";
	m_oSiteName = "";
}

U32 IDBConnect::getConnectType()
{
	return m_iConnectType;
}

IDBConnect::~IDBConnect()
{
	
}