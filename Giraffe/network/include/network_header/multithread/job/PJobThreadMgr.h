#ifndef __GJOB_THREAD_MANAGER_H__
#define __GJOB_THREAD_MANAGER_H__


#include "PJobQueue.h"
#include "IJobHandler.h"
//#include "db/OraDBConnectPool.h"
#include <boost/threadpool.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

using namespace boost::threadpool;

class NETWORK_EXPORT PJobThreadMgr: public IJobResultContainer
{
public:

	PJobThreadMgr(U32 threadNumber);
	virtual ~PJobThreadMgr();

	void	 registerJobHandler( IJobHandler *  handler);

	 IJob *	getAndRemoveJob( ); 
	 void    addJob(IJob *  job);
     U32     getJobCount();

	 void    addJobResult(IJob * resultJob);
	 IJob *	getAndRemoveJobResult();   
	 U32     getJobResultCount();

	S32             getThreadNumber()const;

	void			start();
	void			threadWork(U32 id);
	void			stop();

private:

	boost::mutex				jobqueue_mutex;
	boost::mutex				jobResultQueue_mutex;
	boost::condition_variable	cond;

	boost::threadpool::pool           m_oThreadpool;//

	S32							m_iThreadNo;
	PJobQueue				    m_oJobQueue;
	PJobQueue				    m_oJobResultQueue;
	B8							m_bIsStop;

	//OraDBConnectPool*			m_pConnPool;
	IJobHandler		*			m_pJobHandler;
};

#endif // __GJOB_THREAD_MANAGER_H__