// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#include "common/Header.h"
#include "network/detail/PTransImplUdp.h"
#include "packet/PPacketDefinition.h"

namespace Network { namespace Detail {
		
void		PTransImplUdp::onRun()
{



}

	// return the name of this transmitter.
	// the programmer must implement this in the desenstor of ITransmitter
std::string		PTransImplUdp::getName() const
{


	return "PTransImplUdp";

}

} // namespace detail
} // namespace network