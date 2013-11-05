#include "stdafx.h"

G g;


void G::onUpdate()
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
	if( GetKeyState(VK_DOWN)&0x80 )
		nkey |= KEY_DOWN;

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

	keyTrg = (~this->keys) & nkey;
	this->keys = nkey;
}

COUTHANDLE ConsoleInit()
{
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

	return hOUT;
}

void ConsoleLocate(COUTHANDLE hOUT, int x, int y)
{
#ifdef WIN32
	COORD c={x,y};
	SetConsoleCursorPosition(hOUT,c);
#else
	printf("\e[%d;%dH",y+1,x+1);
#endif
}

void ConsolePrintLine(COUTHANDLE hOUT, const char *pStr)
{
#ifdef WIN32
	WriteFile( hOUT, pStr, strlen(pStr), NULL, NULL );
#else
	printf("%s",pStr);
#endif
}

void ConsoleClear(COUTHANDLE hOUT)
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

unsigned int getMS()
{
#ifdef WIN32
	return (unsigned int)timeGetTime();
#else
	struct timeval t;
	gettimeofday(&t, NULL);
	return (unsigned int)(t.tv_sec*1000+t.tv_usec/1000);
#endif
}

void mSleep(int ms)
{
#ifdef WIN32
	Sleep(ms);
#else
	usleep(ms*1000);
#endif
}
