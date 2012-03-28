// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef __RECEIVE_BUFFER_H__
#define __RECEIVE_BUFFER_H__

#include "packet/PPacketDefinition.h"
#include "network/PSocketType.h"
#include "packet/PPacketBase.h"

#include <set>


namespace Network { //namespace Detail {

	// 设计文档正在整理中,
	// 见设计文档：pktown客户端-网络层设计-tcp-recvbuffer ( edited with smartdraw, by tuxuping)

	class NETWORK_EXPORT PReceiveBuffer
	{
	public:

		typedef Network::PSocketType PSocketType;
		//typedef Network::IRecvBufferParser IRecvBufferParser;

		//typedef  Network::PacketINFO  PacketINFO;
		//typedef  Network::PInetAddr PInetAddr;

		enum{
//modified by  taozhe 2010.4.15
//MAX_SEND_BUF_SIZE = 5*1024,接收缓冲的大小必须是发送的两倍以上，
//而且，pkt的大小(TCPPACKET_MAXSIZE)必须小于发送缓冲大小
			

			MAX_PACKET = TCPPACKET_MAXSIZE, //8*1024*1024,
			MAX_RCV_BUF_SIZE =  MAX_PACKET + 32*1024,

			ONLY_TO_READ = 1,
			ONLY_TO_WRITE = 2,
			ONLY_TO_NORMAL =3
		};

		PReceiveBuffer( U32 socket_type);

		virtual ~PReceiveBuffer(void);

		// let this be virtual functions. 2009-5-16
		virtual const PPacketBase*		getPacket( );

		//bool					IsCanRead( )const;

		S32 					getEmptySize() const;

		void					setSocketType(U32 socke_type);

		const PSocketType*		getSocketType( ) const;

		U32 					getFilledSize() const;

		// added by tuxuping 2009-6-12.
		void					setReadPointer(U32  pos) ;
		U32					    getReadPointer() const;
		//void					setWritePointer(U32  pos) ;
		U32					    getWritePointer() const; // added by tuxuping 2010-Jun-9 support HTTP protocol.
		S8*						getBufferHeader()const;

		S32 					readReceiveBuffer(char * dstBuff, const S32 readLen);

		S32 					writeReceiveBuffer(const char *pSrcBuff, const S32 writeLen);

		//std::string 			toString(const char * buff);
		// log the reader pointer, write pointer, 
		std::string				toString( ) const ;

		void			        toString(std::string	& ) const ;

		void			        toString(S8 * str, U32 from_pos, U32 total_len ) const ;

		void					outputState()const;

		void					setSocketFD(U32 m_port);//taozhe 2010 5.5
		
		U16						getSocketFD();

	private :

		//const PPacketBase *		getMyPacket( );

		//const PPacketBase *		getWAR3Packet( );

		//const PPacketBase *		getXmlPacket( );

	private:

		B8						isCanWrite(U32	size)const;


		const PSocketType*		m_pPacketType;

		S8	 					m_cBuff[MAX_RCV_BUF_SIZE];

		U32						m_iWritePointer;

		U32						m_iReadPointer;
		
		U32						port;

		Network::IRecvBufferParser *  parser;

	};
//} // namespace detail
} // namespace network
#endif

