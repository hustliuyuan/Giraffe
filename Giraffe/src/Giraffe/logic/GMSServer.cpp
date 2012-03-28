#include "Base/GHeader.h"
#include "SessionManager.h"
#include "logic/GMSCommLogger.h"
#include "GPaxosProtocol.h"

int  main()
{
	std::cout << "lllllllll" << std::endl;
	GMSCommLogger::setCommConfig();
	GPaxosProtocol *paxos_instance = new GPaxosProtocol();
	SessionManager *sessionManager = new SessionManager();
	sessionManager->setAppTerminate();
	paxos_instance->setAppTerminate();
	return 0;
}



