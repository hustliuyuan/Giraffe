// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.




#ifndef __TCPSOCKET_H__
#define __TCPSOCKET_H__

#include "PSocket.h"
#include "network/PReceiveBuffer.h"
#include "psendbuffer.h"
#include "network/PSocketTypeFactory.h"
#include "network/PSocketType.h"

#ifdef __linux__
#include <netinet/tcp.h>
typedef long DWORD;
#endif

namespace Network { namespace Detail {

class PTCPSocket: public PSocket
{
public:

	//enum {
	//	TCP_PACKET_SIZE = 1024*8,//32 * 1024,
	//};

	PTCPSocket( U32 inBlockOrNot, U32 socket_type = PSocketTypeFactory::DEFAULT_SOCKET_TYPE );

	virtual ~PTCPSocket();	
	
public:
	// called by transfer.*************************
	//   error socket put to inCloseSockets.
	B8							write(  AddrSet & inCloseSockets );
	//   error socket put to inCloseSockets.
	//   read packets are put to globally packet queue.
	virtual PacketINFO*			recv(   AddrSet & inCloseSockets );  

	//   process listen status. use accept fucntion.	
	PTCPSocket*					accept( AddrSet & inCloseSockets);

	void						setSocketType( U32 socket_type);

	U32							getSocketType() const ;

	PTCPSocket*					createEmptySocket(U32	inBlockOrNot) const;

	std::string					toString()const;

	
	// called by other functions:  if this is a tcp socket.
	void						send(const PPacketBase*	inPacket);
		

	virtual B8					isTCP() const;

	B8							connect(U32	inRemoteIP, U16	inRemotePort , AddrSet&  willCloseSockets);

private:

	void						set(int inSocket, const struct sockaddr_in* remoteaddr);

	PSendBuffer					m_oSendBuffer;
	
	// sending queue.
	OutPacketQueue				m_SendQueue;	

	S8							m_cTempBuf[TCPPACKET_MAXSIZE];

	PReceiveBuffer				m_oRecvBuffer;
#if defined WIN32 || defined _WIN32
	CRITICAL_SECTION			m_SendQueue_mutex;
#endif

	DWORD						timer_begin;

	DWORD						send_counter;

	DWORD						send_pkt_count_num;

	friend  class PTransmitter;
};

} // namespace detail
} // namespace network
#endif // __TCPSOCKET_H__

