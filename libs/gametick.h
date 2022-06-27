#ifndef _QTHWK_GAMETICK_H_
#define _QTHWK_GAMETICK_H_

#include <QMainWindow>
#include "constants.h"
#include "gamemap.h"
#include "gamemath.h"
#include "kbinput.h"

const double eps = 1e-6;
const double INF = 0x3f3f3f3f;

namespace __gameTick
{
	
	class player
	{
	public:
		// location 
		double posX, posY;
		// velocity (blocks per second) 
		double vX, vY; 
		// jump count 
		int jumpCount; 
		// jumping need some cd 
		int jumpCoolDown; 
		// is in Dash 
		bool inDash; 
		
		void initialize()
		{
			vX = 0, vY = 0;
			jumpCount = 0, jumpCoolDown = 0;
			inDash = false;
		}
		
		enum direction
		{LEFT, RIGHT, UP, DOWN};
		void addMove(const direction movDir)
		{
			if(movDir == LEFT)
			{
				if(!inDash)
				{
					vX -= normalSpeed / tps;
					if(vX < -normalSpeedLimit)
						vX = -normalSpeedLimit;
				}
				else
				{
					vX -= dashSpeed / tps; 
					if(vX < -dashSpeedLimit)
						vX = -dashSpeedLimit;
				}
			}
			if(movDir == RIGHT)
			{
				if(!inDash)
				{
					vX += normalSpeed / tps;
					if(vX > normalSpeedLimit)
						vX = normalSpeedLimit;
				}
				else
				{
					vX += dashSpeed / tps; 
					if(vX > dashSpeedLimit)
						vX = dashSpeedLimit;
				}
			}
			if(movDir == UP)
			{
				if(jumpCoolDown == 0 && jumpCount != 2)
				{
					vY += jumpSpeed;
					if(vY > verticalSpeedLimit)
						vY = verticalSpeedLimit;
					jumpCount++;
					jumpCoolDown = jumpCoolDownTicks;
				}
			}
		}
        bool checkinblock(double X,double Y)
        {
            return (unsigned int)gameMap.getBlockType(int(X),int(Y)) == 1;
        }
        double div(double x,double y)
        {
            return fabs(y)<eps?(INF):(x/y);
        }
        void fixStatus(double LX,double LY)
		{
            if(checkinblock(posX,posY) && checkinblock(posX+playerheight,posY))
            {
                posY = int(posY) + 1;
                if(vY<0)vY = 0;//Down
                return;
            }
            if(checkinblock(posX,posY+playerheight) && checkinblock(posX+playerheight,posY+playerheight))
            {
                posY = int(posY+playerheight) - playerheight;
                if(vY>0)vY = 0;//Up
                return;
            }
            if(checkinblock(posX,posY) && checkinblock(posX,posY+playerheight))
            {
                posX = int(posX) + 1;
                if(vX<0)vX = 0;//Left
                return;
            }
            if(checkinblock(posX+playerheight,posY) && checkinblock(posX+playerheight,posY+playerheight))
            {
                posX = int(posX+playerheight) - playerheight;
                if(vX>0)vX = 0;//Right
                return;
            }
            if(checkinblock(posX,posY))//leftdown
            {
                double L=int(posY)+1;
                double tmp=LX+(L-LY)*div(LX-posX,LY-posY);
                if(tmp>=int(posX)&&tmp<=int(posX)+1)
                {
                    posY=L;
                    if(vY<0)vY=0;
                    return;
                }
                else
                {
                    posX=int(posX)+1;
                    if(vX<0)vX=0;
                    return;
                }
            }
            if(checkinblock(posX,posY+playerheight))//leftup
            {
                double L=int(posY+playerheight);
                double tmp=LX+(L-LY-playerheight)*div(LX-posX,LY-posY);
                if(tmp>=int(posX)&&tmp<=int(posX)+1)
                {
                    posY=L-playerheight;
                    if(vY>0)vY=0;
                    return;
                }
                else
                {
                    posX=int(posX)+1;
                    if(vX<0)vX=0;
                    return;
                }
            }
            if(checkinblock(posX+playerheight,posY))//rightdown
            {
                double L=int(posY)+1;
                double tmp=LX+playerheight+(L-LY)*div(LX-posX,LY-posY);
                if(tmp>=int(posX+playerheight)&&tmp<=int(posX+playerheight)+1)
                {
                    posY=L;
                    if(vY<0)vY=0;
                    return;
                }
                else
                {
                    posX=int(posX+playerheight)-playerheight;
                    if(vX>0)vX=0;
                    return;
                }
            }
            if(checkinblock(posX+playerheight,posY+playerheight))//rightup
            {
                double L=int(posY+playerheight);
                double tmp=LX+playerheight+(L-LY)*div(LX-posX,LY-posY);
                if(tmp>=int(posX+playerheight)&&tmp<=int(posX+playerheight)+1)
                {
                    posY=L-playerheight;
                    if(vY>0)vY=0;
                    return;
                }
                else
                {
                    posX=int(posX+playerheight)-playerheight;
                    if(vX>0)vX=0;
                    return;
                }
            }
		}
		bool onGround()
		{
            if(checkinblock(posX,posY-eps)||checkinblock(posX+playerheight,posY-eps))return true;
            return false;
		}
		void move()
		{
			if(jumpCoolDown) jumpCoolDown--;
			
            double LposX = posX,LposY = posY;
			posX += vX / tps, posY += vY / tps;
			
            fixStatus(LposX,LposY);
			if(onGround())jumpCount = 0;
		}
	}P1, P2;
	void begin()
	{
		keyboardStatus.clear();
		P1.initialize();
		P2.initialize();
	}
	void tick()
	{
		// player speed update 
		if(keyboardStatus.l1&&!keyboardStatus.r1)
            P1.addMove(player::LEFT);
		if(keyboardStatus.r1&&!keyboardStatus.l1)
            P1.addMove(player::RIGHT);
		if(keyboardStatus.u1&&!keyboardStatus.d1)
            P1.addMove(player::UP);
		if(keyboardStatus.d1&&!keyboardStatus.u1)
            P1.addMove(player::DOWN);
		 
		if(keyboardStatus.l2&&!keyboardStatus.r2)
            P2.addMove(player::LEFT);
		if(keyboardStatus.r2&&!keyboardStatus.l2)
            P2.addMove(player::RIGHT);
		if(keyboardStatus.u2&&!keyboardStatus.d2)
            P2.addMove(player::UP);
		if(keyboardStatus.d2&&!keyboardStatus.u2)
            P2.addMove(player::DOWN);
		
		// player moving 
		P1.move();
		P2.move();
		
		// player attack 
		// TODO 
		
		// rendering new graphics 
		// TODO 
	}
}
using __gameTick::begin;
using __gameTick::tick;

#endif
