#include "common/Header.h"
#include "multithread/job/PJobThreadFunc.h"
#include "multithread/job/PJobThreadMgr.h"

PJobThreadFunc::PJobThreadFunc( PJobThreadMgr * manger, U32 id)
{
	m_oJobManager = manger;
	this->m_iThreadID = id;
}
void PJobThreadFunc::operator()()
{
	m_oJobManager->threadWork(m_iThreadID);
}

