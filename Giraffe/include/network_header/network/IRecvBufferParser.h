// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

//#ifndef  #define #endif
#ifndef __RECV_BUFFER_PARSER_H__
#define __RECV_BUFFER_PARSER_H__
#include "network/PSocketType.h"
#include "packet/PPacketBase.h"
#include "network/PReceiveBuffer.h"

namespace Network { 

	class Network::PReceiveBuffer;
	//typedef Detail::PReceiveBuffer PReceiveBuffer;

	class NETWORK_EXPORT IRecvBufferParser
	{
	public:

		virtual ~IRecvBufferParser();
		
		//IRecvBufferParser( Network::Detail::PReceiveBuffer * ) ;

		//typedef Network::PSocketType PSocketType;			
		virtual const PPacketBase*	getPacket( )  = 0;

		void	setRecvBuffer( Network::PReceiveBuffer * buffer);

		void	setSocketFD(U32 socket_fd);


	protected:
		
		Network::PReceiveBuffer *  m_pParent;

		U32							sock_fd;

	};

} // namespace network
#endif

