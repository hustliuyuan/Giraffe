// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#ifndef  PNAT_PKT_HANDLER_H 
#define PNAT_PKT_HANDLER_H  

#include <map>

#include <string>


#include "network/PInetAddr.h"
#include "common/IPacketHandler.h"
#include "INatTransImpl.h"

#include "SocketINFO.h"


namespace Network { namespace Detail {

using namespace Network;

// added by tuxuping 2006-9-26.
struct NatReqTimeItem{
	S32 time ; // time that last nat_req is sending.
	S32 counts; // how many times that the nat_req is sent out to the same addresable.
	NatReqTimeItem()
	{
		time = 0;
		counts = 0;
	} 
};


typedef std::multimap< PAddressable ,  const PPacketBase * >   WaitingConnectMap; 

typedef std::map< PAddressable ,  PInetAddr >					GuidAddrMap;

typedef std::map< PAddressable,   NatReqTimeItem >				LastNatReqTimeMap;

typedef std::map< std::string, SocketINFO * >					UDPReliableMap;


class PNatPktHandler : public IPacketHandler
{
public:

	PNatPktHandler( INatTransImpl *  trans, const PInetAddr & natserver );

public:
	enum
	{
		// we at most try to send <MAX_NAT_REQ_SEND_TIMES> NAT_REQ_PKT to nat server for a single transit.
		MAX_NAT_REQ_SEND_TIMES = 3, // times;
		// every <NAT_REQ_SEND_INTERVAL> we send one NAT_REQ_PKT to nat server,
		// if we failed to transit at previous efforts.
		NAT_REQ_SEND_INTERVAL = 2, // seconds;
	};

	virtual ~PNatPktHandler(void);	

	virtual void		handlePkt( const PacketINFO * pktinfo );

	// add a <addressable, addr> pair to local map. 
	void				addMappedItem ( const PAddressable &  remote_addr, const PInetAddr &  inAddr );

	void				removeMappedItem( const PAddressable &  remote_addr ); 

	B8					isMapped( const PAddressable &  remote_addr ) const ; 

	// if the mapped item timeout, we need to relay it by the nat server.
	// added by tuxuping 2008-9-26
	B8					isMappedItemTimeout( const PAddressable &  remote_addr ) const ; 

	const PInetAddr & 	getMappedItem( const PAddressable &  remote_addr ) const ;

	// the return value is 'bool'.
	// if we have added a same waiting item just before ( for example 2 seconds )
	// we return false; then we will not send a NAT_REQ or NAT_TEST_REQ ( or it will 
	// send too much NAT_REQ and NAT_TEST_REQ to the server.
	// else we return true. 
	// added and changed by tuxuping, 2008-9-5.
	B8					updateLastNatReqTime( const PAddressable &  remote_addr );

	// add a <guid, packet> pair to local multimap.
	void				addWaitingItem( const PAddressable &  remote_addr, 
		const PPacketBase * packet ) ;	

	void				setPktHandler( IPacketHandler * hndler);

	// added by tuxuping , 2008-10-7.
	void				checkTransTimeout();

	void				relayAndClear(const PAddressable & remote_addrsable, 
		const PInetAddr & natserver);

	void				sendNatTrans(const PAddressable & remote_addressable,
		const PAddressable & myaddressable);

private: 



	void				removeNatReqTimeItem( const PAddressable &  remote_addr  );


	void				relayWaittingPkts( const PAddressable &  remote_addr, 
		const PInetAddr & nat_addr );

	void				removeWaitingItem( const PAddressable &  remote_addr );


	//support those players behind firewall or nosymatric 
	// users we let the nat server to relay there packets.
	// added by tuxuping, 2008-9-10. 
	// this function will modify the PacketINFO * structure;
	void				handleRelay( PacketINFO *  info );

	// udp nat trans
	void				handleNatAck( const PacketINFO *  info );

	// udp nat trans
	void				handleNatNotify( const PacketINFO *  info );	

	// udp nat trans
	void				handleNatTestReq( const PacketINFO *  info );

	// udp nat trans
	void				handleNatTestAck( const PacketINFO *  info );

	const PAddressable &	getMyAddressable()const ;

protected:

	// transmitter;
	INatTransImpl *				m_pTrans;

	GuidAddrMap					m_oGuidAddrMap;

	WaitingConnectMap			m_oWaitingConnectMap;	

	// to limit the number of nat_req packet, it need to record time that 
	// the peer send nat_req last time; tuxuping 2008-9-5.
	LastNatReqTimeMap			m_oLastNatReqTimeMap;

	
	// normal packets is handled by this one. added by tuxuping 2008-9-4
	IPacketHandler	*			m_pHandler;

	// added by tuxuping 2008-10-7
	PInetAddr					m_oNatServerAddr;
		
};
} // namespace detail
} // namespace network

#endif // PNAT_PKT_HANDLER_H
