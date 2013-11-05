#include "stdafx.h"

void taskStage::CELLINFO::empty()
{
	type = EMPTY;
	state = CELLEMPTY;
}
taskStage::taskStage()
{
	for( int y=0; y<CELLH; y++ )
		for( int x=0; x<CELLW; x++ )
			blocks[y][x].empty();
	RefreshBlock();
	counter = fallSpeed = FALLSPEED;
	state = FALLDOWN;
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
		cursorBlock[i] = (eBLOCKTYPE)( (int)NORMAL1+(int)(genrand_res53()*6) );
	cx = CELLW/2;
	cy = 0;
}
char taskStage::BlockChar(eBLOCKTYPE type)
{
	switch( type ) {
		case NORMAL1:	return '1';
		case NORMAL2:	return '2';
		case NORMAL3:	return '3';
		case NORMAL4:	return '4';
		case NORMAL5:	return '5';
		case NORMAL6:	return '6';
		case SPECIAL:	return '*';
		default:	return ' ';
	}
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
	p2->state = p1->state = p->state = REMOVEEFFECTING;
	return true;
}
bool taskStage::BlockCheck()
{
	bool result = false;
	for( int y=0; y<CELLH; y++ ) {
		for( int x=0; x<CELLW; x++ ) {
			CELLINFO *p1, *p2, *p = &blocks[y][x];
			if( p->type == EMPTY )
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
		blocks[cy-i][cx].state = HOLD;
	}
}
void taskStage::onUpdate(COUTHANDLE hOUT)
{
	counter--;
	if( counter<0 ) {
		switch( state )
		{
			case FALLDOWN:
				counter = fallSpeed;
				if( BlockDown() ) {
					BlockFreeze();
					state = CHECKWAIT;
				}
				break;
			case CHECKWAIT:
				if( BlockCheck() ) {
					state = EFFECTBLINK;
					counter = BLINKFRAME;
				}else{
					RefreshBlock();
					counter = fallSpeed;
					state = FALLDOWN;
				}
				break;
			case EFFECTBLINK:
				for( int y=0; y<CELLH; y++ ) {
					for( int x=0; x<CELLW; x++ ) {
						if( blocks[y][x].state != REMOVEEFFECTING )
							continue;
						blocks[y][x].empty();
						score++;
					}
				}
				for( int x=0; x<CELLW; x++ ) {
					for( int y=CELLH-1; y>0; y-- ) {
						if( blocks[y][x].type!=EMPTY )
							continue;
						for( int y2=y-1; y2>=0; y2-- ) {
							if( blocks[y2][x].type==EMPTY )
								continue;
							blocks[y][x] = blocks[y2][x];
							blocks[y2][x].empty();
							break;
						}
					}
				}
				state = CHECKWAIT;
				break;
		}
	}

	// input
	if( state==FALLDOWN )
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
				if( cy-i>=0 && blocks[cy-i][cx-1].type != EMPTY ) {
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
				if( cy-i>=0 && blocks[cy-i][cx+1].type != EMPTY ) {
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

	draw(hOUT);
}

void taskStage::PrintBlock(COUTHANDLE hOUT,int x, int y, eBLOCKTYPE type)
{
	if( y<0 )	return;
	ConsoleLocate(hOUT,x,y);
	char buf[2]={BlockChar(type),0};
	ConsolePrintLine(hOUT,buf);
}

void taskStage::draw(COUTHANDLE hOUT)
{
	ConsoleLocate(hOUT,0,0);
	
	// wall
	bool isEffectBlink = true;
	if( state==EFFECTBLINK && (counter/10)&1 )
		isEffectBlink = false;
	char lineBuff[1000];
	for( int y=0; y<CELLH; y++ ) {
		char *p = lineBuff;
		*(p++) = '#';
		for( int x=0; x<CELLW; x++ ) {
			if( blocks[y][x].state ==REMOVEEFFECTING && isEffectBlink==false )
				*(p++) = '-';
			else
				*(p++) = BlockChar( blocks[y][x].type );
		}
		*(p++) = '#';
		*(p++) = '\n';
		*(p++) = '\0';
		ConsolePrintLine(hOUT, lineBuff );
	}
	for( int x=0; x<CELLW+2; x++ )
		ConsolePrintLine(hOUT, "#" );
	
	// cursorBlock
	if( state==FALLDOWN ) {
		for( int i=0; i<NUMBLOCK; i++ )
			PrintBlock(hOUT,cx+1,cy-i,cursorBlock[i]);
	}
	
	// score
	ConsoleLocate( hOUT,CELLW+4, 1 );
	sprintf(lineBuff,"SCORE:%d\n", score );
	ConsolePrintLine(hOUT, lineBuff );
	ConsoleLocate( hOUT,0, CELLH+3 );
#ifdef WIN32
	ConsolePrintLine(hOUT, "[A]< >[D] ExitGame:[Escape]\n" );
	ConsolePrintLine(hOUT, "   [S]    Rotation:[SPACE]or[Enter]" );
#else
	ConsolePrintLine(hOUT, "[A]< >[D] ExitGame:[q]\n" );
	ConsolePrintLine(hOUT, "   [S]    Rotation:[SPACE]" );
#endif
}

void* taskStage::onMessage(int msg, void*p1, void *p2)
{
	return NULL;
}
