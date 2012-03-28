// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: taozhe. 
// email:  
// date:   2010-4-23
// all rights reserved.


#ifndef PTCP_SOCKET_ASYN_H
#define PTCP_SOCKET_ASYN_H

#include "PSocket.h"
#include "network/PReceiveBuffer.h"
#include "PSendBuffer.h"
#include "network/PSocketTypeFactory.h"
#include "network/PSocketType.h"

#ifdef __LINUX__
#include <netinet/tcp.h>
#endif

namespace Network { namespace Detail {

//修改时必须保证IORead=< X >=IOWriteComplete
#define IORead				(10)    //read data
#define IOReadComplete      (11)    //read data complete
#define IOWrite				(12)    //write data
#define IOWriteComplete     (13)    //write data complete

//这里的TCP_PACKET_SIZE最好不要太大，影响并发速度
#define TCP_PACKET_SIZE		(1024*8)
struct Io_Data
{
	OVERLAPPED overlapped;
	WSABUF     buf;
	//使用m_cTempBuf[TCP_PACKET_SIZE]作为缓冲
	U32       type;
	DWORD	  lenth;
};

class PTCPSocketAsyn: public PSocket
{
public:

	PTCPSocketAsyn( U32 inBlockOrNot, U32 socket_type = PSocketTypeFactory::DEFAULT_SOCKET_TYPE );

virtual ~PTCPSocketAsyn();	
	
public:
	// called by transfer.*************************
	//   error socket put to inCloseSockets.
	B8							write(  AddrSet & inCloseSockets );
	//   error socket put to inCloseSockets.
	//   read packets are put to globally packet queue.
	virtual PacketINFO*			recv(   AddrSet & inCloseSockets );
	
	// 使用异步读的方法	
	B8							onRecv(AddrSet & inCloseSockets);
	//   process listen status. use accept fucntion.	
	PTCPSocketAsyn*				accept(AddrSet & inCloseSockets);

	void						setSocketType( U32 socket_type);

	U32							getSocketType() const ;

	PTCPSocketAsyn*				createEmptySocket(U32	inBlockOrNot) const;

	B8							connect(U32	inRemoteIP, U16	inRemotePort , AddrSet&  willCloseSockets);

	std::string					toString()const;

	B8							SetupReadBuffer(U32 type);

	B8							SetupWriteBuffer(U32 type);

	B8							SetupWriteBufferSize();

	B8							SetupReadBufferSize(U32 lenth);

	B8							AdjustSendPointer(DWORD lenth);

public:
	// called by other functions:  if this is a tcp socket.
	void						send(const PPacketBase*	inPacket);

	B8							setIoWriteData(Io_Data* m_io_data);	

	B8							setIoReadData(Io_Data* m_io_data);	

	B8							setStartWrite(bool bWrite);	

	B8							getStartWrite(BOOL *bWrite);		
		
	B8							setError();	

	U16							getError();		

	void						getCompleteLock();

	void						releaseCompleteLock();
	
	void						getReadCompleteLock();

	void						releaseReadCompleteLock();


	virtual B8					isTCP() const;
	
private:

	void						set(int inSocket, const struct sockaddr_in* remoteaddr);

	PSendBuffer					m_oSendBuffer;

	//Data struct to process io
	Io_Data*					m_io_write_data;

	Io_Data*					m_io_read_data;

	// sending queue.
	OutPacketQueue				m_SendQueue;	

	S8							m_cTempBuf[TCP_PACKET_SIZE];

	DWORD						recv_len;

	PReceiveBuffer				m_oRecvBuffer;

private:
	//m_SendQueue的互斥访问锁
	CRITICAL_SECTION			m_SendQueue_mutex;

	U16							m_error;

	bool						m_start_write;

	//m_start_write的互斥访问锁
	CRITICAL_SECTION			m_write_mutex;

	//这个锁是为了保证IOWriteComplete发出的读信号一定在IOWrite函数执行之后
	CRITICAL_SECTION			m_complete_mutex;

	//这个锁是为了保证IOReadComplete发出的读信号一定在IORead函数执行之后
	CRITICAL_SECTION			m_complete_read_mutex;

	
	friend  class PTransmitter;
	friend  class PTransImplTcpAsyn;
};

} // namespace detail
} // namespace network
#endif // PTCP_SOCKET_ASYN_H
