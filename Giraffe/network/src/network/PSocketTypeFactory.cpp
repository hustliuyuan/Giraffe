// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#include "common/Header.h"
#include "os/PUtils.h"
#include "network/PSocketTypeFactory.h"

namespace Network {

	ISocketTypeFactoryImpl* PSocketTypeFactory::m_pTypeFactoryImpl = NULL;


	PSocketTypeFactory::PSocketTypeFactory() 
	{
	}

	PSocketTypeFactory::~PSocketTypeFactory(void)
	{
		delete m_pTypeFactoryImpl;
	}


	const PSocketType*	PSocketTypeFactory::getSocketType(U32  id ) 
	{
		if( m_pTypeFactoryImpl == NULL )
		{
			
			m_pTypeFactoryImpl = new ISocketTypeFactoryImpl();			
		}
		return m_pTypeFactoryImpl->getSocketType(id);

	}

	IRecvBufferParser*	PSocketTypeFactory::getBufferParser(U32  id ) 
	{
		if( m_pTypeFactoryImpl == NULL )
		{
			LOG_WARN("PSocketTypeFactory", "getBufferParser():no implementation"<< id);
			return NULL;
		}
		return m_pTypeFactoryImpl->getBufferParser(id);

	}

	void	PSocketTypeFactory::setImpl(ISocketTypeFactoryImpl * sockFactoryImpl) 
	{
		m_pTypeFactoryImpl = sockFactoryImpl;
	}

}