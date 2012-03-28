// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef  INAT_TRANS_IMPL_H 
#define INAT_TRANS_IMPL_H 

//#include "network/PAddressable.h"
#include "packet/PPacketBase.h"
#include "network/detail/ITransImpl.h"
#include "network/PAddressable.h"

// different thread can / or can not send the packet without explicitly synchronization!
namespace Network { namespace Detail {

	class INatTransImpl : 
		public ITransImpl

	{
	public:

		virtual void					reliableSend( const PAddressable & inAddr ,  const PPacketBase   * inPacket ) =0 ;

		virtual void					unreliableSend( const PAddressable & inAddr , const PPacketBase * inPacket ) = 0;

		virtual void					reliableSend( U32 inSocket,  const PPacketBase   * inPacket ) = 0;	
		
		virtual void					reliableSend( const PInetAddr & inAddr ,  const PPacketBase   * inPacket ) =0;

		virtual void					unreliableSend( const PInetAddr & inAddr , const PPacketBase * inPacket ) =0;
		
		virtual const PAddressable &	getMyAddressable()const =0;

	};

}
}

#endif // INAT_TRANS_IMPL_H
