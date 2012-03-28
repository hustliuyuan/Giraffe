#ifndef __GJOB_QUEUE_H__
#define __GJOB_QUEUE_H__

#include "IJob.h"

typedef std::deque<IJob*> JobQueue;

class NETWORK_EXPORT PJobQueue
{
public:
	PJobQueue();
	virtual ~PJobQueue();
	void			addJob(IJob*	inJob);

	IJob*			getJob();

	U32             jobNumber();

private:

	JobQueue m_oJobQueue;
};

#endif //__GJOB_QUEUE_H__
