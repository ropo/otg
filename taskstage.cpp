#include "stdafx.h"

void taskStage::CELLINFO::empty()
{
	type = eBLOCKTYPE::EMPTY;
	state = eCELLSTATE::CELLEMPTY;
}
taskStage::taskStage()
{
	for( int y=0; y<CELLH; y++ )
		for( int x=0; x<CELLW; x++ )
			blocks[y][x].empty();
	RefreshBlock();
	counter = fallSpeed = FALLSPEED;
	state = eSTATE::FALLDOWN;
	score = 0;
}

taskStage::~taskStage()
{
}
void taskStage::onAddTask()
{
}
void taskStage::onReleaseTask()
{
}
bool taskStage::onInitTask()
{
	return false;
}
void taskStage::RefreshBlock()
{
	for( int i=0; i<NUMBLOCK; i++ )
		cursorBlock[i] = (eBLOCKTYPE)( (int)eBLOCKTYPE::NORMAL1+(int)(genrand_res53()*6) );
	cx = CELLW/2;
	cy = 0;
}
char taskStage::BlockChar(eBLOCKTYPE type)
{
	switch( type ) {
		case eBLOCKTYPE::NORMAL1:	return '1';
		case eBLOCKTYPE::NORMAL2:	return '2';
		case eBLOCKTYPE::NORMAL3:	return '3';
		case eBLOCKTYPE::NORMAL4:	return '4';
		case eBLOCKTYPE::NORMAL5:	return '5';
		case eBLOCKTYPE::NORMAL6:	return '6';
		case eBLOCKTYPE::SPECIAL:	return '*';
	}
	return ' ';
}
bool taskStage::BlockDown()
{
	if( cy+1>=CELLH || blocks[cy+1][cx].type != EMPTY )
		return true;
	cy++;
	return false;
}
bool taskStage::BlockCheck(CELLINFO *p, CELLINFO *p1, CELLINFO *p2 )
{
	if( p->type!=p1->type || p->type!=p2->type)
		return false;
	p2->state = p1->state = p->state = eCELLSTATE::REMOVEEFFECTING;
	return true;
}
bool taskStage::BlockCheck()
{
	bool result = false;
	for( int y=0; y<CELLH; y++ ) {
		for( int x=0; x<CELLW; x++ ) {
			CELLINFO *p1, *p2, *p = &blocks[y][x];
			if( p->type == eBLOCKTYPE::EMPTY )
				continue;
			bool w=false, h=false;
			if( y>=1 && y<CELLH-1 ){
				h = true;
				p1=&blocks[y-1][x];
				p2=&blocks[y+1][x];
				if( BlockCheck(p,p1,p2) )
					result = true;
			}
			if( x>=1 && x<CELLW-1 ){
				w = true;
				p1=&blocks[y][x-1];
				p2=&blocks[y][x+1];
				if( BlockCheck(p,p1,p2) )
					result = true;
			}
			if( w && h ){
				p1=&blocks[y-1][x-1];
				p2=&blocks[y+1][x+1];
				if( BlockCheck(p,p1,p2) )
					result = true;
				p1=&blocks[y-1][x+1];
				p2=&blocks[y+1][x-1];
				if( BlockCheck(p,p1,p2) )
					result = true;
			}
		}
	}
	return result;
}
void taskStage::BlockFreeze()
{
	for( int i=0; i<NUMBLOCK; i++ ) {
		blocks[cy-i][cx].type = cursorBlock[i];
		blocks[cy-i][cx].state = eCELLSTATE::HOLD;
	}
}
void taskStage::onUpdate(HANDLE hOUT)
{
	auto Locate = [&hOUT](int x, int y){
		COORD c={x,y};
		SetConsoleCursorPosition(hOUT,c);
	};
	auto PrintLine = [&hOUT](const char *pStr){
		WriteFile( hOUT, pStr, strlen(pStr), NULL, NULL );
	};
	auto PrintBlock = [&](int x, int y, eBLOCKTYPE type){
		if( y<0 )	return;
		Locate(x,y);
		char buf[2]={BlockChar(type),0};
		PrintLine(buf);
	};
	Locate(0,0);

	counter--;
	if( counter<0 ) {
		switch( state )
		{
			case eSTATE::FALLDOWN:
				counter = fallSpeed;
				if( BlockDown() ) {
					BlockFreeze();
					state = eSTATE::CHECKWAIT;
				}
				break;
			case eSTATE::CHECKWAIT:
				if( BlockCheck() ) {
					state = eSTATE::EFFECTBLINK;
					counter = BLINKFRAME;
				}else{
					RefreshBlock();
					counter = fallSpeed;
					state = eSTATE::FALLDOWN;
				}
				break;
			case eSTATE::EFFECTBLINK:
				for( int y=0; y<CELLH; y++ ) {
					for( int x=0; x<CELLW; x++ ) {
						if( blocks[y][x].state != eCELLSTATE::REMOVEEFFECTING )
							continue;
						blocks[y][x].empty();
						score++;
					}
				}
				for( int x=0; x<CELLW; x++ ) {
					for( int y=CELLH-1; y>0; y-- ) {
						if( blocks[y][x].type!=eBLOCKTYPE::EMPTY )
							continue;
						for( int y2=y-1; y2>=0; y2-- ) {
							if( blocks[y2][x].type==eBLOCKTYPE::EMPTY )
								continue;
							blocks[y][x] = blocks[y2][x];
							blocks[y2][x].empty();
							break;
						}
					}
				}
				state = eSTATE::CHECKWAIT;
				break;
		}
	}

	// input
	if( state==eSTATE::FALLDOWN )
	{
		if( g.keyTrg & KEY_ROTATION ) {
			eBLOCKTYPE type = cursorBlock[0];
			for( int i=1; i<NUMBLOCK; i++ )
				cursorBlock[i-1] = cursorBlock[i];
			cursorBlock[NUMBLOCK-1] = type;
		}

		if( cx > 0 && g.keyTrg & KEY_LEFT ) {
			bool isHit = false;
			for( int i=0; i<NUMBLOCK; i++ ) {
				if( cy-i>=0 && blocks[cy-i][cx-1].type != eBLOCKTYPE::EMPTY ) {
					isHit = true;
					break;
				}
			}
			if( !isHit )
				cx--;
		}
		if( cx < CELLW-1 && g.keyTrg & KEY_RIGHT ) {
			bool isHit = false;
			for( int i=0; i<NUMBLOCK; i++ ) {
				if( cy-i>=0 && blocks[cy-i][cx+1].type != eBLOCKTYPE::EMPTY ) {
					isHit = true;
					break;
				}
			}
			if( !isHit )
				cx++;
		}
		if( counter > FALLSPEEDUP && g.keys & KEY_DOWN )
			counter = FALLSPEEDUP;
	}

	// 外壁
	bool isEffectBlink = true;
	if( state==eSTATE::EFFECTBLINK && (counter/20)&1 )
		isEffectBlink = false;
	char lineBuff[1000];
	for( int y=0; y<CELLH; y++ ) {
		char *p = lineBuff;
		*(p++) = '#';
		for( int x=0; x<CELLW; x++ ) {
			if( blocks[y][x].state == eCELLSTATE::REMOVEEFFECTING && isEffectBlink==false )
				*(p++) = '-';
			else
				*(p++) = BlockChar( blocks[y][x].type );
		}
		*(p++) = '#';
		*(p++) = '\n';
		*(p++) = '\0';
		PrintLine( lineBuff );
	}
	for( int x=0; x<CELLW+2; x++ )
		PrintLine( "#" );

	// カーソル
	if( state==eSTATE::FALLDOWN ) {
		for( int i=0; i<NUMBLOCK; i++ )
			PrintBlock(cx+1,cy-i,cursorBlock[i]);
	}

	// すこあ
	Locate( CELLW+5, 1 );
	sprintf(lineBuff,"SCORE:%d\n", score );
	PrintLine( lineBuff );
	Locate( 0, CELLH+3 );
	PrintLine( "A < > D  GameExit:[Escape]\n" );
	PrintLine( "   S     Rotation:[SPACE]or[Enter]" );
}

void* taskStage::onMessage(int msg, void*p1, void *p2)
{
	return NULL;
}
