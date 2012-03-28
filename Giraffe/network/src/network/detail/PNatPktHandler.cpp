// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#include "common/Header.h"
#include "network/detail/PNatPkthandler.h"

#include "config/PSettings.h"
#include "packet/nats/NATPacket.h"
#include "packet/PPacketFactory.h"

namespace Network { namespace Detail {

	PNatPktHandler::PNatPktHandler(INatTransImpl *  trans, const PInetAddr & natserver)
	{
		// 
		this->m_pTrans = trans;
		this->m_oNatServerAddr = natserver;
		
	}

	PNatPktHandler::~PNatPktHandler(void)
	{
		m_oGuidAddrMap.clear();
		m_oLastNatReqTimeMap.clear();

		WaitingConnectMap::const_iterator it = this->m_oWaitingConnectMap.begin();
		for ( ; it!= this->m_oWaitingConnectMap.end(); ++it )
		{	
			const PPacketBase * waiting_packet = it->second; 
			delete waiting_packet;
		}
		m_oWaitingConnectMap;
	}

	void	PNatPktHandler::addWaitingItem( const PAddressable &  remote_addr, const PPacketBase * p ) {

		//WaitingConnectMap::const_iterator it = this->m_oWaitingConnectMap.find( remote_addr ) ;

		//assert(it == this->m_oWaitingConnectMap.end());

		this->m_oWaitingConnectMap.insert( std::pair<PAddressable,const PPacketBase*>( remote_addr, p ) );
		char temp_str[2048]={0};
		p->toString(temp_str);
		LOG_DEBUG( "PNatPktHandler", "addWaitingItem(): addr:" << remote_addr.toString()
			<<" pkt: "<<temp_str);

	}

	void	PNatPktHandler::removeWaitingItem( const PAddressable &  remote_addr )//, const PPacketBase  * packet )
	{
		WaitingConnectMap::const_iterator it = this->m_oWaitingConnectMap.find( remote_addr ) ;

		// this function is invoked by NAT_ACK and NAT_TEST_ACK. 
		// may be it is already removed before, so we cannot put a 
		// dissertation here . tuxuping 2008-9-5
		//assert(it != this->m_oWaitingConnectMap.end());
		//if ( isMapped( remote_addr))
		//{
			this->m_oWaitingConnectMap.erase( remote_addr ) ;
			LOG_DEBUG( "PNatPktHandler", "removeWaitingItem():" << remote_addr.toString());
		//}

	}


	void	PNatPktHandler::addMappedItem( const PAddressable &  remote_addr, const PInetAddr &  inAddr ){

		//assert( ! isExistDestItem( remote_addr )  ) ;
		// this asseration is removed by tuxuping, because addDestItem 
		// can be invoked by two 
		// functions when receiving NAT_TEST_REQ and NAT_ACK. so 
		// we can not assure it doest not exist before. 2008-9-5.

		this->m_oGuidAddrMap.insert( std::pair< PAddressable, PInetAddr > ( remote_addr, inAddr ) );
		LOG_DEBUG( "PNatPktHandler", "addMappedItem():" << remote_addr.toString()
			<<" mapto: "<<inAddr.toString()	);		

	}

	void	PNatPktHandler::removeNatReqTimeItem( const PAddressable &  remote_addr  )
	{
		LastNatReqTimeMap::iterator it = m_oLastNatReqTimeMap.find(remote_addr);
		if( it != m_oLastNatReqTimeMap.end() )
		{
			m_oLastNatReqTimeMap.erase(remote_addr);
			LOG_DEBUG( "PNatPktHandler", "removeNatReqTimeItem(): " << remote_addr.toString());
		}

	}

	void	PNatPktHandler::removeMappedItem( const PAddressable &  remote_addr ){

		assert( isMapped( remote_addr ) ) ;

		this->m_oGuidAddrMap.erase( remote_addr );
		LOG_DEBUG( "PNatPktHandler", "removeDestItem():" << remote_addr.toString()
			);

	}

	const   PInetAddr  &  PNatPktHandler::getMappedItem( const PAddressable &  remote_addr )  const {

		assert( isMapped( remote_addr )  ) ;

		GuidAddrMap::const_iterator it = this->m_oGuidAddrMap.find(remote_addr ) ;

		return it->second;

	}

	//B8	PNatPktHandler::reliableSend( const PInetAddr &  inAddr, const PPacketBase * inPacket ){
	//
	//	return PBaseTransmitter::reliableSend( inAddr, inPacket ) ;
	//
	//}

	B8 PNatPktHandler::isMapped( const PAddressable &  remote_addr ) const {

		GuidAddrMap::const_iterator it = this->m_oGuidAddrMap.find(remote_addr ) ;

		if( it == this->m_oGuidAddrMap.end() ) {

			return false ;
		}
		return true;
	}

	B8	PNatPktHandler::updateLastNatReqTime( const PAddressable &  remote_addr )
	{
		S32 nat_send_time = PUtils::seconds();
		LastNatReqTimeMap::iterator it = m_oLastNatReqTimeMap.find(remote_addr);
		if( it == m_oLastNatReqTimeMap.end() )
		{
			m_oLastNatReqTimeMap[remote_addr].time = nat_send_time;
			LOG_DEBUG( "PNatPktHandler", "updateLastNatReqTime():" << remote_addr.toString()
				<< " time:"<< PUtils::time2String(nat_send_time));
			return true;
		}
		U32 last_time = m_oLastNatReqTimeMap[remote_addr].time;
		if(  nat_send_time - last_time > NAT_REQ_SEND_INTERVAL )// every 2 seconds, we try one nat_req.
		{
			m_oLastNatReqTimeMap[ remote_addr ].counts += 1; //added by tuxuping 2008-9-26.
			m_oLastNatReqTimeMap[remote_addr].time = nat_send_time;
			LOG_DEBUG( "PNatPktHandler", "updateLastNatReqTime():" << remote_addr.toString()
				<< " last_time:"<< PUtils::time2String(last_time)
				<< " now_time:"<< PUtils::time2String(nat_send_time));
			return true;
		}
		return false;
	}

	//TODO: multiple trackers.
	B8	PNatPktHandler::isMappedItemTimeout( const PAddressable &  remote_addr ) const 
	{
		LastNatReqTimeMap::const_iterator it = m_oLastNatReqTimeMap.find(remote_addr);
		if( it == m_oLastNatReqTimeMap.end() )
		{
			return false;
		}
		if( it->second.counts >=  MAX_NAT_REQ_SEND_TIMES  )// two times , we will deem it times
		{
			return true;
		}
		return false;
	}


	void	PNatPktHandler::handleNatTestReq( const PacketINFO *  info )
	{
		const PPacketBase * packet = info->packet;	

		const PNATTestReqPkt * transreq = (const PNATTestReqPkt *) packet ;

		const PAddressable & from = transreq->getSender();	

		PInetAddr addr ;

		addr.set(from.getGIP() , from.getGPort()) ; 

		const PNATTestAckPkt * ack =  new PNATTestAckPkt( getMyAddressable() );

		this->addMappedItem(from, addr );

		// udp here it is.
		m_pTrans->unreliableSend( addr, ack );

		return ;

	}

	// udp nat trans
	void	PNatPktHandler::handleNatTestAck( const PacketINFO *  info )
	{
		// send waiting packet
		handleNatAck(info);

	}

	void	PNatPktHandler::handleNatNotify( const PacketINFO *  info )
	{
		const PPacketBase * packet = info->packet;	

		const PNATNotifyPkt * transreq = (const PNATNotifyPkt *) packet ;

		const PInetAddr & dest_addr = transreq->getRequiredAddr();

		// TODO: the position of two guid.
		const PNATAckPkt * transit_res = new PNATAckPkt( 
			getMyAddressable() );

		// TODO: get the remote addr.
		//const PInetAddr  from_addr ;//= transreq->getFromGuid()->getGAddr();

		// udp here it is
		m_pTrans->unreliableSend( dest_addr, transit_res);

		return ;

	}

	// implemented by tuxuping , 2008-9-25.
	void	PNatPktHandler::handleRelay( PacketINFO *  info )
	{
		const PNATRelayPkt * pkt = (const PNATRelayPkt *)(info->packet);
		const PPacketFactory & instance = PPacketFactory::getInstance();
		const PPacketBase * pbase = instance.createPacket( 
			pkt->getDataBuffer(), pkt->getRelayedPktSize() );
		if( pbase ==NULL )
		{
			LOG_ERROR("PNatPktHandler","PNatPktHandler(): relay payload error"<< info->toString());
			return ;
		}

		info->packet = pbase;

		info->addr = PInetAddr( pkt->getSrcAddressable().getGIP(),
			pkt->getSrcAddressable().getGPort()); 
		info->r_addr = PInetAddr( pkt->getSrcAddressable().getRIP(),
			pkt->getSrcAddressable().getRPort()); 
		//info->setFirewalled( true );
		info->setFail2Transit( true );

		delete pkt; // let it re enterinto the packet processing cycle.
		this->handlePkt( info );
	}

	void	PNatPktHandler::relayWaittingPkts(const PAddressable &  remote_addr, 
		const PInetAddr & nat_addr )
	{
		//
		WaitingConnectMap::const_iterator it = this->m_oWaitingConnectMap.find( remote_addr );

		for ( ; it!= this->m_oWaitingConnectMap.end(); ++it )
		{	
			const PPacketBase * waiting_packet = it->second;

			const PAddressable & myaddressable = getMyAddressable();

			const PNATRelayPkt *  p = new PNATRelayPkt( myaddressable,
				remote_addr, *waiting_packet);

			delete waiting_packet;

			m_pTrans->unreliableSend( nat_addr, p);			

		}

		removeWaitingItem( remote_addr );
	}

	void	PNatPktHandler::handleNatAck( const PacketINFO *  info )
	{

		const PPacketBase * packet = info->packet;

		const PInetAddr &  inAddr = info->addr;

		const PNATAckPkt *  respacket = ( const PNATAckPkt *  )packet;

		const PAddressable & from_guid = respacket->getSender();

		addMappedItem(from_guid, inAddr);	


		//
		WaitingConnectMap::const_iterator it = this->m_oWaitingConnectMap.find( from_guid );

		U32 num = this->m_oWaitingConnectMap.count(from_guid);
		U32 i = 0;

		for ( ; (i< num) && ( it!= this->m_oWaitingConnectMap.end()); ++it, ++i )
		{	
			const PPacketBase * waiting_packet = it->second; 

			//*if( natack->isUdp() )
			//{
			// this is a udp trans.


			m_pTrans->unreliableSend( inAddr, waiting_packet);
			//*/}
			/*{

			// this is a tcp trans.
			addTcpDestItem( from_guid, inAddr );
			PBaseTransmitter::reliableSend( inAddr, waiting_packet );
			}//*/

		}
		removeWaitingItem( from_guid );//, waiting_packet );
		removeNatReqTimeItem( from_guid); // added by tuxuping, 2008-12-1
	}


	const PAddressable &	PNatPktHandler::getMyAddressable()const 
	{
		//return m_oMyAddressable;
		return m_pTrans->getMyAddressable();
	}

	void	PNatPktHandler::setPktHandler( IPacketHandler * hndler)
	{
		this->m_pHandler = hndler;
	}

	void	PNatPktHandler::checkTransTimeout()
	{

		LastNatReqTimeMap::const_iterator it = m_oLastNatReqTimeMap.begin();

		for( ; it != m_oLastNatReqTimeMap.end();  )
		{	
		
			//LastNatReqTimeMap::const_iterator tempit =it;

			PAddressable addressable = it->first;

			B8   item_snd_long_time  = updateLastNatReqTime ( addressable ); 

			if( ! item_snd_long_time )
			{
				continue;
			}

			

			if( it->second.counts <  MAX_NAT_REQ_SEND_TIMES  ) // two times , we will deem it timeout
			{
				++ it ;
				sendNatTrans(addressable, getMyAddressable());
				
			}else 
			{
				++ it ;
				relayAndClear( addressable, m_oNatServerAddr );	
				
			}
			 
		}
	}

	void	PNatPktHandler::relayAndClear(const PAddressable & remote_addrsable, const PInetAddr & natserver)
	{
		// a. relay all the packets in the waiting list.		
		relayWaittingPkts(remote_addrsable,natserver);

		// b. let the remote_addr map to nat server. then all packets latter for 
		// this address will be relayed by the nat server.
		addMappedItem(remote_addrsable, natserver);	

		// c. remove nat req time item. 
		removeNatReqTimeItem(remote_addrsable);

		return ;
	}

	void	PNatPktHandler::sendNatTrans(const PAddressable & remote_addressable,
		const PAddressable & myaddressable) 
	{
		PInetAddr  tempaddr;
		tempaddr.set(remote_addressable.getGIP(), remote_addressable.getGPort());

		// 1. remote is private or remote has firewalls, we must traverse.
		if(  remote_addressable.getGIP() != remote_addressable.getRIP()
			||remote_addressable.hasFireWall())						
		{	
			PInetAddr me( myaddressable.getGIP(), myaddressable.getGPort());
			const PPacketBase*  req = new PNATReqPkt( me, tempaddr);
			m_pTrans->unreliableSend( m_oNatServerAddr, req );
			//notice : not return here . tuxuping. 2008-10-7
		}
		// 2. ( only I am public and not firewalled, there is no need to send testreqpkt,
		//    other cases need to send test) .
		if( !( myaddressable.getGIP() == myaddressable.getRIP() 
			&& !myaddressable.hasFireWall() ))
		{	
			const PPacketBase*  req = new PNATTestReqPkt( myaddressable );
			m_pTrans->unreliableSend( tempaddr, req );						
		}		
	}

	void	PNatPktHandler::handlePkt( const PacketINFO * pktinfo )
	{
		U8 type = pktinfo->packet->getType();
		// nat pkt.
		if (type == PPktDef::PEER_TO_NAT_SERVER)
		{
			U8	subtype = pktinfo->packet->getSubType();
			switch ( subtype ) 
			{		
			case PPktDef::NAT_NOTIFY:
				this->handleNatNotify(pktinfo);
				break;

			case PPktDef::NAT_TEST_REQ:
				this->handleNatTestReq(pktinfo);
				break;

			case PPktDef::NAT_TEST_ACK:
				this->handleNatTestAck(pktinfo);			
				break;

			case PPktDef::NAT_ACK:
				this->handleNatAck(pktinfo);
				break;
			case PPktDef::NAT_RELAY:
				this->handleRelay(( PacketINFO*)pktinfo);
				break;

			default:
				assert(0);

			}			
						
			if( subtype != PPktDef::NAT_RELAY ) 
			{
				delete pktinfo->packet; // added by tuxuping 2008-9-5
				delete pktinfo;  // added by tuxuping 2008-9-5
			}
			
		}else 
		{
			this->m_pHandler->handlePkt( pktinfo);
			return ;
		}
	}
} // namespace detail
} // namespace network
