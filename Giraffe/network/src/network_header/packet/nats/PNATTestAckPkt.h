// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef PNAT_TEST_ACK_PKT_H 
#define PNAT_TEST_ACK_PKT_H

#include "packet/PPacketBase.h"
#include "packet/PPacketDefinition.h"
#include "network/PAddressable.h"

using namespace Network;

//Keep HeartBeat with NAT
class PNATTestAckPkt :
	public PPacketBase
{

public:
	PNATTestAckPkt();
	PNATTestAckPkt( const PAddressable & addr );



	virtual U32			getLength( ) const;
	virtual U32			serialize( S8 *  buf,  S32 offset ) const ;
	virtual U32			deserialize( const S8 * buf, S32 offset ) ;
	virtual String      toString() const ;
	virtual U32		toString(S8* str) const;

	const PAddressable & getSender() const ;

private:

	PAddressable		m_oSenderAddress;
};


#endif //PNATTestAckPkt 