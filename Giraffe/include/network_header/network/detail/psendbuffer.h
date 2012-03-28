// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#ifndef __PSEND_BUFFER_H__
#define __PSEND_BUFFER_H__

#include "packet/PPacketDefinition.h"
#include "network/PSocketType.h"
#include "common/Header.h"
#include "packet/PPacketBase.h"
#include "packet/PPacketFactory.h"
// 见设计文档：pktown客户端-网络层设计-tcp-sendbuffer ( edited with smartdraw, by tuxuping)
namespace Network { namespace Detail {

	class PSendBuffer
	{
	public:

		typedef Network::PSocketType PSocketType;
		//typedef  Network::PacketINFO  PacketINFO;
		//typedef  Network::PInetAddr PInetAddr;

		enum{
//modified by taozhe 2010.4.17 Attention:MAX_SEND_BUF_SIZE、
//TCPPACKET_MAXSIZE、MAX_SEND_BUF_SIZE have relations
			MAX_SEND_BUF_SIZE =  TCPPACKET_MAXSIZE + 32*1024,
			//MAX_SEND_BUF_SIZE = 5* 1024,
			MAX_PACKET = TCPPACKET_MAXSIZE , //4*1024*1024,

		};
		// removed psockettype from the psendbuffer. 
		// because sendbuffer has no relation to sockettype.2008-7-16, xupingtu,
		PSendBuffer(/*const PSocketType * packettype*/);

		virtual ~PSendBuffer(void);

		// set the sockettype binded with this socket.
		//void					setSocketType(const PSocketType* packettype);		

		// return <size> of the data stored in the buffer, which is to be sent. since this buffer is circled,
		// so, here only count the max size which can be sent in this time. in the next send event, we will 
		// move the sending pointer to the start of the buffer.
		S32 					getFilledSize() const;

		// store the packet to the buffer.
		S32 					writeSendBuffer(const PPacketBase * inPacket);

		//和getSendPointer一样，只是不是const的    by taozhe   2010.4.12
		S8*						getSendBuffer();

		// get the pointer where to send from
		const S8*				getSendPointer() const ;

		void					adjustSendPointer( S32  sent_len );

		void					adjustWritePointer( S32  write_len );

		// 1: if  sender < writer && writer == ender  && getemptysize() < write_len , 
		//    then 
		//        let wrtier = 0;

		B8						tryWritePointer( S32  write_len );

		// 
		std::string 			toString() const ;

	private:

		// return <size> of the empty space of the buffer, it is can be stored another packet.
		S32 					getEmptySize() const;

		B8						isCanWrite(S32	size)const;

		// get the pointer where to write to , invoked by function: writeSendBuffer 
		S32						getWritePointer( ) const ;

		//const PSocketType*		m_pPacketType;

		S8	 					m_cBuff[MAX_SEND_BUF_SIZE];

		//写缓冲区时从这里开始写
		S32						m_iWritePointer;

		//发送时从这里开始发送
		S32						m_iSendPointer;

		//发送时到这里结束，一般和writepointer一样，但是有时不一样,见下图。
		S32						m_iEndPointer;

	};
}
}
#endif

/* 
** |			|/////////////////|                               |             
** ----------------------------------------------------------------
**              ^                 ^
** buf_start    |                 |                               
**        send_pointer       write_pointer                  buf_end
**							 end_pointer 
***/


/* 
** |			                  |////////////////////////////// |             
** ----------------------------------------------------------------
** ^							  ^
** |							  |                               
** buf_start				  send_pointer						buf_end
** write_pointer												end_pointer 
***/
