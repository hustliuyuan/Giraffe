// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware

// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#include "common/Header.h"

#include "network/PSocketType.h"
#include "network/PSocketTypeFactory.h"
#include "network/detail/PDefaultRecvBufferParser.h"


namespace Network {



	const Network::PSocketType*	ISocketTypeFactoryImpl::getSocketType(U32  id ) const {

		switch( id )
		{
		case PSocketTypeFactory::DEFAULT_SOCKET_TYPE :
			{
				static PSocketType 
					s_iMY (PSocketTypeFactory::DEFAULT_SOCKET_TYPE,NULL,0,0,2,0);
				return &s_iMY;
			}

			// here is some example, for you to extend this sockettype. tuxuping , 
			// please do not remove them.

			//case PSocketTypeFactory::FLASH_AS2_SOCKET_TYPE:
			//	 {
			//	  static PSocketType s_iFLASH_AS2(PSocketTypeFactory::FLASH_AS2_SOCKET_TYPE,
			//		  '\0',1,(U32)strlen("<packet size=\""),(U32)strlen("0xffff"),0);

			//	  return &s_iFLASH_AS2;
			//	 }
			//	case HTTP_PACKET:
			//	 {
			//	  static PSocketType 
			//		  m_iHTTP ( PSocketTypeFactory::HTTP_PACKET,"\r\n\r\n",4,0,0,0);

			//	  return &m_iHTTP;
			//	 }
			//	 
			//	case WAR3_PACKET  :
			//	 {
			//	 static PSocketType 
			//	  m_iWAR3 (PSocketTypeFactory::WAR3_PACKET,NULL,0,2,2,0);

			//	 return &m_iWAR3;
			//	 }

		default:
			LOG_WARN("ISocketTypeFactoryImpl", "getSocketType():no implementation for major type:" << id);
			return NULL;

		}

	}

	 IRecvBufferParser*	
		ISocketTypeFactoryImpl::getBufferParser(U32  id ) const 
	{
		switch( id )
		{
		case  PSocketTypeFactory::DEFAULT_SOCKET_TYPE:
			{
				static Network::Detail::PDefaultRecvBufferParser s_myRecvParser;

				return &s_myRecvParser;
			}

		default:
			LOG_WARN("ISocketTypeFactoryImpl", "getBufferParser(): no implementation for major type:" << id);
			return  NULL;
			
		}
	}

	
}