#ifndef __GIRAFFE_MS_COMM__H__
#define __GIRAFFE_MS_COMM__H__

class Session;
class GCLPktParser;

class GCLComm
{
public:
	GCLComm(Session * pSession);
	~GCLComm(void);
	void tcpSend( const PInetAddr & inAddr ,  const PPacketBase   * inPacket );
	void udpSend( const PInetAddr & inAddr , const PPacketBase * inPacket );
	void setAppTerminate();
	Network::PTransmitter * getTransmitter();

private:
	Network::PTransmitter *m_pTransmitter;
	Session *m_pSession;
	IPacketHandlerTask *m_handlerTask;
	GCLPktParser *m_pCLPktParser;
	PPlatTransThreadFunc *m_pPlatTransThreadFunc;
	boost::thread_group m_threadGroups;
};
#endif //__GMSCOMM__H__
