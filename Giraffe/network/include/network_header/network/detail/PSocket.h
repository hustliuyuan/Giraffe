#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "common/Header.h"
#include "packet/PPacketBase.h"
#include "network/PInetAddr.h"

#include "network/PacketINFO.h"
#include <set>
#include <map>

#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif
#endif

	typedef  Network::PSocketType PSocketType;
	typedef  Network::PacketINFO  PacketINFO;
	//typedef  Network::PInetAddr PInetAddr;	

	typedef std::set < Network::PInetAddr > AddrSet;

	typedef std::map < Network::PInetAddr  , PPacketBase* > PacketMap;

	typedef std::queue < const PPacketBase* > OutPacketQueue;

namespace Network { namespace Detail {

class PSocket
{
public:

	PSocket( U32 inStreamOrDataGram, U32 inBlockOrNot);

	enum	// PGs this in on socket constructors to specify whether the
	{		// socket should be non-blocking or blocking
		NONE_BLOCK_SOCK = 1,
		BLOCK_SOCK= 0
	};
	
	enum
	{
		kInvalidFileDesc = -1	//int
	};	
public:
	// called by transfer.*************************
	//   error socket put to inCloseSockets.
	//B8			write(  AddrSet & inCloseSockets );
	//   error socket put to inCloseSockets.
	//   read packets are put to globally packet queue.
	virtual PacketINFO*			recv(   AddrSet & inCloseSockets );  

public:
	
	// called by other functions:  if this is a udp socket.
	
	S32							getSocketFD()  const     { return m_iFileDesc; }

	B8							close();

	virtual						~PSocket();

	virtual B8					isTCP() const =0;

	B8							open();

	//this is Aysn socket's open fuction     by:taozhe,2010.4.5 
	B8							openAysn();

	PInetAddr					getDestAddr() const{

		return					m_DestAddr;
	}

	PInetAddr					getLocalAddr() const {

		return			m_LocalAddr;
	}

	virtual std::string			toString() const;

	B8							bind(U32	inIP, U16 inPort);

	//bool				IsWrite();

	//bool				IsRead();

#ifdef __linux__
	static U32		GetEth0Addr();
#endif

protected:

	B8							setNoneBlockRecv( ) ; 

	S32							m_iFileDesc;	// socket handler.    -1 : means invalid socket handler.
	// the socket is tcp or udp
	U32							m_StreamType;	// Stream : SOCK_STREAM , DataGram:  SOCK_DGRAM
	// the socket is working in block or none-block mode. 
	U32							m_bIsNoneBlock;	// 1: NONE_BLOCK_SOCK, 0: BLOCK_SOCK; default is None-Block.

	PInetAddr					m_LocalAddr;

	PInetAddr					m_DestAddr;
	
	friend  class PTransmitter;

};

} // namespace detail
} // namespace network
#endif // __SOCKET_H__

//extern PacketQueue			gPacketQueue;


