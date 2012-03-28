#pragma once
#include "common/Header.h"

class NETWORK_EXPORT IUIReqMsgBase
{
public:
	IUIReqMsgBase( U32 req_type);
public:
	virtual ~IUIReqMsgBase(void);

	 U32  m_iReqType;
};


class NETWORK_EXPORT IUIAckMsgBase
{
public:
	IUIAckMsgBase(U32 ack_type);
public:
	virtual ~IUIAckMsgBase(void);

	U32  m_iAckType;
};

