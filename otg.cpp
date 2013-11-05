#include "stdafx.h"

int main(int argc, char* argv[])
{
	init_genrand(timeGetTime());
	COUTHANDLE hOUT = ConsoleInit();
	g.pTaskMan = new TaskMan();
	g.keys = 0;

	g.pTaskMan->AddTask( new taskStage() );

	ConsoleClear(hOUT);
	unsigned int dwNextTime = 0;
	while( (g.keys & KEY_ESCAPE) == false ) {
		g.onUpdate();

		unsigned int dwTime;
		while( (dwTime=getMS()) < dwNextTime )
			mSleep(1);
		dwNextTime = dwTime + 16;
		g.pTaskMan->onUpdate(hOUT);
	}
	delete g.pTaskMan;

#ifndef WIN32
	tcsetattr(0, TCSANOW, &saveAttr);
#endif
	
	return 0;
}

