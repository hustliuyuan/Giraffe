#ifndef __GIRAFFE_MS_COMM__H__
#define __GIRAFFE_MS_COMM__H__

class SessionManager;
class GMSPktParser;

class GMSComm
{
public:
	GMSComm(SessionManager * pMaster);
	~GMSComm(void);
	void tcpSend( const PInetAddr & inAddr ,  const PPacketBase   * inPacket );
	void udpSend( const PInetAddr & inAddr , const PPacketBase * inPacket );
	void setAppTerminate();
	Network::PTransmitter * getTransmitter();

private:
	Network::PTransmitter *m_pTransmitter;
	SessionManager *m_pMaster;
	IPacketHandlerTask *m_handlerTask;
	GMSPktParser *m_pMsPktParser;
	PPlatTransThreadFunc *m_pPlatTransThreadFunc;
	boost::thread_group m_threadGroups;
};
#endif //__GIRAFFE_MS_COMM__H__
