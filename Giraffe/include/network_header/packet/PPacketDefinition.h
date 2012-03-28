#ifndef   PPKTDEF_H
#define PPKTDEF_H 

// billion Coporation. Ltd
// author: tuxuping. 2009-6-23
// all rights reserved.

/************************************************************************/
/*所有包的类型枚举在这里给出*/ //tuxuping 2008-5-20
/************************************************************************/

enum 
{
	//包头数据的偏移量
	PACKET_LENGTH_OFFSET = 0,//包长度
	PACKET_TYPE_OFFSET = 2,//包类型
	PACKET_SUBTYPE_OFFSET = 3,//包子类型
	PACKET_VERSION_OFFSET = 4,//包版本
	PACKET_CHECKSUM_OFFSET = 6,//校验和
	PACKET_DATA_OFFSET = 8,//包数据

	UDPPACKET_MAXSIZE = 1460,//UDP包的最大长度
	//modified by taozhe 2010.4.17
	//TCPPACKET_MAXSIZE = 4096,
	TCPPACKET_MAXSIZE = 8*1024*1024,//TCP包的最大长度

	CURRENT_PKT_VERSION = 2,//包的当前版本
	COMMON_HEAD_LENGTH = 8,//包头公共长度
};


/*
包的结构：其中, length =“包头的长度”+ “other content的长度”。
-------------------------------------------------------------------
| length(2)|type(1)|subtype(1)|version(2)|checksum(2)|other content|
|------------------------------------------------------------------
|<----------------------包  头---------------------->|
*/


namespace PPktDef
{

	// major type :  between 1 and 20.
	enum PacketType{
		// old is:
		//PEER_TO_NAT_SERVER = 6,  // with nat server.
		//INTERNAL_COMMUNICATION = 13, //added by tuxuping.

		PEER_TO_NAT_SERVER = 1,  // with nat server.
		INTERNAL_COMMUNICATION = 2, //added by tuxuping.		
		USER_DEFINE_MAJOR = 3, 

		MAX_MAJOR_TYPE = 255,
		UNDEFINE_PKT  = 0xff,//未定义的包类型
	};	

	//PEER_TO_NAT_SERVER = 6,  // with nat server.
	// minor type:
	enum {
		USER_DEFINE_MINOR = 20,
	};

	enum{

		NAT_REQ = 60,
		NAT_ACK = 61,
		NAT_TEST_REQ = 62,
		NAT_TEST_ACK =63,
		NAT_NOTIFY = 64,
		NAT_HEARTBEAT = 65,	
		NAT_RELAY = 66,
	};


	//PEER_TO_LOG_SERVER = 7,  // with log server.
	// minor type:
	enum {
		LOG_RESEARCH  = 70,
		LOG_DEBUG = 71,
	};

	// INTERNAL_COMMUNICATION = 13, internal platform communiction with itself. added by tuxuping 2008-9-27
	enum
	{
		QUIT_APPLICATION = 121,
		NO_DELETE = 122, // added.
	};

	NETWORK_EXPORT String   type2String( U8 type, U8 subtype );
	
}

#endif //PPKTDEF_H

