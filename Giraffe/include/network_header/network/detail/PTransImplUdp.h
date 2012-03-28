// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef  PTRANS_IMPL_UDP_H
#define  PTRANS_IMPL_UDP_H

#include "common/ITask.h"
#include "PTCPSocket.h"
#include "PUDPSocket.h"
#include "network/PInetAddr.h"
#include "config/PSettings.h"
#include "SocketINFO.h"
#include "network/detail/PTransImpl.h"

#include <map>

namespace Network { namespace Detail {

using  namespace Network;

class PTransImplUdp :
	 public PTransImpl
{
public:

	typedef  Network::PSocketType PSocketType;
	typedef  Network::PacketINFO  PacketINFO;
	typedef  Network::PInetAddr PInetAddr;

	enum{

		READ_FROM_CONFIG_FILE = 0, // indicate the tcp port and udp port can be read from the configuration file.
	};
	
	PTransImplUdp();

	virtual								~PTransImplUdp(void);
	
	//add the a copy of the packet to the corresponding socket's send buffer.

	virtual void						reliableSend( const PInetAddr & inAddr ,  const PPacketBase   * inPacket );	

	virtual void						reliableSend( U32 inSocket,  const PPacketBase   * inPacket );	
	
		
	virtual void						onRun();

	// return the name of this transmitter.
	// the programmer must implement this in the desenstor of ITransmitter
	virtual std::string					getName() const;

protected:


	
};

} // namespace detail
} // namespace network


#endif  //PTRANS_IMPL_UDP_H
