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

void TaskMan::onUpdate(HANDLE hOUT)
{
	for(auto itr=tasks.begin(); itr!=tasks.end(); ++itr ) {
		TASKINFO *pTI = &*itr;
		if( pTI->state == eSTATE::ADDTASK ) {
			pTI->pTask->onInitTask();
			pTI->state = eSTATE::IDLE;
		}
		itr->pTask->onUpdate(hOUT);
	}

	for(auto itr=tasks.begin(); itr!=tasks.end(); ++itr ) {
		TASKINFO *pTI = &*itr;
		if( pTI->state != eSTATE::RELEASE )
			continue;
		pTI->pTask->onReleaseTask();
		delete pTI->pTask;
	}
}

void TaskMan::AddTask( TaskBase *pTask )
{
	TASKINFO ti;
	ti.pTask = pTask;
	ti.state = eSTATE::ADDTASK;
	tasks.push_back( ti );
}

void TaskMan::ReleaseTask( TaskBase *pTask )
{
	for( auto itr=tasks.begin(); itr!=tasks.end(); ++itr ) {
		TASKINFO *pTI = &*itr;
		if( pTI->pTask == pTask )
			pTI->state = eSTATE::RELEASE;
	}
}

void* TaskMan::SendMessage( int msg, void *p1, void *p2 )
{
	void *pResult = NULL;
	for( auto itr=tasks.begin(); itr!=tasks.end(); ++itr ) {
		TASKINFO *pTI = &*itr;
		if( pTI->state != eSTATE::RELEASE )
			pTI->pTask->onMessage( msg, p1, p2 );
	}
	return pResult;
}
