#ifndef __GCL_PACKET_HANDLER_H__
#define __GCL_PACKET_HANDLER_H__

#include "Base/GHeader.h"

class Session;
class WatcherManager;

class GCLPktHandler : public IPacketHandlerTask
{
public:
	typedef Network::PacketINFO PacketINFO;
	typedef Network::PInetAddr  PInetAddr;
	typedef Network::PAddressable PAddressable;

	GCLPktHandler(Network::PTransmitter	*ITransmitter_task, Session * session/*,OraDBConnectPool* connPool*/);	

	virtual ~GCLPktHandler();

	virtual		void			handlePkt( const PacketINFO * packetInfo );

	void						maintain( );

	virtual		void			onRun( );

	virtual		std::string		getName()const;	

	void                        setAppTerminate(IAppTerminate * iappquit);

	void						handleKeepAliveResApplication(const PacketINFO * pktinfo);
	void						handleMkDirResApplication(const PacketINFO * pktinfo);
	void						handleOpenFileResApplication(const PacketINFO * pktinfo);
	void						handleLockResPkt(const PacketINFO * pktinfo);
	void						handleSetAttrResApplication(const PacketINFO * pktinfo);
	void						handleGetAttrResApplication(const PacketINFO * pktinfo);
	void						handleGetLeaderLocResApplication(const PacketINFO * pktinfo);
	void						handleCloseFileResApplication(const PacketINFO * pktinfo);
	void						handleReadDirResApplication(const PacketINFO * pktinfo);
	void						handleExitsResApplication(const PacketINFO * pktinfo);
	void						handleWatcherEventApplication(const PacketINFO * pktinfo);	
	
	
private:

	void						handleInternalQuitApplication( const PacketINFO * pktinfo);

private:

	// my local ip address.
	U32							m_iIP;
	IAppTerminate	*			m_pAppterminate; 
	PTransmitter *				transmiter;

	U32							m_iInterval_time;
	U32							m_curTime;

	Session *					m_session;
	WatcherManager*				m_watcherManger;
};


#endif /*__GMS_PACKET_HANDLER_H__*/


