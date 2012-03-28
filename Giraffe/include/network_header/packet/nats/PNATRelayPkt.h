// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

//张亚霏2008-09-09加
// tuxuping add srcaddr field into the relaypacket. 
// because the receiver of this packet need to know which sent to him.
// 2008-9-25
#include "packet/PPacketBase.h"
#include "packet/PPacketDefinition.h"
#include "network/PAddressable.h"
#include <vector>

class PNATRelayPkt : public PPacketBase
{
public:
	typedef std::vector<char> BufferType;
	PNATRelayPkt();
	PNATRelayPkt(const Network::PAddressable& SrcAddr, 
		const Network::PAddressable& DestAddr, const PPacketBase& pkt);


	virtual U32			getLength( ) const;
	virtual U32			serialize( S8 *  buf,  S32 offset ) const ;
	virtual U32			deserialize( const S8 * buf, S32 offset ) ;
	virtual String      toString() const ;
	virtual U32		 toString(S8 * str) const;

	// added by tuxuping. skipped the packet length field. 2008-9-24
	U32					getRelayedPktPos()const;
	U32					getRelayedPktSize()const;
	const Network::PAddressable	& getDstAddressable() const ;
	const Network::PAddressable	& getSrcAddressable() const ;

	
	const char*			getDataBuffer()const{return (m_oData.empty())?NULL:&m_oData[0];}
	char*				getDataBuffer(){return (m_oData.empty())?NULL:&m_oData[0];}
	const BufferType&	getData()const{return m_oData;}
	BufferType&			getData(){return m_oData;}

	U32					getGIP()const{return m_oDestAddress.getGIP();}
	U16					getGPort()const{return m_oDestAddress.getGPort();}
private:
	Network::PAddressable
						m_oDestAddress;//包要转给的目标地址
	Network::PAddressable 
						m_oSrcAddress;//包的来源地址 added by tuxuping 2008-9-25
	BufferType			m_oData;//要转给的数据
};
