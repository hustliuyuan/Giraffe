// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#ifndef  PACKET_INFO_H
#define    PACKET_INFO_H

#include "network/PInetAddr.h"
#include "packet/PPacketBase.h"
#include "PAddressable.h"
#include <queue>
#include <string>

namespace Network{

	class  NETWORK_EXPORT PacketINFO {

	private:
		PacketINFO();

	public:

		enum{
			UNDEFINED_DESCRIPTOR = -1,
		};

		// functions.
		std::string				toString() const;

		U32					toString(S8 * to_str) const;

		PacketINFO( const PInetAddr	& inAddr, S32 socket_descriptor, const PPacketBase*	inPacket);
		// added by tuxuping 2008-9-25, to support the packet is relayed by nat server.
		// if a packet is relayed by nat server, we assume it is from a firewalled peer
		// or i am a firewalled peer.
		PacketINFO( const PInetAddr	& inAddr, const PInetAddr	& reserved_addr, 
			S32 socket_descriptor, const PPacketBase*	inPacket);

		// members
	public:

		PacketINFO*		halfClone()const;

		B8				isFail2Transit()const;
		
		void			setFail2Transit( B8	fail_or_not );

		PAddressable	getAddressable( )const;

		static		PacketINFO* getInstance() ;

		// public address
		PInetAddr		addr;

		S32				socket;

		// reserved/private address. added by tuxuping 2008-9-24
		PInetAddr		r_addr;

		const PPacketBase*		packet;

	private:
		// is behind firewall. 		 added by tuxuping 2008-9-24		
		U8				m_cNetStatus;

	};


	typedef std::queue < const PacketINFO* >	InPacketQueue;

}

NETWORK_EXPORT void deletePktInfo(const Network::PacketINFO* p);

#endif // PACKET_INFO_H

