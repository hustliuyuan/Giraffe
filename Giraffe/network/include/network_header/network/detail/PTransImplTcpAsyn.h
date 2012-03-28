// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: taozhe. 
// email:  taozhe1989@gmail.com, 
// date:   2010-04-02
// all rights reserved.


#ifndef  _PTRANS_IMPL_TCP_ASYN_H_ 
#define	_PTRANS_IMPL_TCP_ASYN_H_   

#include "common/ITask.h"
#include "PTCPSocketAsyn.h"
#include "PUDPsocket.h"
#include "network/PInetAddr.h"
#include "config/PSettings.h"
//#include "network/PTransmitter.h"
#include "SocketINFO.h"
#include "PTransImpl.h"
#include <set>
#include <map>

namespace Network { namespace Detail {

using  namespace Network;

#define DEFAULT_THREAD_NUM    (4)

#define MAX_THREAD_NUM		  (32)

struct CompleteKey
{
	SOCKET				m_Socket; // The Connection socket. 
	SOCKADDR_IN			m_Socket_Addr;//use m_Socket_Addr and m_LiveSockets to find PTCPSocketAsyn.
};


class PTransImplTcpAsyn :
	public PTransImpl
{
public:

	typedef  Network::PSocketType PSocketType;
	typedef  Network::PacketINFO  PacketINFO;
	typedef  Network::PInetAddr PInetAddr;
	typedef  std::set< U16 >		NewConnectionSet;

	enum{

		READ_FROM_CONFIG_FILE = 0, // indicate the tcp port and udp port can be read from the configuration file.
	};
	
	PTransImplTcpAsyn();

virtual								~PTransImplTcpAsyn(void);
	
	virtual void						reliableSend( const PInetAddr & inAddr ,  const PPacketBase   * inPacket );	

	virtual void						reliableSend( U32 inSocket,  const PPacketBase   * inPacket );	
	
	virtual void						onRun();

	virtual std::string					getName() const;

	B8									initialize ( U16 tcpPort = READ_FROM_CONFIG_FILE, U16 udpPort =READ_FROM_CONFIG_FILE);

	B8									addTcpListener(U32 local_ip, U16	inPort, U32  socket_type = PSocketTypeFactory::DEFAULT_SOCKET_TYPE);

protected:

	void								removeSocket(const PInetAddr &	inAddr);	

	B8									tcpClose( const PInetAddr & inAddr );
	
	B8									tcpSend( const PInetAddr & addr , const PPacketBase   * inPacket );	

protected:

	CompleteKey*						AllocateCompleteKey();

	Io_Data*							AllocateIoData(int type);

	B8									ReleaseCompleteKey(CompleteKey* complete_key);

	B8									ReleaseIoData(Io_Data* io_data_buffer);

	//不再使用这三个方法来处理IO，使用OnRead...等五个方法
	//void								readDataFromSocket( SocketINFO * info ) ;
	//void								writeDataToSocket ( PSocket * s ); 
	//void								handleException( PSocket * s ) ;

	static DWORD WINAPI					IOThread(LPVOID pParam);
	
	//create  complete port ....mabye could before create socket
	B8									CreateCompletionPort();

	//Process the io
	B8									ProcessIO(DWORD dwIoSize,CompleteKey * complete_key,LPOVERLAPPED lpOverlapped);

	B8									OnRead(PTCPSocketAsyn* socket_aysn,DWORD dwIoSize,Io_Data* m_io_data);

	B8									OnReadCompleted(PTCPSocketAsyn* socket_aysn,DWORD dwIoSize,Io_Data* m_io_data,CompleteKey* complete_key);

	B8									OnWrite(PTCPSocketAsyn* socket_aysn,DWORD dwIoSize,Io_Data* m_io_data);

	B8									OnWriteCompleted(PTCPSocketAsyn* socket_aysn,DWORD dwIoSize,Io_Data* m_io_data，,CompleteKey* complete_key);

	//每次新出现的socket都必须调用这个方法
	CompleteKey*						CompletePortInitialize(PTCPSocketAsyn* socket_aysn);

	//传递一个status信号，可以是IORead、IOReadCompleted、... ...
	B8									PostCompletionStatus(CompleteKey* complete_key,Io_Data* data_buffer,int type,PTCPSocketAsyn *s);


	PTCPSocketAsyn*						getSocketByAddr(SOCKADDR_IN Addr);

	//不再使用这个来控制error的socket
	AddrSet								m_WillCloseSockets;
	
	//only listen one port for test
	PTCPSocketAsyn*						m_TcpListener;

	PTCPSocketAsyn*						m_TcpServerMap[Network::PSocketTypeFactory::MAX_SOCKET_TYPE_NUM];

	NewConnectionSet					m_oInitialConnection;
	
	// Event for handling Network IO
	HANDLE								m_hEvent;
	
	// Handle of completion port
	HANDLE								m_hCompletionPort;

	std::queue<CompleteKey*>            m_key_queue;

	//Handle of the io thread
	HANDLE								m_iothread_group[MAX_THREAD_NUM];

	DWORD								dwThreadID[MAX_THREAD_NUM];

	//m_freebuffer_queue的互斥访问锁
	CRITICAL_SECTION					m_freebuffer_mutex;

	//m_livesockets的互斥访问锁
	CRITICAL_SECTION					m_livesockets_mutex;


	//用来存放free io data 结构的
	std::queue<Io_Data*>				m_freebuffer_queue;

	U16									THREAD_NUM;

};


} // namespace detail
} // namespace network

#endif //_PTRANS_IMPL_TCP_ASYN_H_
