#ifndef __GIRAFFE_SERVER_SESSION_DATA_H__
#define __GIRAFFE_SERVER_SESSION_DATA_H__

#include "Base/GHeader.h"

typedef set<int> HandleSet;

class SessionData
{
	public:
		SessionData():m_id(0)
		{
		}

		SessionData( Network::PInetAddr addr, S64 peroid)
		{	
			m_leaseInterval = peroid;
			m_id = 0;
			m_addr = addr;
		}

		~SessionData()
		{

		}

		inline void addHandle(const unsigned int id)
		{
			m_handleSet.insert(id);
			//m_handleSet[m] = id;

		}

		void getHandles(vector<unsigned int> &handles)
		{
			HandleSet::iterator it = m_handleSet.begin();
			while (it != m_handleSet.end())
			{
				handles.push_back( *it);
				++ it;
			}
		}

		inline void  rmHandle(const unsigned int id)
		{
			m_handleSet.erase(id);
		}

		inline bool exitsHandle(const unsigned int id)
		{
			//HandleSet ::iterator it = m_handleSet.find(id);
			HandleSet ::iterator it = m_handleSet.begin();
			while(m_handleSet.end() != it)	
			{	
				if ( *it == id)
					return true;
				else
					++ it;
			}
			return false;
		}

		void renew_expire_time()
		{
			S64 cur =  PUtils::localMilliSeconds();
			m_expireTime = cur + m_leaseInterval;
		}

		int diff_time(S64 t)
		{
			if( m_expireTime <=t)
			{
				return 1;
			}
			return 0;
		}

		inline unsigned int getID()
		{
			return m_id;
		}

		inline void setID(unsigned int id)
		{
			m_id = id;
		}

		inline Network::PInetAddr getAddr()
		{
			return m_addr;
		}

		inline int get_jeo_time()
		{
			return m_jeotimes;
		}

		inline void dec_jeo_time()
		{
			m_jeotimes --;
		}

		inline void reset_jeo_time()
		{
			m_jeotimes = 2;
		}

	private:
		unsigned int m_id;
		S64 m_expireTime;
		S64 m_leaseInterval;
		//int m_dis_beat_heart_times;
		HandleSet m_handleSet;
		int m_jeotimes;
		Network::PInetAddr m_addr;//session client location
		//MasterPtr master;
		
};
typedef SessionData* SessionDataPtr;
#endif //__GIRAFFE_SERVER_SESSION_DATA_H__
