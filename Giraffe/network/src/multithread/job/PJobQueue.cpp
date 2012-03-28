#include "common/Header.h"
#include "multithread/job/PJobQueue.h"

PJobQueue::PJobQueue()
{

}

PJobQueue::~PJobQueue()
{
	JobQueue::iterator iter = m_oJobQueue.begin();
	while(iter != m_oJobQueue.end())
	{
		IJob* job = (*iter);
		//new 的job 则 需要delete
		delete job;
		++iter;
	}
}

void PJobQueue::addJob(IJob* inJob)
{
	m_oJobQueue.push_back(inJob);
}

IJob* PJobQueue::getJob()
{
	if(m_oJobQueue.size() == 0 )
		return NULL;
	IJob* job = m_oJobQueue.front();
	m_oJobQueue.pop_front();//从对列中删除该任务
	return job;
}

U32 PJobQueue::jobNumber()
{
	return m_oJobQueue.size();
}