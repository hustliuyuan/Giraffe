// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping. 
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.

#ifndef   _NETWORK_H_
#define   _NETWORK_H_

// all of these classes are exported by network dll.

#include  "common/Header.h"

#include  "network/PInetAddr.h"
#include  "network/PAddressable.h"
//#include  "network/IPacketHandler.h"
#include  "network/PacketINFO.h"
#include  "network/PNatTransmitter.h"
#include  "network/PTransmitter.h"
#include  "network/PSocketType.h"
#include  "network/PSocketTypeFactory.h"
#include  "network/PReceiveBuffer.h"
#include  "network/PacketINFO.h"
#include  "network/ISocketTypeFactoryImpl.h"
#include  "network/IRecvBufferParser.h"
#include  "config/PConfigValue.h"
#include  "config/PIniReader.h"
#include  "config/PPublicConfig.h"
#include  "config/PSettings.h"
#include  "common/IPacketParser.h"
#include  "common/Checksum.h"
#include  "common/Exception.h"
#include  "common/IAppTerminate.h"
#include  "common/IPacketHandler.h"
#include  "common/ISerializble.h"
#include  "common/ITask.h"
//#include  "common/MD5.h"
#include  "common/PPlatTransThreadFunc.h"
#include  "common/PTime.h"
#include  "os/PUtils.h"
#include  "packet/PPacketBase.h"
#include  "packet/PPacketDefinition.h"
#include  "packet/PPacketFactory.h"
#include  "packet/internal/PInternalPkt.h"  // use for application to quit safely.
#include  "packet/nats/PNATAckPkt.h"  // use for optimization of pktown

// new added  tu .  
#include  "multithread/job/IJob.h"
#include  "multithread/job/IJobHandler.h"
#include  "multithread/job/PJobQueue.h"
#include  "multithread/job/PJobThreadFunc.h"
#include  "multithread/job/PJobThreadMgr.h"
//#include  "multithread/job/PJobDef.h"
#include  "multithread/database/IDBConnect.h"

#include  "ui/IUIMsg.h"
#include  "ui/IUIMsgHandlerTask.h"
#include  "ui/PNetworkThreadBase.h"
#include  "common/PSynQueue.h"

////////////////////////////////////////////////////////////////////////////
//void	CommunicationThread::startLogInfrastrue(const char *path, S32 log_level)
//{
//
//	/* step 1: Instantiate an appender object */
//	//log4cplus::SharedAppenderPtr _append(new log4cplus::RollingFileAppender( logfilename.c_str(), 20*1024, 1000));
//	log4cplus::SharedAppenderPtr _append(new log4cplus::FileAppender( path));//, 20*1024, 1000));
//	//SharedAppenderPtr _append(new DailyRollingFileAppender(logfilename.c_str(), HOURLY, true, 1000));
//	_append->setName(_T("append for test"));
//	/* step 2: Instantiate a layout object */
//	String pattern = _T("%-6p  %-15c %D %m %n ");
//	std::auto_ptr<log4cplus::Layout>  _layout(new log4cplus::PatternLayout(pattern));
//	/* step 3: Attach the layout object to the appender */
//	_append->setLayout( _layout );
//	/* step 4: Instantiate a logger object */
//	log4cplus::Logger _logger = log4cplus::Logger::getRoot();
//	_logger.setLogLevel( log_level);
//	/* step 5: Attach the appender object to the logger  */
//	_logger.addAppender(_append);
//}
//void  CommunicationThread::threadrun()
//{
//	PTime::setLowResolutionSeconds();
//	PTime::setLowResolutionMilliSeconds();
//
//	PUtils::setDefaultPath2ModulePath();
//	srand(PUtils::seconds());
//
//	//pstring dd = "hello";
//	String path ;
//	PUtils::getCurrentDirectory(path);
//
//	startLogInfrastrue("dession_log.txt", DEBUG_LOG_LEVEL);
//
//	PSettings &  settings= PSettings::getInstance();
//
//	LOG_WARN("client process: ","_tmain(): 2009-6-5-13-31 ");	
//
//	PPublicConfig pub_inireader("public_conf.ini");
//
//	settings.loadEnv( pub_inireader );
//
//	PClientConfig client_inireader("client_conf/client_conf.ini");
//
//	settings.loadEnv(client_inireader);
//	log4cplus::Logger _logger = log4cplus::Logger::getRoot();
//
//	S32  my_log_level = settings.getConfig("log-level").m_value.intvalue;
//
//	LOG_INFO("main"," now i try to set log level:"<< my_log_level );
//
//	_logger.setLogLevel( my_log_level);
//
//	itransmitter = new Network::PTransmitter();
//	m_pSessionMgr = new DSessionMgr();
//
//	handler_task = new SClientPacketHandler( itransmitter, m_pSessionMgr );
//	ITask * uiReqMsg_handler_task = new SClientUIReqMsgHandlerTask( m_pUIReqMsgContainer, m_pSessionMgr );
//
//	PPlatTransThreadFunc	plat_trans_thread_func
//		(itransmitter, handler_task, uiReqMsg_handler_task);
//	boost::thread_group thread_groups;
//	thread_groups.create_thread(boost::ref(plat_trans_thread_func) );
//	// to wait local network initialization done.
//	PUtils::sleep(1000);
//	LOG_DEBUG("main","all thread exit");	
//	thread_groups.join_all();
//}

//////////////////////////////////////////////////////////////////////////

// working process.
// THE MOST SIMPLE EXAMPLE:
// A. declare and implement a class inhereited from IPacketHandler. 
//    and create an instance.
// B. there is no content in this part.
// C.
// C.1. initialize network ( socket port of tcp and udp are defined in configure file).
// C.2. initialize time.
// C.3. initialize log infrastructure.
// C.4. create an instance of transmitter.
// C.5. create an packet handler with the previously created transmiter as a parameter.
// C.6. create an thread_groups and create a thread from an instance of PPlatTransThreadFunc.
// C.7. thread_groups.join_all();


// ANOTHER EXAMPLE WHICH NEED DIFFERENT SOCKET TYPE. 
// A. declare and implement a class inhereited from IPacketHandler. 
//    and create an instance.
// B.
// B.1. create a PSocketType if needed.
// B.2. declare and implement a class inhereited from IPktParser, and create an instance.
// B.3. declare and implement  a class inhereited from IRecvBufferParser
//    then implement it, and create an instance of it.
// B.4. declare and implement a class inhereited from ISocketTypeFactoryImpl.
// C.
// C.1
// .......


#endif // _NETWORK_H_
