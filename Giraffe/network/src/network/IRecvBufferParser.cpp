// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

//#ifndef  #define #endif

#include "common/Header.h"
#include "network/PReceiveBuffer.h"
#include "network/IRecvBufferParser.h"
//#include "win32.h"


namespace Network { 

	IRecvBufferParser::~IRecvBufferParser()
	{

	}

	void IRecvBufferParser::setRecvBuffer( Network::PReceiveBuffer * buff )
	{
		this->m_pParent = buff;
	}

} // namespace network


