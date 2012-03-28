#ifndef __GMS_PACKET_HANDLER_H__
#define __GMS_PACKET_HANDLER_H__

#include "Base/GHeader.h"
#include "SessionManager.h"

class GMSPktHandler : public IPacketHandlerTask
{
public:
	typedef Network::PacketINFO PacketINFO;
	typedef Network::PInetAddr  PInetAddr;
	typedef Network::PAddressable PAddressable;

	GMSPktHandler(Network::PTransmitter	*transmiter/*, IAppTerminate * iappquit */);
	GMSPktHandler(Network::PTransmitter	*ITransmitter_task, SessionManager* pMs/*,OraDBConnectPool* connPool*/);	

	virtual ~GMSPktHandler();

	virtual		void			handlePkt( const PacketINFO * packetInfo );

	void						maintain( );

	virtual		void			onRun( );

	virtual		std::string		getName()const;	

	void                        setAppTerminate(IAppTerminate * iappquit);

private:

	void						handleInternalQuitApplication( const PacketINFO * pktinfo);
	void						handleFileAttrGetReq( const PacketINFO * pktinfo);
	void						handleFileAttrSetReq( const PacketINFO * pktinfo);
	void						handleFileCloseReq( const PacketINFO * pktinfo);
	void						handleDirAttrGetReq( const PacketINFO * pktinfo);
	void						handleDirAttrSetReq( const PacketINFO * pktinfo);
	void						handleNodeExitsReq( const PacketINFO * pktinfo);
	void						handleKeepAliveReq( const PacketINFO * pktinfo);
	void						handleFileLockReq( const PacketINFO * pktinfo);
	void						handleDirMakeReq( const PacketINFO * pktinfo);
	void						handleFileOpenReq( const PacketINFO * pktinfo);
	void						handleDirReadReq( const PacketINFO * pktinfo);
	void						handleLockReleaseReq( const PacketINFO * pktinfo);
	void						handleNodeDelReq( const PacketINFO * pktinfo);
	void						handleDelFileReq( const PacketINFO * pktinfo);
	void						handlePaxosEvent( const PacketINFO * pktinfo);
	void						handlePaxosKeepAliveReqEvent( const PacketINFO * pktinfo);
	void						handlePaxosKeepAliveResEvent( const PacketINFO * pktinfo);
	void						handleLeaderLocationReq( const PacketINFO * pktinfo);
	void						handleUnlockReq( const PacketINFO * pktinfo);
	
	
private:

	// my local ip address.
	U32							m_iIP;
	IAppTerminate	*			m_pAppterminate;
	PTransmitter *				transmiter;
	SessionManager*						m_pMaster;

	U32							m_curTime;
	U32							m_iInterval_time;
};
#endif /*__GMS_PACKET_HANDLER_H__*/


