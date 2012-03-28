
// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#ifndef __MY_RECV_BUFFER_PARSER_H__
#define __MY_RECV_BUFFER_PARSER_H__
#include "network/PSocketType.h"
#include "common/Header.h"
#include "packet/PPacketBase.h"
#include "network/IRecvBufferParser.h"

namespace Network { namespace Detail {  

	class PDefaultRecvBufferParser : public Network::IRecvBufferParser
	{
	public:

		//typedef Network::PSocketType PSocketType;			
		virtual const PPacketBase*		getPacket( );

	};

} // namespace Detail
} // namespace network
#endif

