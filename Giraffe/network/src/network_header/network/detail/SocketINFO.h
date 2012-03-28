// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.



#ifndef  SOCKET_INFO_H
#define SOCKET_INFO_H

#include "common/Header.h"
#include "PSocket.h"
#include "network/PInetAddr.h"
#include <map>

namespace Network { namespace Detail {

	class SocketINFO{

	public:

		typedef Network::PSocketType PSocketType;
		typedef  Network::PacketINFO  PacketINFO;
		typedef  Network::PInetAddr PInetAddr;

		SocketINFO(PSocket* s, B8 read, B8 write, B8 istcpserver = false /*B8 localclient=false*/ ) {

			pSocket = s;

			bSelectRead = read;

			bSelectWrite = write;

			//bLocalClient = localclient;		

			bIsTcpServer = istcpserver;

		}

		PSocket*   pSocket;

		B8	   bSelectRead;

		B8	   bSelectWrite;

		//B8	   bLocalClient; // removed by tuxuping , 2008-7-24.

		B8	   bIsTcpServer; // new added by tuxuping , 2008-7-24

	};

	typedef std::map< Network::PInetAddr,  SocketINFO * >  SocketMap;

} // namespace detail
} // namespace network

#endif //  SOCKET_INFO_H




