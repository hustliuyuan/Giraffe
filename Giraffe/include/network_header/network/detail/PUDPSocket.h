
// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#ifndef __UDPSOCKET_H__
#define __UDPSOCKET_H__

#include "PSocket.h"
#include "network/PReceiveBuffer.h"
#include "network/PSocketType.h"
#include "network/PSocketTypeFactory.h"
#include "network/PacketINFO.h"
#include <list>

namespace Network { namespace Detail {

	class PUDPSocket: public PSocket
	{
	public:

		typedef std::list< const PacketINFO * > UDPOutPacketList;

		PUDPSocket( U32 inBlockOrNot, U32 socket_type = PSocketTypeFactory::DEFAULT_SOCKET_TYPE );

		virtual					~PUDPSocket();

	public:	


		// called by transfer.*************************
		//   error socket put to inCloseSockets.
		//   read packets are put to globally packet queue.
		virtual PacketINFO*		recv(   AddrSet & inCloseSockets );  


		// called by other functions:  if this is a udp socket.	
		virtual B8				isTCP()const;

		B8						write(  AddrSet & inCloseSockets );

		void					send( const PacketINFO * info);

	protected:

		S8						m_cInBuff[UDPPACKET_MAXSIZE];

		S8						m_cOutBuff[UDPPACKET_MAXSIZE];

		// sending buffer.
		UDPOutPacketList		m_SendList;

		// change "const PSocketType*" to sockettype 2009-6-16 tuxuping
		U32						m_iSocketType;

		DWORD					timer_begin;

		DWORD					send_counter;

		DWORD					send_pkt_count_num;

		//friend  class PTransmitter;
	};
} // namespace detail
} // namespace network

#endif // __UDPSOCKET_H__

