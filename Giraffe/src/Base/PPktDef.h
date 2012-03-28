#ifndef __GIRAFFE_PACKET_DEFINE_H__
#define __GIRAFFE_PACKET_DEFINE_H__

namespace PMyPktDef
{
	// major type :  between 1 and 20.
	enum MajorType
	{
		MY_MAJOR_PKT = 14,
		HTTP_PKT = 15,
	};

	enum SubType
	{	
		//paxos protocol packets
		GRF_PAXOS_EVENT									= 50,
		GRF_BROADCAST_EVENT							= 51,
		GRF_PAXOS_KEEPALIVE_LEADER_REQ							= 60,
		GRF_PAXOS_KEEPALIVE_LEADER_RES							= 61,
		GRF_LEADER_LOCATION_REQ					= 62,
		GRF_LEADER_LOCATION_RES					= 63,
		GRF_WATCHER_EVENT						= 64,
		QUIT_APPLICATION						= 100,

		//coordination service  packets
		GRF_OPEN_REQ								= 101,
		GRF_OPEN_RES								= 102,
		GRF_CLOSE_REQ							= 103,
		GRF_EXITS_REQ							= 104,
		GRF_EXITS_RES							= 105,
		GRF_ATTR_GET_REQ							= 106,
		GRF_ATTR_GET_RES							= 107,
		GRF_ATTR_SET_REQ							= 108,
		GRF_ATTR_SET_RES							= 109,
		GRF_LOCK_REQ								= 110,
		GRF_LOCK_RES								= 111,
		GRF_RELEASE_REQ							= 112,
		GRF_RELEASE_RES							= 113,
		GRF_MKDIR_REQ							= 115,
		GRF_MKDIR_RES							= 114,
		GRF_READ_DIR_REQ							= 116,
		GRF_READ_DIR_RES							= 117,
		GRF_UNLINK_REQ							= 118,
		GRF_UNLINK_RES							= 119,
		GRF_KEEPALIVE_REQ						= 120,
		GRF_KEEPALIVE_RES						= 121,
		GRF_DIR_ATTR_SET_REQ						= 99,
		GRF_DIR_ATTR_SET_RES						= 123,
		GRF_DIR_ATTR_GET_REQ						= 124,
		GRF_DIR_ATTR_GET_RES						= 125,
		GRF_CLOSE_RES							= 126,
		GRF_DELETE_FILE_REQ						= 127,
		GRF_DELETE_FILE_RES						= 128,
	};

	enum HTTP_MINOR{
		STORE_ADDRESS_REQ = 71,
	};

	String   type2String( U8 type, U8 subtype );
};

#endif // __GIRAFFE_PACKET_DEFINE_H__
