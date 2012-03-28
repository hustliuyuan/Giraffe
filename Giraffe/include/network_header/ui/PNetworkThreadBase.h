#pragma once
#include "common/Header.h"
#include "common/PSynQueue.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "IUIMsgHandlerTask.h"
#include "network/PTransmitter.h"
#include "common/IPacketHandlerTask.h"
#include "common/IPacketHandler.h"
#include "config/PIniReader.h"
#include "IUIMsgHandlerTask.h"
#include "common/IPacketParser.h"
#include "common/PPlatTransThreadFunc.h"
// invoke example:
// after implemente compose
// setConfigFile();
// setUIReqMsgContainer()
// 


class NETWORK_EXPORT PNetworkThreadBase
{
public:
	typedef Network::PTransmitter PTransmitter;

	PNetworkThreadBase(void);

	virtual ~PNetworkThreadBase(void);

	// if you have ui msg handler, that is to see, if 
	// this programm has UI interface. you should invoke this function.
	// to let the network can process the ui message from the windows ui.
	// 
	void				setConfigReader( const std::string&  log_file, PIniReader * config_file );

	void				setUIReqMsgContainer(PSynQueue<IUIReqMsgBase* >*);
	
	virtual  void		composeComponent () =  0 ;  	

	static U32			start(LPVOID para);

	void				cleanUp();

private:

	void				startLogInfrastrue(const char *path, S32 log_level);

protected:

	void				configure();


	PSynQueue<IUIReqMsgBase* >*		m_pUIReqMsgQueue;
	
	PTransmitter*				    m_pTransmitter;
	IPacketHandlerTask *			m_pHandlerTask;
	PUIMsgHandlerTask *			    m_pUIMsgHandlerTask; 
	IPacketParser *				    m_pPktUIParser;
	PIniReader*						m_pConfigReader;
	std::string 					m_strLogFileName;

	boost::thread_group			thread_groups;
	PPlatTransThreadFunc*		plat_trans_thread_func;
};


