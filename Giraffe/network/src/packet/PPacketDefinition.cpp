// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


// tuxuping . 2008-6-20
#include "common/Header.h"
#include "packet/PPacketDefinition.h"
//#include "win32.h"
namespace PPktDef
{
	// major type :  between 1 and 20.

	String   type2String( U8 type , U8 subtype )
	{
		String s;
		switch( type )
		{

		case PEER_TO_NAT_SERVER :
			s.append("ns ");
			break;
		
		case INTERNAL_COMMUNICATION:
			s.append("internal ");
			break;

		default:
			s.append("undefine ");
			//LOG_DEBUG
			assert(0);
			break;

		}


		if( type ==  PEER_TO_NAT_SERVER )
		{
			switch( subtype ) {

		case NAT_REQ:
			s.append("NAT_REQ");
			break;

		case NAT_ACK:
			s.append("NAT_ACK");
			break;

		case NAT_TEST_REQ:
			s.append("NAT_TEST_REQ");
			break;

		case NAT_TEST_ACK:
			s.append("NAT_TEST_ACK");
			break;

		case NAT_NOTIFY:
			s.append("NAT_NOTIFY");
			break;

		case NAT_HEARTBEAT:
			s.append("NAT_HEARTBEAT");
			break;

		case NAT_RELAY:
			s.append("NAT_RELAY");
			break;

		default:
			LOG_DEBUG("Packet"," errNS");
			assert(0);

			}
			return s;
		}

		if( type ==  INTERNAL_COMMUNICATION )
		{
			switch( subtype ) {

		case QUIT_APPLICATION:
			s.append("QUIT_APPLICATION");
			break;
		case NO_DELETE:
			s.append("NO_DELETE");
			break;

		default:
			LOG_DEBUG("Packet"," errinternal");
			assert(0);
			}
			return s;
		}

		return String();
	}
}

