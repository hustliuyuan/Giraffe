#include "Base/GHeader.h"
#include "GServerConfig.h"
#include "GMSCommLogger.h"

using namespace log4cplus;

void GMSCommLogger::startLogInfrastrue(const char *path, S32 log_level)
{
	/* step 1: Instantiate an appender object */
	//log4cplus::SharedAppenderPtr _append(new log4cplus::RollingFileAppender( logfilename.c_str(), 20*1024, 1000));
	log4cplus::SharedAppenderPtr _append(new log4cplus::FileAppender( path));//, 20*1024, 1000));
	//SharedAppenderPtr _append(new DailyRollingFileAppender(logfilename.c_str(), HOURLY, true, 1000));
	_append->setName("append for test");
	//_append->setName(_T("append for test"));
	/* step 2: Instantiate a layout object */
	//const log4cplus::tstring  pattern = _T("%-6p  %-15c %D %m %n ");
	const log4cplus::tstring  pattern = ("%-6p  %-15c %D %m %n ");
	std::auto_ptr<log4cplus::Layout>  _layout(new log4cplus::PatternLayout(pattern));
	/* step 3: AttaGRF the layout object to the appender */
	_append->setLayout( _layout );
	/* step 4: Instantiate a logger object */
	log4cplus::Logger _logger = log4cplus::Logger::getRoot();
	_logger.setLogLevel( log_level );
	/* step 5: AttaGRF the appender object to the logger  */
	_logger.addAppender(_append);
}


void GMSCommLogger::setCommConfig()
{
	PTime::setLowResolutionSeconds();
	PTime::setLowResolutionMilliSeconds();

	PUtils::setDefaultPath2ModulePath();
	srand(PUtils::seconds());

	String path ;
	PUtils::getCurrentDirectory(path);
	startLogInfrastrue("Giraffe_Log.txt", DEBUG_LOG_LEVEL);

	PSettings &settings= PSettings::getInstance();

	LOG_DEBUG("Dession Tracer Main Process: ","main(): 2009-08-04 ");	

	GServerConfig pub_inireader("public_conf.ini");

	settings.loadEnv( pub_inireader );
	
	GServerConfig client_inireader("Giraffe_conf/Giraffe_conf.ini");

	settings.loadEnv(client_inireader);
	log4cplus::Logger _logger = log4cplus::Logger::getRoot();

	S32  my_log_level = settings.getConfig("log-level").m_value.intvalue;
	//S32  my_log_level = 10000;
	_logger.setLogLevel( my_log_level);

	LOG_INFO("main"," now i try to set log level:"<< my_log_level );

	_logger.setLogLevel( my_log_level);
}

