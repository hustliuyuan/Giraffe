#ifndef __GMS_DB_PKTHANDLER_H__
#define __GMS_DB_PKTHANDLER_H__

#include "PJobThreadMgr.h"

class NETWORK_EXPORT PJobThreadFunc
{
public:
	PJobThreadFunc( PJobThreadMgr * manger, U32 id);
	void   operator()(); 

private:
	PJobThreadMgr*  m_oJobManager;


	U32					m_iThreadID;
};

#endif // __GMS_DB_PKTHANDLER_H__
