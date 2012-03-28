#include "common\Header.h"
#include "ui/IUIMsg.h"

IUIReqMsgBase::IUIReqMsgBase(U32 req_type)
{
	m_iReqType = req_type;
}

IUIReqMsgBase::~IUIReqMsgBase(void)
{
}

IUIAckMsgBase::IUIAckMsgBase(U32 ack_type)
{
	m_iAckType = ack_type;
}

IUIAckMsgBase::~IUIAckMsgBase(void)
{
}