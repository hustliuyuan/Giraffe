// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.



#ifndef PSOCKET_TYPE_FACTORY_H
#define  PSOCKET_TYPE_FACTORY_H 


#include "network/PSocketType.h"
#include "network/ISocketTypeFactoryImpl.h"


namespace  Network{

	class NETWORK_EXPORT PSocketTypeFactory
	{
	private:

		PSocketTypeFactory();

	public:

		virtual ~PSocketTypeFactory(void);

		enum{
			UNKNOWN_SOCKET_TYPE   = -1,

			DEFAULT_SOCKET_TYPE   = 1,
			//FLASH_AS2_SOCKET_TYPE = 2,
			//RESERVED_1 = 3,
			//HTTP_PACKET = RESERVED_1  + 0,
			//WAR3_PACKET = RESERVED_1  + 1,
			USER_DEFINED = 5,

			////ASF_PACKET  = USER_DEFINED  + 2,

			MAX_SOCKET_TYPE_NUM = 10,

		};


		static const PSocketType*	getSocketType(U32  id ) ;

		static IRecvBufferParser*	getBufferParser(U32  id ) ;

		static void					setImpl( ISocketTypeFactoryImpl *  ) ;

		//static void					deleteImpl( );
		static IRecvBufferParser*	newBufferParser(U32 id);

	private:

		static ISocketTypeFactoryImpl *	m_pTypeFactoryImpl;


	};

}


#endif // PSOCKET_TYPE_FACTORY_H 
