// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#ifndef  PTRANS_IMPL_TCP_H 
#define PTRANS_IMPL_TCP_H  

#include "common/ITask.h"
#include "PTCPSocket.h"
#include "PUDPSocket.h"
#include "network/PInetAddr.h"
#include "config/PSettings.h"
#include "network/detail/PTransImpl.h"
#include "SocketINFO.h"
#include "network/detail/PTransImpl.h"

#include <map>

namespace Network { namespace Detail {

using  namespace Network;

class PTransImplTcp :public PTransImpl
{
public:

	typedef  Network::PSocketType PSocketType;
	typedef  Network::PacketINFO  PacketINFO;
	typedef  Network::PInetAddr PInetAddr;
	typedef  std::set< U16 >		NewConnectionSet;

	enum{

		READ_FROM_CONFIG_FILE = 0, // indicate the tcp port and udp port can be read from the configuration file.
	};
	
	PTransImplTcp();

	virtual								~PTransImplTcp(void);
	
	//add the a copy of the packet to the corresponding socket's send buffer.

	virtual void						reliableSend( const PInetAddr & inAddr ,  const PPacketBase   * inPacket );	
	
	
	virtual void						reliableSend( U32 inSocket,  const PPacketBase   * inPacket );	
	
	//InPacketQueue *						getPacketQueue()  ; // cant be const fucntion

	// this onRun() just for single thread progrmmer model
	virtual void						onRun();

	// return the name of this transmitter.
	// the programmer must implement this in the desenstor of ITransmitter
	virtual std::string					getName() const;

	//private:
	//    initialize  tcp listner, udp listner.  and socket environment
	B8									initialize ( U16 tcpPort = READ_FROM_CONFIG_FILE, U16 udpPort =READ_FROM_CONFIG_FILE);
	// delete  tcp listner socket , udp listner socket .
	// removed by tuxuping . 2008-9-1, 
	//B8									unInitialize();



		// called only once.
	// modified by tuxuping, 2008-7-22. support different socket type.
	// old is "B8	addTcpListener(U16		inPort);"	
	// modified by tuxuping , addtcpListner is "protected" before, now is changed to "public" 
	// 2009-6-22
	B8									addTcpListener(U32 local_ip, U16		inPort, U32  socket_type = PSocketTypeFactory::DEFAULT_SOCKET_TYPE);

protected:

	void								removeSocket(const PInetAddr &	inAddr);	
	
	//	  close the corresponding socket 	

	B8									tcpClose( const PInetAddr & inAddr );
	// select the sockets in the socket map .
	// process read .  write request and error request of every socket and remote address.
	
	B8									tcpSend( const PInetAddr & addr , const PPacketBase   * inPacket );	
	

protected:

	// may be new connections.
	// reade packets from this specified socket s.
	void								readDataFromSocket( SocketINFO * info ) ;

	void								writeDataToSocket ( PSocket * s ); 

	void								handleException( PSocket * s ) ;

	// all sockets ( excepte local players ) are  here.
	//SocketMap							m_LiveSockets;	

	// my guid and address.
	//PGuid								m_oGuid;

	//PInetAddr							m_oAddr;

    // those sockets will be closed are here 
	AddrSet								m_WillCloseSockets;
	// tcp listener.
	//PTCPSocket	*						m_pTCPListener;
	// udp listener and sender are the same socket.
	
	PTCPSocket*							m_TcpServerMap[Network::PSocketTypeFactory::MAX_SOCKET_TYPE_NUM];

	// if a packet 's "type subtype" is in this set, we will start a new tcp connection.
	NewConnectionSet					m_oInitialConnection;

	
};


} // namespace detail
} // namespace network

#endif //PTRANS_IMPL_TCP_H 
