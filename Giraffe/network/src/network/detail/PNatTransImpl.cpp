// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#include "common/Header.h"
#include "network/detail/PNatTransImpl.h"

#include "common/PTime.h"
#include "config/PSettings.h"
#include "network/detail/PTransImplTcp.h"
#include "packet/nats/NATPacket.h"


namespace Network { namespace Detail {

	PNatTransImpl::PNatTransImpl()
	{
		m_pTransImpl = (PTransImplTcp*)new PTransImplTcp();
		PTransImplTcp *  tcpImpl = (PTransImplTcp*)m_pTransImpl;
		tcpImpl->initialize();
		
		const PSettings & settings = PSettings::getInstance();
		String nat_server_ip_str = settings.getConfig("nat-server-ip").m_value.strvalue;
		U16 nat_server_port = settings.getConfig("nat-server-port").m_value.intvalue;

		m_oNatServerAddr.set( nat_server_ip_str.c_str(), nat_server_port);

		assert( m_oNatServerAddr.getAddr()!= 0xffffffff);// 
		LOG_INFO("PNatTransImpl","ctor: set nat server addr:"<<m_oNatServerAddr.toString() );

		m_pNatPktHandler = new PNatPktHandler(this, m_oNatServerAddr);
		// get seconds, 

		m_iHeartbeatInterval =  settings.getConfig("heartbeat-interval-seconds").m_value.intvalue; 

		m_iTransTimeoutInterval = settings.getConfig("trans-timeout-interval-seconds").m_value.intvalue; 

		m_iLastHeartbeatTime = PUtils::seconds();

		this->m_iLastTransTimeoutTime = m_iLastHeartbeatTime;

		m_bIsFirstRun = true;
	}

	PNatTransImpl::PNatTransImpl(U16 tcpport, U16 udpport)
	{

		m_pTransImpl = (PTransImplTcp*)new PTransImplTcp();
		PTransImplTcp *  tcpImpl = (PTransImplTcp*)m_pTransImpl;
		tcpImpl->initialize(tcpport,udpport);
		
		const PSettings & settings = PSettings::getInstance();
		String nat_server_ip_str = settings.getConfig("nat-server-ip").m_value.strvalue;
		U16 nat_server_port = settings.getConfig("nat-server-port").m_value.intvalue;

		m_oNatServerAddr.set( nat_server_ip_str.c_str(), nat_server_port);
		
		assert( m_oNatServerAddr.getAddr()!= 0xffffffff);// 
		LOG_INFO("PNatTransImpl","ctor: set nat server addr:"<<m_oNatServerAddr.toString() );

		m_pNatPktHandler = new PNatPktHandler(this, m_oNatServerAddr);

		// get seconds, 

		m_iHeartbeatInterval =  settings.getConfig("heartbeat-interval-seconds").m_value.intvalue; 

		m_iTransTimeoutInterval = settings.getConfig("trans-timeout-interval-seconds").m_value.intvalue; 

		m_iLastHeartbeatTime = PUtils::seconds();

		this->m_iLastTransTimeoutTime = m_iLastHeartbeatTime;

	}

	PNatTransImpl::~PNatTransImpl(void)
	{
		delete m_pTransImpl;
		delete m_pNatPktHandler;
	}

	void	PNatTransImpl::reliableSend( const PAddressable &  remote_addr , const PPacketBase * inPacket ) {

		//return tcpSend( remote_guid, inPacket ) ;
		//this->m_pTransImpl->reliableSend(
		LOG_ERROR("PNatTransImpl","send to PAddressable is not implemented yet:)");
		assert(0);

	}

	void	PNatTransImpl::unreliableSend( const PAddressable &  remote_addr , const PPacketBase * inPacket ){
		
		//
		// 1. there is an map item in previous communiction.
		// so, we need not to nat transit.
		if( this->m_pNatPktHandler->isMapped(remote_addr) ) 
		{
			const PInetAddr & addr = m_pNatPktHandler->getMappedItem(remote_addr );

			// 1a.if the mapped address is not natserver, we send it directly
			if( !(addr == m_oNatServerAddr ))
			{
				return m_pTransImpl->unreliableSend( addr, inPacket );

			}else // 1b. else we let the nat server relay.
			{				
				const PNATRelayPkt *  relay_wrapper = new PNATRelayPkt( getMyAddressable() ,remote_addr,  *inPacket);
				delete inPacket;
				m_pTransImpl->unreliableSend( m_oNatServerAddr, relay_wrapper );
				return ;
			}			
		}

		PInetAddr  tempaddr;
		tempaddr.set(remote_addr.getGIP(), remote_addr.getGPort());

		// 2. remote is public and no fireall
		if ( remote_addr.getRIP() == remote_addr.getGIP()
			&& !remote_addr.hasFireWall() ) 
		{
			return m_pTransImpl->unreliableSend( tempaddr, inPacket);
		}

		const PAddressable &myaddr = this->getMyAddressable();
		//3. we are in same reserved network;
		if( remote_addr.getGIP() == myaddr.getGIP())
		{
			PInetAddr  remote_r_addr;
			remote_r_addr.set(remote_addr.getRIP(), remote_addr.getRPort());
			return m_pTransImpl->unreliableSend( remote_r_addr, inPacket);
		}

		// 4. put this packets to the waiting list.
		m_pNatPktHandler->addWaitingItem(remote_addr, inPacket);

		//const PInetAddr & remote_nat_server = getNatServerAddr( tempaddr );

		B8   item_snd_long_time  = m_pNatPktHandler->updateLastNatReqTime (remote_addr); 

		B8	 item_time_out = false;

		// 5. to avoid that we just send nat_req packet and we send relay packet at once.
		// so we found last transit is timeout. we send this relay packet.
		if( item_snd_long_time )
		{
			item_time_out = m_pNatPktHandler->isMappedItemTimeout(remote_addr);
			if( item_time_out )
			{
				// relay all packets for the addressable by a nat server,
				// and adjust state of natpkthandler.
				m_pNatPktHandler->relayAndClear(remote_addr, m_oNatServerAddr);
				return ;
			}
		}

		// added by tuxuping 2008-9-5. why I add such variable, 
		// please refere to declaration "addWaitingItem".

		// 6. send nat trans to remote addressable.
		if( item_snd_long_time )
		{
			this->m_pNatPktHandler->sendNatTrans( remote_addr, myaddr);
		}

		return ;
		// 7. synmatric nat ( it must timeout when trans ), we have relayed them to natserver

	}

	void	PNatTransImpl::reliableSend( U32 inSocket,  const PPacketBase   * inPacket )
	{
		m_pTransImpl->reliableSend(inSocket,inPacket);
	}


	void	PNatTransImpl::reliableSend( const PInetAddr & inAddr ,  const PPacketBase   * inPacket )
	{
		m_pTransImpl->reliableSend(inAddr,inPacket);
	}

	void	PNatTransImpl::unreliableSend( const PInetAddr & inAddr , const PPacketBase * inPacket )
	{
		m_pTransImpl->unreliableSend(inAddr,inPacket);
	}


	const PInetAddr &		PNatTransImpl::getNatServerAddr( const PInetAddr &  addr ) const {

		return this->m_oNatServerAddr;

	}
	const PAddressable &	PNatTransImpl::getMyAddressable()const 
	{
		return m_oMyAddressable;
	}

	void		PNatTransImpl::onRun()
	{
		m_pTransImpl->onRun();

		// because m_oMyAddressable is set after we received the ls_login_ack.
		// so we can skip this condition and send heartbeat to ns
		if( ! ( m_oMyAddressable == PAddressable::getNull()))
		if( ( m_oMyAddressable.getRIP() == m_oMyAddressable.getGIP())
			&& (!m_oMyAddressable.hasFireWall()) // added by tuxuping 2008-9-26
			)
			return ;

		// let the peer send hearbeat right now. added by tuxuping 2008-9-26

		sendHeartbeat();
		handleTransTimeout();

	}


	void		PNatTransImpl::sendHeartbeat() 
	{
		if ( m_bIsFirstRun  ) 
		{
			// heart beat to ns;
			const PNATHeartBeatPkt* nspkt = new PNATHeartBeatPkt();
			this->m_pTransImpl->unreliableSend( m_oNatServerAddr, nspkt);
			m_bIsFirstRun   = false;
			return;
		}

		S32 curtime = PTime::getLowResolutionSeconds();

		if( curtime - this->m_iLastHeartbeatTime < m_iHeartbeatInterval )
		{
			return ;	
		}

		// heart beat to ns;
		const PNATHeartBeatPkt* nspkt = new PNATHeartBeatPkt();				

		this->m_pTransImpl->unreliableSend( m_oNatServerAddr, nspkt);

		m_iLastHeartbeatTime = curtime;
		


	}

	void		PNatTransImpl::handleTransTimeout() 
	{

		S32 curtime = PTime::getLowResolutionSeconds();

		if( curtime - this->m_iLastTransTimeoutTime < m_iTransTimeoutInterval )
		{
			return ;
		}
		m_pNatPktHandler->checkTransTimeout();
		m_iLastTransTimeoutTime = curtime;

	}

	String		PNatTransImpl::getName() const
	{
		return "PNatTransImpl";
	}

	void	PNatTransImpl::handlePkt( const PacketINFO *  info ) 
	{
		assert(0);// no used this function. tuxuping 2008-9-4.
		//if( info->packet->getType() == PPktDef::PEER_TO_NAT_SERVER ) {
		//	this->m_pNatPktHandler->handlePkt(info);
		//	return ;
		//}
		//this->m_pTransImpl->handlePkt(info);
	}

	void	PNatTransImpl::registerPacketHandler( IPacketHandler * packet_handler ) 
	{	
		this->m_pNatPktHandler->setPktHandler( packet_handler);
		// TODO: notice, here we omit the old handler, this may raise 
		// memory leaks, if you invoke registerPacketHandler twice or more times.
		// tuxuping 2008-9-4.
		this->m_pTransImpl->registerPacketHandler(m_pNatPktHandler);		
	}

	void	PNatTransImpl::setMyAddressable(const PAddressable & addr_able)
	{
		LOG_INFO("PNatTransImpl","setMyAddressable(): "<< addr_able.toString());
		this->m_oMyAddressable= addr_able;
	}
	bool	PNatTransImpl::addTcpListener( const String& ip,  U16 port, U32 socket_type )
	{
		Detail::PTransImplTcp * trans = (Detail::PTransImplTcp *)m_pTransImpl;
		U32 ip_int = PUtils::addr2Long(ip.c_str());
		return  trans->addTcpListener(ip_int,port,socket_type);

	}

} // namespace detail
} // namespace network
