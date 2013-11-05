// otg.cpp : コンソール アプリケーションのエントリ ポイントを定義します。

#include "stdafx.h"


void inputUpdate()
{
	int nkey = 0;
#ifdef WIN32
	if( GetKeyState(VK_ESCAPE)&0x80)
		nkey |= KEY_ESCAPE;
	if( GetKeyState(VK_SPACE)&0x80 )
		nkey |= KEY_ROTATION;
	if( GetKeyState(VK_RETURN)&0x80 )
		nkey |= KEY_ROTATION;
	if( GetKeyState(VK_LEFT)&0x80 )
		nkey |= KEY_LEFT;
	if( GetKeyState(VK_RIGHT)&0x80 )
		nkey |= KEY_RIGHT;

	BYTE keys[256];
	GetKeyboardState( keys );
	if( keys['A']&0x80 )
		nkey |= KEY_LEFT;
	if( keys['D']&0x80 )
		nkey |= KEY_RIGHT;
	if( keys['S']&0x80 )
		nkey |= KEY_DOWN;
#else
	auto canRecv = [](int fd)->int{
		fd_set fdset;
		struct timeval timeout;
		FD_ZERO( &fdset );
		FD_SET( fd , &fdset );
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		return select( fd+1 , &fdset , NULL , NULL , &timeout );
	};
	while( canRecv(0) ) {
		char k;
		read(0,&k,1);
		if( k == 0 )
			break;
		k = tolower(k);
		if( k == 'a' )
			nkey |= KEY_LEFT;
		if( k == 'd' )
			nkey |= KEY_RIGHT;
		if( k == 's' )
			nkey |= KEY_DOWN;
		if( k == 'q' )
			nkey |= KEY_ESCAPE;
		if( k == ' ' )
			nkey |= KEY_ROTATION;
	}
#endif

	g.keyTrg = (~g.keys) & nkey;
	g.keys = nkey;
}

#ifndef WIN32
typedef unsigned int	DWORD;
DWORD timeGetTime()
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return (DWORD)(t.tv_sec*1000+t.tv_usec/1000);
}

void Sleep(int n)
{
	usleep(n*1000);
}
#endif
static void ConsoleClear(COUTHANDLE hOUT)
{
#ifdef WIN32
    static COORD coordScreen;
    DWORD dwCharsWritten;
    DWORD dwConsoleSize;
    CONSOLE_SCREEN_BUFFER_INFO  csbi;
	GetConsoleScreenBufferInfo(hOUT,&csbi);
    dwConsoleSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter( hOUT,' ',dwConsoleSize,coordScreen,&dwCharsWritten);
	GetConsoleScreenBufferInfo(hOUT,&csbi);
    FillConsoleOutputAttribute( hOUT,csbi.wAttributes,dwConsoleSize,coordScreen,&dwCharsWritten);
    SetConsoleCursorPosition(hOUT,coordScreen);
#else
	printf("\e[2");
#endif
}

int main(int argc, char* argv[])
{
	init_genrand(timeGetTime());
#ifdef WIN32
	COUTHANDLE hOUT = GetStdHandle(STD_OUTPUT_HANDLE);
#else
	COUTHANDLE hOUT = 1;
	struct termios termAttr, saveAttr;
	tcgetattr(0, &termAttr);
	saveAttr = termAttr;
	termAttr.c_lflag &= ~(ICANON|ECHO);
	termAttr.c_cc[VMIN] = 1;
	termAttr.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &termAttr);
	fcntl(0, F_SETFL, O_NONBLOCK);
#endif
	g.pTaskMan = new TaskMan();
	g.keys = 0;

	g.pTaskMan->AddTask( new taskStage() );

	ConsoleClear(hOUT);
	DWORD dwNextTime = 0;
	while( (g.keys & KEY_ESCAPE) == false ) {
		inputUpdate();

		DWORD dwTime;
		while( (dwTime=timeGetTime()) < dwNextTime )
			Sleep(1);
		dwNextTime = dwTime + 16;
		g.pTaskMan->onUpdate(hOUT);
	}
	delete g.pTaskMan;

#ifndef WIN32
	tcsetattr(0, TCSANOW, &saveAttr);
#endif
	
	return 0;
}

