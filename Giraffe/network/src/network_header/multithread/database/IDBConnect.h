#ifndef __IDB_CONNECT_H__
#define __IDB_CONNECT_H__

#include <string>
#include "common/Header.h"

using namespace std;


class  NETWORK_EXPORT IDBConnect
{
public:
	IDBConnect();
	virtual ~IDBConnect();

	/*virtual bool init(const string username,const string password,const string dbname,const string sitename = "") = 0;*/

	virtual U32  getConnectType();

	string		m_oUsername;
	string		m_oPassword;
	string		m_oDbname;
	string		m_oSiteName;
	U32			m_iConnectType;
};

#endif //__IDB_CONNECT_H__