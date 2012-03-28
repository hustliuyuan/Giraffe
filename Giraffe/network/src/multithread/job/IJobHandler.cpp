#include "common/Header.h"
#include "multithread/job/IJobHandler.h"

IJobHandler::IJobHandler()
{

}

IJobHandler::~IJobHandler()
{

}

void		IJobHandler::setJobResultContainer( IJobResultContainer *  container)
{
	this->m_pJobResultContainer  = container;
}