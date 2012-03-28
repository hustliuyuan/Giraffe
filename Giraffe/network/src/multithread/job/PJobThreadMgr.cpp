#include "common/Header.h"
#include "multithread/job/PJobThreadMgr.h"
#include "multithread/job/PJobThreadFunc.h"
//#include "job/GDBJobHandler.h"


PJobThreadMgr::PJobThreadMgr(/*OraDBConnectPool* connPool,*/U32 threadNumber)
{
	m_iThreadNo = threadNumber;
	this->m_bIsStop = false;
	//m_pConnPool = connPool;
}

void PJobThreadMgr::start()
{
	this->m_oThreadpool.size_controller().resize(m_iThreadNo);
	
	
	for(int i = 0;i< m_iThreadNo;i++)
	{   
		PJobThreadFunc gf(this,i);
		this->m_oThreadpool.schedule(gf);
	}
	LOG_INFO("PJobThreadFunc","start thread number:"<<m_iThreadNo);
}

S32 PJobThreadMgr::getThreadNumber()const
{
	return m_iThreadNo;
}

IJob * PJobThreadMgr::getAndRemoveJob( )
{
	
	boost::mutex::scoped_lock lock(jobqueue_mutex);
	
	while(m_oJobQueue.jobNumber() == 0 && m_bIsStop != true)
	{
		//cond.wait(lock);
		boost::xtime xt;
		boost::xtime_get(&xt, boost::TIME_UTC);
		xt.sec += 1;
		B8 result = cond.timed_wait(lock,xt);
	}

	IJob * jobInfo = m_oJobQueue.getJob();
	if(jobInfo == NULL)
	{
		LOG_DEBUG("PJobThreadMgr","getAndRemoveJob:"<<"get a null job");
		return jobInfo;
	}
	LOG_DEBUG("PJobThreadMgr","getAndRemoveJob:"<<jobInfo->toString());
	return jobInfo;
}


void  PJobThreadMgr::addJob(IJob *  job)
{
	boost::mutex::scoped_lock lock(jobqueue_mutex);
	m_oJobQueue.addJob(job);
	cond.notify_one();
	LOG_WARN("PJobThreadMgr","addJob: "<<job->toString());
}

void PJobThreadMgr::addJobResult(IJob *jobResult)
{
	boost::mutex::scoped_lock lock(jobResultQueue_mutex);
	m_oJobResultQueue.addJob(jobResult);
	LOG_DEBUG("PJobThreadMgr","addJobResult:"<<jobResult->toString());
}

IJob* PJobThreadMgr::getAndRemoveJobResult()
{
	boost::mutex::scoped_lock lock(jobResultQueue_mutex);
	IJob * jobResultInfo = m_oJobResultQueue.getJob();
	LOG_DEBUG("PJobThreadMgr","getAndRemoveJobResult:"<<jobResultInfo->toString());
	return jobResultInfo;
}


U32 PJobThreadMgr::getJobResultCount()
{
	return m_oJobResultQueue.jobNumber();
}

U32  PJobThreadMgr::getJobCount()
{
	return m_oJobQueue.jobNumber();
}

//TODO: need to rewrite , tu.
void PJobThreadMgr::threadWork(U32 id)
{
	//GDBJobHandler job_handler(m_pConnPool);
	while(! this->m_bIsStop)
	{		
		IJob * job = this->getAndRemoveJob();

		if( job == NULL)
			continue;

		m_pJobHandler->handleJob(job);
		//U32 job_count = m_pJobHandler->getResultSum();
		//if( job_count > 0)
		//{
		//	for(U32 i = 0 ;i< job_count;i++)
		//	{
		//		this->addJobResult(m_pJobHandler->getJobResult());
		//	}
		//}

//		delete job;   //多线程访问冲突，先注释。
	}

	LOG_DEBUG("PJobThreadMgr","PJobThreadFunc thread quit. this id : " << id); 
}

void	PJobThreadMgr::stop() 
{
	LOG_DEBUG("PJobThreadFunc","stop thread :");
	this->m_bIsStop = true;	
}

void	 PJobThreadMgr::registerJobHandler( IJobHandler *  handler)
{
	this->m_pJobHandler = handler;
	this->m_pJobHandler->setJobResultContainer(this);
}

PJobThreadMgr::~PJobThreadMgr()
{
	//if(m_pConnPool != NULL)
	//{
	//	//在这儿删除后，还有线程在运行，则会导致出现有的连接释放时程序奔溃
	//	//delete m_pConnPool;
	//	//m_pConnPool = NULL;
	//}
}
