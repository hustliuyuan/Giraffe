// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef  IRECV_BUFFER_PARSER_H
#define    IRECV_BUFFER_PARSER_H


#include "network/PSocketType.h"


namespace  Network{

	class IRecvBufferParser;

	class NETWORK_EXPORT ISocketTypeFactoryImpl
	{
	public:

		virtual const Network::PSocketType*      getSocketType( U32 id) const ;

		virtual Network::IRecvBufferParser*      getBufferParser( U32 id) const ;
		
	};

}

#endif // IRECV_BUFFER_PARSER_H
