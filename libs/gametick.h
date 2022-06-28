#ifndef _QTHWK_GAMETICK_H_
#define _QTHWK_GAMETICK_H_

#include <ctime>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QMainWindow>
#include <vector>
#include "constants.h"
#include "gamemap.h"
#include "gamemath.h"
#include "kbinput.h"
#include "player.h"

namespace __gameTick
{

    enum gameStatusEnum{menu,inGame,endGame}gameStatus = menu;
    void tick()
	{
		// player speed update 
		if(keyboardStatus.l1&&!keyboardStatus.r1)
            P1.addMove(LEFT);
        else if(keyboardStatus.r1&&!keyboardStatus.l1)
            P1.addMove(RIGHT);
        else P1.diminishX();
		if(keyboardStatus.u1&&!keyboardStatus.d1)
            P1.addMove(UP);
		if(keyboardStatus.d1&&!keyboardStatus.u1)
            P1.addMove(DOWN);
		 
		if(keyboardStatus.l2&&!keyboardStatus.r2)
            P2.addMove(LEFT);
        else if(keyboardStatus.r2&&!keyboardStatus.l2)
            P2.addMove(RIGHT);
        else P2.diminishX();
		if(keyboardStatus.u2&&!keyboardStatus.d2)
            P2.addMove(UP);
		if(keyboardStatus.d2&&!keyboardStatus.u2)
            P2.addMove(DOWN);
		
        // player skill
        if(keyboardStatus.toss1)P1.Toss(1);
        if(keyboardStatus.toss2)P2.Toss(2);
        if(keyboardStatus.shoot1)P1.Shoot(1);
        if(keyboardStatus.shoot2)P2.Shoot(2);
        if(keyboardStatus.dash1)P1.Dash(keyboardStatus.l1,keyboardStatus.r1);
        if(keyboardStatus.dash2)P2.Dash(keyboardStatus.l2,keyboardStatus.r2);

		// player moving 
        P1.updatedirection();
        P2.updatedirection();
		P1.move();
        P2.move();
        std::list<bullet>::iterator it;
        for(it=L.begin();it!=L.end();it++)
        {
            (*it).move();
            if((*it).checkinblock())L.erase(it);
        }
		// rendering new graphics 
        // (maybe not implemented here?)
		// TODO 
    }
}
using __gameTick::gameStatusEnum;
using __gameTick::gameStatus;
using __gameTick::inGame;
using __gameTick::menu;
using __gameTick::endGame;

#endif // _QTHWK_GAMETICK_H_
