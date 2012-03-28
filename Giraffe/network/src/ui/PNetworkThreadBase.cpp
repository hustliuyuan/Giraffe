#include "common/Header.h"
#include "common/PTime.h"
#include "config/PPublicConfig.h"
#include <queue>
#include <tchar.h>
#include "ui/PNetworkThreadBase.h"

using namespace log4cplus;

PNetworkThreadBase::PNetworkThreadBase(void)
:m_pUIReqMsgQueue(NULL)
{
	this->m_pHandlerTask = NULL;
	this->m_pUIMsgHandlerTask = NULL;
	this->m_pPktUIParser= NULL;
	this->m_pTransmitter= NULL;
	this->m_pUIReqMsgQueue= NULL;

}
PNetworkThreadBase::~PNetworkThreadBase(void)
{
	cleanUp();
}

void	PNetworkThreadBase::startLogInfrastrue(const char *path, S32 log_level)
{
	/* step 1: Instantiate an appender object */
	//log4cplus::SharedAppenderPtr _append(new log4cplus::RollingFileAppender( logfilename.c_str(), 20*1024, 1000));
	log4cplus::SharedAppenderPtr _append(new log4cplus::FileAppender( path));//, 20*1024, 1000));
	//SharedAppenderPtr _append(new DailyRollingFileAppender(logfilename.c_str(), HOURLY, true, 1000));
	_append->setName(_T("append for test"));
	/* step 2: Instantiate a layout object */
	String pattern = _T("%-6p  %-15c %D %m %n ");
	std::auto_ptr<log4cplus::Layout>  _layout(new log4cplus::PatternLayout(pattern));
	/* step 3: Attach the layout object to the appender */
	_append->setLayout( _layout );
	/* step 4: Instantiate a logger object */
	log4cplus::Logger _logger = log4cplus::Logger::getRoot();
	_logger.setLogLevel( log_level);
	/* step 5: Attach the appender object to the logger  */
	_logger.addAppender(_append);
}
void  PNetworkThreadBase::configure()
{
	PTime::setLowResolutionSeconds();
	PTime::setLowResolutionMilliSeconds();

	PUtils::setDefaultPath2ModulePath();
	srand(PUtils::seconds());

	String path;
	PUtils::getCurrentDirectory(path);

	startLogInfrastrue(m_strLogFileName.c_str(), log4cplus::DEBUG_LOG_LEVEL);

	PSettings &  settings= PSettings::getInstance();

	LOG_WARN("client process: ","_tmain(): 2010-01-01-01-01 ");	
	
	String strPublicSessionig = "public_conf.ini";
	PPublicConfig pub_inireader(strPublicSessionig);

	settings.loadEnv( pub_inireader );

	//  get config from outside.

	settings.loadEnv(*m_pConfigReader);

	log4cplus::Logger _logger = log4cplus::Logger::getRoot();

	S32  my_log_level = settings.getConfig("log-level").m_value.intvalue;
	
	_logger.setLogLevel( my_log_level);

	delete m_pConfigReader; // 
	m_pConfigReader = NULL;
	
}

void	PNetworkThreadBase::setConfigReader( const std::string &  log_file_name, PIniReader *  config_reader )
{

	//GDCSConfig client_inireader("dcs_conf/dcs_conf.ini");

	this->m_pConfigReader = config_reader;
	this->m_strLogFileName = log_file_name;

}

void	PNetworkThreadBase::composeComponent() 
{

	////Transmitter
	//m_pTransmitter = new Network::PTransmitter();
	//
	////packetHandler
	//m_pHandlerTask = new PPacketHandler(m_pTransmitter);
	//m_pTransmitter->registerPacketHandler(m_pHandlerTask) ;
	//
	////uiReqMsgHandler
	//m_pUIMsgHandlerTask = new  IUIMsgHandlerTask(m_pUIReqMsgQueue);
	//IUIMsgHandler * ui_msg_hdl = PMsgMgr->getInstance( m_pTransmitter );
	//m_pUIMsgHandlerTask->registerHandler( ui_msg_hdl );

	////packetParser
	//m_pPktUIParser = new GDCSPktParser();
	////nat_pkt_parser = new PNatPktParser();
	//
	//PPacketFactory & pkt_factory_instance = PPacketFactory::getInstance();
	//
	////×¢²á pkt parser
	//bool is_succ = pkt_factory_instance.registerPktParser( PMyPktDef::MY_MAJOR_PKT, m_pPktUIParser,
	//	Network::PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
	//assert( is_succ );

	////×¢²áNAT pkt parser
	////is_succ = pkt_factory_instance.registerPktParser(PPktDef::PEER_TO_NAT_SERVER, nat_pkt_parser, 
	////	Network::PSocketTypeFactory::DEFAULT_SOCKET_TYPE);
	////assert( is_succ );
	//
 //   //PMsgMgr  = PMsgMgr::GetInstance(m_pTransmitter);
	////TransThread
	//plat_trans_thread_func = new PPlatTransThreadFunc(m_pTransmitter, m_pHandlerTask,m_pUIMsgHandlerTask);
	//thread_groups.create_thread( boost::ref(*plat_trans_thread_func) );
	//thread_groups.join_all();

}

U32  PNetworkThreadBase::start(LPVOID para)
{
	PNetworkThreadBase* run = (PNetworkThreadBase*)para;
	run->configure();
	run->composeComponent();
	return 0;
}

void PNetworkThreadBase::setUIReqMsgContainer( PSynQueue<IUIReqMsgBase* > * pUIReqQueue )
{
	assert( !m_pUIReqMsgQueue );
	m_pUIReqMsgQueue = pUIReqQueue;
}

void PNetworkThreadBase::cleanUp()
{
	plat_trans_thread_func->stop();

	//if (NULL != plat_trans_thread_func)
	//{
	//	delete plat_trans_thread_func;
	//	plat_trans_thread_func = NULL;
	//}
	if (m_pTransmitter != NULL)
	{
	  delete m_pTransmitter;
	  m_pTransmitter = NULL;
	}


	if (m_pHandlerTask != NULL)
	{
	  delete m_pHandlerTask;
	  m_pHandlerTask = NULL;
	}
    if (m_pUIMsgHandlerTask != NULL)
    {
      delete m_pUIMsgHandlerTask;
	  m_pUIMsgHandlerTask = NULL;
    }	
    if (m_pPktUIParser != NULL)
    {
		delete m_pPktUIParser;
		m_pPktUIParser = NULL;
    }
	//delete nat_pkt_parser;
}
