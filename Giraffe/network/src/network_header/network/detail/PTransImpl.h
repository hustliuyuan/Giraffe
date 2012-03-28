// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#ifndef  PTRANS_IMPL_H 
#define PTRANS_IMPL_H 

#include "common/ITask.h"
#include "network/detail/PUDPSocket.h"
#include "network/PInetAddr.h"
#include "config/PSettings.h"
#include "network/detail/ITransImpl.h"
#include "network/detail/SocketINFO.h"

#include <map>

//using  namespace Network;

namespace Network { namespace Detail {

	class PTransImpl 
		: public ITransImpl	
	{
	public:

		typedef  Network::PSocketType PSocketType;
		typedef  Network::PacketINFO  PacketINFO;
		typedef  Network::PInetAddr PInetAddr;
		//typedef  std::set< U16 >		NewConnectionSet;

		enum{

			READ_FROM_CONFIG_FILE = 0, // indicate the tcp port and udp port can be read from the configuration file.
		};

		PTransImpl();

		virtual							~PTransImpl(void);

		//add the a copy of the packet to the corresponding socket's send buffer.

		virtual void					unreliableSend( const PInetAddr & inAddr , const PPacketBase * inPacket );

		// modified by tuxuping, 2008-7-22. support different socket type.
		// old is "B8	addUDPListener(U16		inPort);"		
		B8								addUDPListener(U16	inPort, U32 socket_type = PSocketTypeFactory::DEFAULT_SOCKET_TYPE);

		virtual	void					registerPacketHandler( IPacketHandler * packet_handler ) ;


	protected:

		// the decendent class  will invoke this function to handle all the packets.
		virtual	void					handlePkt( const PacketINFO *  info ) ;


	protected:
		
		PUDPSocket * 					m_UdpSockMap[PSocketTypeFactory::MAX_SOCKET_TYPE_NUM];

		SocketMap						m_LiveSockets;

		IPacketHandler *				m_pPacketHandler;



	};
} // namespace detail
} // namespace network



#endif //PTRANS_IMPL_H
