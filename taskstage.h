#pragma once

#define CELLW		6	// �X�e�[�W��
#define CELLH		12	// �X�e�[�W��
#define NUMBLOCK	3	// 

#define FALLSPEED	30	// �������x
#define FALLSPEEDUP	1	// ���L�[���ꂽ���̗������x
#define CHECKFRAME	30	// �ݒu���Ă���̊ԃt���[��
#define BLINKFRAME	60	// �������o����

class taskStage : public TaskBase
{
public:
	taskStage();
	virtual ~taskStage();

	virtual void onAddTask();
	virtual void onReleaseTask();
	virtual bool onInitTask();
	virtual void onUpdate(HANDLE hOUT);

	virtual void* onMessage(int msg, void*p1, void *p2);

private:
	enum eBLOCKTYPE {
		EMPTY,
		NORMAL1,
		NORMAL2,
		NORMAL3,
		NORMAL4,
		NORMAL5,
		NORMAL6,
		SPECIAL
	};
	enum eCELLSTATE {
		CELLEMPTY,
		HOLD,
		BLINK,
		REMOVEEFFECTING,
	};
	enum eSTATE {
		FALLDOWN,
		CHECKWAIT,
		EFFECTBLINK,
	};
	typedef struct tagCELLINFO {
		eBLOCKTYPE type;
		eCELLSTATE state;

		void empty();
	}CELLINFO;

	int cx, cy;
	eBLOCKTYPE cursorBlock[NUMBLOCK];

	int counter;
	int fallSpeed;

	int score;

	CELLINFO blocks[CELLH][CELLW];
	eSTATE state;

	void RefreshBlock();
	char BlockChar(eBLOCKTYPE type);
	bool BlockDown();
	bool BlockCheck();
	bool BlockCheck(CELLINFO *p, CELLINFO *p1, CELLINFO *p2);
	void BlockFreeze();
};