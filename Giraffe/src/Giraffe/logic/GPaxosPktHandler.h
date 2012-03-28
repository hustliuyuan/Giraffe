#ifndef __GPAXOS_PACKET_HANDLER_H__
#define __GPAXOS_PACKET_HANDLER_H__

#include "Base/GHeader.h"
#include "GPaxosComm.h"
#include "GPaxosProtocol.h"


using namespace std;
using namespace Network;

class GPaxosPktHandler : public IPacketHandlerTask
{
public:
	typedef Network::PacketINFO PacketINFO;
	typedef Network::PInetAddr  PInetAddr;
	typedef Network::PAddressable PAddressable;

	GPaxosPktHandler(Network::PTransmitter	*transmiter/*, IAppTerminate * iappquit */);
	GPaxosPktHandler(Network::PTransmitter	*ITransmitter_task, GPaxosProtocol* pPaxos/*,OraDBConnectPool* connPool*/);	

	virtual ~GPaxosPktHandler();

	virtual		void			handlePkt( const PacketINFO * packetInfo );

	void						maintain( );

	virtual		void			onRun( );

	virtual		std::string		getName()const;	

	void                        setAppTerminate(IAppTerminate * iappquit);

private:

	void						handleInternalQuitApplication( const PacketINFO * pktinfo);
	void						handlePaxosEvent( const PacketINFO * pktinfo);
	void						handlePaxosKeepAliveReqEvent( const PacketINFO * pktinfo);
	void						handlePaxosKeepAliveResEvent( const PacketINFO * pktinfo);
	void						handleBroadcastEvent( const PacketINFO * pktinfo);
	
private:

	// my local ip address.
	U32							m_iIP;
	U16							m_iTcpPort;
	U16							m_iUdpPort;
	IAppTerminate	*			m_pAppterminate;
	PTransmitter *				transmiter;
	GPaxosProtocol*						m_pPaxos;

	U32							m_curTime;
	U32							m_iInterval_time;
};


#endif /*__GMS_PACKET_HANDLER_H__*/


