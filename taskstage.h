#pragma once

#define CELLW		6	// ステージ幅
#define CELLH		12	// ステージ高
#define NUMBLOCK	3	// 

#define FALLSPEED	30	// 落下速度
#define FALLSPEEDUP	1	// ↓キー入れた時の落下速度
#define CHECKFRAME	30	// 設置してからの間フレーム
#define BLINKFRAME	60	// 消去演出時間

class taskStage : public TaskBase
{
public:
	taskStage();
	virtual ~taskStage();

	virtual void onAddTask();
	virtual void onReleaseTask();
	virtual bool onInitTask();
	virtual void onUpdate(COUTHANDLE hOUT);

	virtual void* onMessage(int msg, void*p1, void *p2);

private:
	ENUMCLASS(eBLOCKTYPE,char) {
		EMPTY,
		NORMAL1,
		NORMAL2,
		NORMAL3,
		NORMAL4,
		NORMAL5,
		NORMAL6,
		SPECIAL
	};
	ENUMCLASS(eCELLSTATE,char) {
		CELLEMPTY,
		HOLD,
		BLINK,
		REMOVEEFFECTING,
	};
	ENUMCLASS(eSTATE,char) {
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
	
	void PrintBlock(COUTHANDLE hOUT,int x, int y, eBLOCKTYPE type);
	void draw(COUTHANDLE hOUT);
};
