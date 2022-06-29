#ifndef _QTHWK_GAMETICK_H_
#define _QTHWK_GAMETICK_H_

#include <ctime>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QMainWindow>
#include <QSound>

#include "gamesound.h"
#include "constants.h"
#include "gamemap.h"
#include "gamemath.h"
#include "kbinput.h"
#include "player.h"

namespace __gameTick
{
    enum gameStatusEnum{menu,inGame,endGame,paused,mapset}gameStatus = menu;
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

        if(P1.Hp>0)P1.updatedirection(keyboardStatus.l1,keyboardStatus.r1);
        if(P2.Hp>0)P2.updatedirection(keyboardStatus.l2,keyboardStatus.r2);
        // player skill
        if(P1.Hp>0)if(keyboardStatus.toss1)P1.Toss(1);
        if(P2.Hp>0)if(keyboardStatus.toss2)P2.Toss(2);
        if(P1.Hp>0)if(keyboardStatus.shoot1)P1.Shoot(1);
        if(P2.Hp>0)if(keyboardStatus.shoot2)P2.Shoot(2);
        if(P1.Hp>0)if(keyboardStatus.dash1)P1.Dash(keyboardStatus.l1,keyboardStatus.r1);
        if(P2.Hp>0)if(keyboardStatus.dash2)P2.Dash(keyboardStatus.l2,keyboardStatus.r2);

		// player moving 

		P1.move();
        P2.move();
        std::list<bullet>::iterator it;
        for(it=Bullet.begin();it!=Bullet.end();it++)
        {
            (*it).move();
            if((*it).owner==1)if(P2.checkhit(*it))Bullet.erase(it),__player::CreateParticle((*it).posX-(*it).vX/tps,(*it).posY-(*it).vY/tps,4),soundStatus.crash = true;
            if((*it).owner==2)if(P1.checkhit(*it))Bullet.erase(it),__player::CreateParticle((*it).posX-(*it).vX/tps,(*it).posY-(*it).vY/tps,4),soundStatus.crash = true;
            if((*it).checkinblock())
            {
                Bullet.erase(it),__player::CreateParticle((*it).posX-(*it).vX/tps,(*it).posY-(*it).vY/tps,4),soundStatus.crash = true;
            }
        }
        std::list<particle>::iterator it2;
        for(it2=Particle.begin();it2!=Particle.end();it2++)
        {
            (*it2).move();
            if((*it2).checkinblock())Particle.erase(it2);
        }
        if (P1.Hp == 0 || P2.Hp == 0)
            gameStatus = endGame;//gameOver
    }
}
using __gameTick::gameStatusEnum;
using __gameTick::gameStatus;
using __gameTick::inGame;
using __gameTick::menu;
using __gameTick::endGame;
using __gameTick::paused;

#endif // _QTHWK_GAMETICK_H_
