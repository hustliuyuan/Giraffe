#ifndef __GPAXOSCOMM__H__
#define __GPAXOSCOMM__H__

#include "GPaxosProtocol.h"
class GMSPktParser;
class Paxos;

class GPaxosComm
{
public:
	GPaxosComm(GPaxosProtocol * pPaxos);
	~GPaxosComm(void);
	void tcpSend( const PInetAddr & inAddr ,  const PPacketBase   * inPacket );
	void udpSend( const PInetAddr & inAddr , const PPacketBase * inPacket );
	void setAppTerminate();
	Network::PTransmitter * getTransmitter();

private:
	Network::PTransmitter *m_pTransmitter;
	GPaxosProtocol *m_pPaxos;
	IPacketHandlerTask *m_handlerTask;
	GMSPktParser *m_pPktParser;
	PPlatTransThreadFunc *m_pPlatTransThreadFunc;
	boost::thread_group m_thread_groups;
};
#endif //__GMSCOMM__H__
