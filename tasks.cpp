#include "stdafx.h"
#pragma comment(lib,"winmm.lib")

TaskBase::TaskBase()
{
}

TaskBase::~TaskBase()
{
}


TaskMan::TaskMan()
{
}

TaskMan::~TaskMan()
{
}

void TaskMan::onUpdate(COUTHANDLE hOUT)
{
	for(std::vector<TASKINFO>::iterator itr=tasks.begin(); itr!=tasks.end(); ++itr ) {
		TASKINFO *pTI = &*itr;
		if( pTI->state == ADDTASK ) {
			pTI->pTask->onInitTask();
			pTI->state = IDLE;
		}
		itr->pTask->onUpdate(hOUT);
	}

	for(std::vector<TASKINFO>::iterator itr=tasks.begin(); itr!=tasks.end(); ) {
		TASKINFO *pTI = &*itr;
		if( pTI->state != RELEASE ){
			++itr;
		}else{	
			pTI->pTask->onReleaseTask();
			delete pTI->pTask;
			itr = tasks.erase(itr);
		}
	}
}

void TaskMan::AddTask( TaskBase *pTask )
{
	TASKINFO ti;
	ti.pTask = pTask;
	ti.state = ADDTASK;
	tasks.push_back( ti );
}

void TaskMan::ReleaseTask( TaskBase *pTask )
{
	for( std::vector<TASKINFO>::iterator itr=tasks.begin(); itr!=tasks.end(); ++itr ) {
		TASKINFO *pTI = &*itr;
		if( pTI->pTask == pTask )
			pTI->state = RELEASE;
	}
}

void* TaskMan::SendMessage( int msg, void *p1, void *p2 )
{
	void *pResult = NULL;
	for( std::vector<TASKINFO>::iterator itr=tasks.begin(); itr!=tasks.end(); ++itr ) {
		TASKINFO *pTI = &*itr;
		if( pTI->state != RELEASE )
			pTI->pTask->onMessage( msg, p1, p2 );
	}
	return pResult;
}
