#pragma once

#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread.hpp>
#include <queue>


template <typename T > 
class  PSynQueue
{
	typedef boost::recursive_mutex MutexType;//使用递归锁

public:
	void push( const T& t) 
	{
		m_oQueue.push(t);
	}

	const T&	 front ( ) const
	{
		return m_oQueue.front();
	}

	void    pop( )
	{
		m_oQueue.pop();
	}

	bool	empty( ) const
	{
		return m_oQueue.empty();
	}

	void	lock( ) 
	{
		m_oMutex.lock();
	}

	void	unlock( )
	{
		m_oMutex.unlock();
	}

protected:
	//typedef MutexType::scoped_lock ScopedLockType;//使用scoped lock技术
	MutexType m_oMutex;//锁
	std::queue< T >  m_oQueue;
};
