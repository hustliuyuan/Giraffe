// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef PNATHEARTBEAT_PKT_H
#define PNATHEARTBEAT_PKT_H


#include "packet/PPacketBase.h"
#include "packet/PPacketDefinition.h"


//Keep HeartBeat with NAT
class PNATHeartBeatPkt :
	public PPacketBase
{

public:
	PNATHeartBeatPkt();



	//virtual U32			getLength( ) const;
	//virtual U32			serialize( S8 *  buf,  S32 offset ) const ;
	//virtual U32			deserialize( const S8 * buf, S32 offset ) ;
	virtual String      toString() const ;

	virtual U32		toString(S8 * str) const;
private:
     //ÄÚÈÝ´ý¶¨
};

#endif