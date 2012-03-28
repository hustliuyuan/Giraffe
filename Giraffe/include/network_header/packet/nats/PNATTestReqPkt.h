// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.
#ifndef  PNAT_TEST_REQ_PKT_H
#define PNAT_TEST_REQ_PKT_H 


#include "packet/PPacketBase.h"
#include "packet/PPacketDefinition.h"
#include "network/PAddressable.h"

using namespace Network;

class PNATTestReqPkt :
	public PPacketBase
{

public:
	PNATTestReqPkt();
	PNATTestReqPkt( const PAddressable & addr );

	virtual U32			getLength( ) const;
	virtual U32			serialize( S8 *  buf,  S32 offset ) const ;
	virtual U32			deserialize( const S8 * buf, S32 offset ) ;
	virtual String      toString() const ;
	virtual U32		toString(S8* str) const;

	const PAddressable & getSender() const ;
private:

	PAddressable		m_oSenderAddress;

};

#endif //PNAT_TEST_REQ_PKT_H
