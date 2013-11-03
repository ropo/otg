#pragma once

#include "targetver.h"

#include <Windows.h>

#include <stdio.h>
#include <tchar.h>
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


typedef struct tagG{
	TaskMan *pTaskMan;

	// input
	int keys;
	int keyTrg;
}G;

extern G g;
