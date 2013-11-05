#pragma once

#include "targetver.h"

#ifdef WIN32
 #include <Windows.h>
 #include <tchar.h>
 typedef HANDLE	COUTHANDLE;
 #define ENUMCLASS(n,t) enum n
#else
 #include <unistd.h>
 #include <termios.h>
 #include <fcntl.h>
 #include <sys/time.h>
 typedef int COUTHANDLE;
 #define ENUMCLASS(n,t)	enum class n : t
#endif

#include <stdio.h>
#include <algorithm>
#include <list>
#include <vector>

#include "mt.h"
#include "tasks.h"
#include "taskstage.h"


#define KEY_ESCAPE	(1<<0)
#define KEY_LEFT	(1<<1)
#define KEY_RIGHT	(1<<2)
#define KEY_DOWN	(1<<3)
#define KEY_ROTATION (1<<4)

COUTHANDLE ConsoleInit();
void ConsoleLocate(COUTHANDLE hOUT, int x, int y);
void ConsolePrintLine(COUTHANDLE hOUT, const char *pStr);
void ConsoleClear(COUTHANDLE hOUT);
unsigned int getMS();
void mSleep(int ms);

typedef struct tagG{
	TaskMan *pTaskMan;

	// input
	int keys;
	int keyTrg;

	void onUpdate();
}G;

extern G g;
