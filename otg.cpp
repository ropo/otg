// otg.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"


void inputUpdate()
{
	int nkey = 0;

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

	g.keyTrg = (~g.keys) & nkey;
	g.keys = nkey;
}

static void ConsoleClear(HANDLE hOUT)
{
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
}

int _tmain(int argc, _TCHAR* argv[])
{
	init_genrand(timeGetTime());

	HANDLE hOUT = GetStdHandle(STD_OUTPUT_HANDLE);
	g.pTaskMan = new TaskMan();
	g.keys = 0;

	g.pTaskMan->AddTask( new taskStage() );

	DWORD dwNextTime = 0;
	while( (g.keys & KEY_ESCAPE) == false ) {
		inputUpdate();

		DWORD dwTime;
		while( (dwTime=timeGetTime()) < dwNextTime )
			Sleep(1);
		dwNextTime = dwTime + 16;

//		ConsoleClear(hOUT);
		g.pTaskMan->onUpdate(hOUT);
	}
	delete g.pTaskMan;

	return 0;
}

