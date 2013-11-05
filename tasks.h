#pragma once

class TaskBase
{
public:
	TaskBase();
	virtual ~TaskBase();


	virtual void onAddTask()=0;
	virtual void onReleaseTask()=0;
	virtual bool onInitTask()=0;
	virtual void onUpdate(COUTHANDLE hOUT)=0;

	virtual void* onMessage(int msg, void*p1, void *p2)=0;
};


class TaskMan
{
public:
	TaskMan();
	~TaskMan();

	void onUpdate(COUTHANDLE hOUT);

	void AddTask( TaskBase *pTask );
	void ReleaseTask( TaskBase *pTask );
	void* SendMessage( int msg, void *p1, void *p2 );

private:
	enum eSTATE {
		ADDTASK,
		IDLE,
		RELEASE
	};
	typedef struct tagTASKINFO{
		TaskBase *pTask;
		eSTATE	state;
	}TASKINFO;
	std::vector<TASKINFO>	tasks;
};


