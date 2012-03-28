#ifndef __IJOB_HANDLER_H__
#define __IJOB_HANDLER_H__

#include "IJob.h"

class IJobResultContainer
{
public:
	virtual void    addJobResult(IJob * resultJob) = 0;
	virtual IJob *	getAndRemoveJobResult() = 0;   
	virtual U32     getJobResultCount() = 0;
};


class NETWORK_EXPORT IJobHandler
{
protected:
	IJobHandler();
public:
	virtual ~IJobHandler();
	virtual void handleJob(IJob* job) = 0;
	void			setJobResultContainer( IJobResultContainer *  container);

private:

	IJobResultContainer*  m_pJobResultContainer;
};

#endif//__IJOB_HANDLER_H__