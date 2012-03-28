// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef  PTASKQUEUE_H 
#define PTASKQUEUE_H 

#include "ITask.h"
#include "os/PUtils.h"
#include <deque>

#ifdef __linux__
typedef  long DWORD;
#endif
typedef  std::deque< ITask* > TaskQueue;


class PTaskQueue
{
public:

	PTaskQueue(void);

	// remove and delete every tGks 
	~PTaskQueue(void);

	void			addTask(ITask*	inTask);

	void			removeTask(ITask*  inTask);

	void			start();	

	void			stop();

	void			runOne();



private:
	// iterate and run every tGk in the queue, it is a loop.
	void			onRun();
private:

	B8				m_bIsStop ;

	TaskQueue		m_oTasks;

	DWORD			m_sleep_intival;

	DWORD			m_sleep_counter;

};


#endif
