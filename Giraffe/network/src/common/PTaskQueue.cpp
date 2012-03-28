// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#include "common/Header.h"
#include "common/PTaskQueue.h"
#include "common/PTime.h"
#include "config/PSettings.h"



PTaskQueue::PTaskQueue(void)
{
	m_bIsStop = false;
	const PSettings & settings= PSettings::getInstance();
	m_sleep_intival=settings.getConfig("sleep_intival").m_value.intvalue;
	m_sleep_counter=0;
}

PTaskQueue::~PTaskQueue(void)
{	
	TaskQueue::iterator iter = m_oTasks.begin();
	while( iter!= m_oTasks.end()){

		ITask * task = (*iter);
		//delete task;  removed by tuxuping, 2008-8-28. 
		++iter;
	}
	m_oTasks.clear();
}

void		PTaskQueue::addTask(ITask*		inTask){

	m_oTasks.push_back(inTask);

}

void		PTaskQueue::removeTask(ITask *  inTask){
	//m_TGks.(inTGk);

}

void		PTaskQueue::start(){

	onRun();
}

void		PTaskQueue::stop(){

	m_bIsStop = true;	

}

void		PTaskQueue::runOne(){

	if( m_bIsStop == true ) {
			LOG_INFO("TaskQueue"," task quit is quit" );
			//break;
			return;
		}
		TaskQueue::iterator iter = m_oTasks.begin();
		while( iter!= m_oTasks.end()){

			ITask * task = (*iter);
			//LOG_INFO("TaskQueue","OnRun():"<<task->getName());
			task->onRun();
			++iter;
		}
		//LOG_DEBUG("PTaskQueue","taskqueue.size()="<<m_oTasks.size());
		m_sleep_counter++;
		if(m_sleep_counter>=m_sleep_intival)
		{
			m_sleep_counter=0;
			PUtils::sleep(1); // sleep 1 millseconds.
		}
		
		PTime::setLowResolutionSeconds();
		PTime::setLowResolutionMilliSeconds();

}

void		PTaskQueue::onRun(){
	
	PTime::setLowResolutionSeconds();
	PTime::setLowResolutionMilliSeconds();

	while(true){
		runOne();

	}
}