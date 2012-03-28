// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#include "common/Header.h"
#include "network/PacketINFO.h"
#include <sstream>
#include "os/PUtils.h"
//#include "win32.h"

namespace Network {

	PacketINFO::PacketINFO()  // added by tu. 2010.8.29
	{
			socket = -1;
			packet = NULL;
			m_cNetStatus = 0 ; 
	}
	
	std::string		PacketINFO::toString() const
	{
		std::ostringstream s;
		s<<" addr:"<< addr.toString();
		s<<" r_addr: "<< r_addr.toString();
		s<<" sockid:"<< socket ;
		s<<" is_fail2transit:"<< (m_cNetStatus?" true":"false");
		
		char temp_str[2048]={0};
		packet->toString(temp_str);
		s<< temp_str;
		return s.str();
	}

	U32	PacketINFO::toString(S8 * to_str) const
	{
		
	
		S8 temp_str[2048]={0};
		packet->toString(temp_str);
		std::ostringstream s;
		s<<" addr:"<< PUtils::long2Addr(addr.getAddr())<<" :"<< addr.getPort();
		s<<" r_addr: "<< PUtils::long2Addr(r_addr.getAddr())<<" :"<< r_addr.getPort();
		s<<" sockid:"<< socket ;
		s<<" is_fail2transit:"<< (m_cNetStatus?" true":"false");		
		s<< temp_str;
		String str1 = s.str();
		memcpy(to_str, str1.c_str(), str1.size());
		to_str[str1.size()] = '\0';
		return str1.size()+1;

	}


	PacketINFO::PacketINFO( const PInetAddr	& inAddr, S32 socket_descriptor,	
		const PPacketBase*	inPacket)
	{

		addr = inAddr;
		packet = inPacket;
		socket = socket_descriptor;
		m_cNetStatus = PAddressable::nNAT_nFIREWALL_nUPNP;
		setFail2Transit(false);
	}
	PacketINFO::PacketINFO( const PInetAddr	& inAddr, const PInetAddr	& reserved_addr, 
		 S32 socket_descriptor, const PPacketBase*	inPacket)
	{
		addr = inAddr;
		packet = inPacket;
		socket = socket_descriptor;
		r_addr = reserved_addr;
		
		m_cNetStatus = PAddressable::nNAT_nFIREWALL_nUPNP;

		setFail2Transit(true);		
	}

	PAddressable	PacketINFO::getAddressable( )const
	{
		PAddressable addr_able = PAddressable( addr.getAddr(), 
			addr.getPort(), r_addr.getAddr(),r_addr.getPort(), m_cNetStatus);
		return addr_able;
	}

	B8	PacketINFO::isFail2Transit()const
	{
		return 0 != (m_cNetStatus & 8);
	}
	void  PacketINFO::setFail2Transit( B8	fail_or_not )
	{
		m_cNetStatus |= ( fail_or_not ? 8 :0 );
	}

	PacketINFO * PacketINFO::getInstance() 
	{
		static PacketINFO pinfo;
		return &pinfo;
	}

	PacketINFO*		PacketINFO::halfClone()const
	{
		PacketINFO * info = new PacketINFO(addr,r_addr,socket,NULL);
		info->setFail2Transit(isFail2Transit());
		LOG_DEBUG("PacketINFO","halfClone(): copied "<<toString());
		return info;
	}

}

void deletePktInfo(const Network::PacketINFO* p)
{
	delete p;
}