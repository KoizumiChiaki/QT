#ifndef _QTHWK_GAMETICK_H_
#define _QTHWK_GAMETICK_H_

#include <QMainWindow>
#include "contants.h"
#include "gamemap.h"
#include "gamemath.h"
#include "kbinput.h"

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
		bool crashIntoBlock()
		{
			// TODO 
		}
		void fixStatus()
		{
			// TODO 
		}
		bool onGround()
		{
			// TODO 
		}
		void move()
		{
			if(jumpCoolDown) jumpCoolDown--;
			
			posX += vX / tps, posY += vY / tps;
			
			if(crashIntoBlock())
				fixStatus();
			
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
			P1.addMove(LEFT);
		if(keyboardStatus.r1&&!keyboardStatus.l1)
			P1.addMove(RIGHT);
		if(keyboardStatus.u1&&!keyboardStatus.d1)
			P1.addMove(UP);
		if(keyboardStatus.d1&&!keyboardStatus.u1)
			P1.addMove(DOWN);
		 
		if(keyboardStatus.l2&&!keyboardStatus.r2)
			P2.addMove(LEFT);
		if(keyboardStatus.r2&&!keyboardStatus.l2)
			P2.addMove(RIGHT);
		if(keyboardStatus.u2&&!keyboardStatus.d2)
			P2.addMove(UP);
		if(keyboardStatus.d2&&!keyboardStatus.u2)
			P2.addMove(DOWN);
		
		// player moving 
		P1.move();
		P2.move();
		
		// player attack 
		// TODO 
		
		// rendering new graphics 
		// TODO 
	}
}
using __gameTick::begin();
using __gameTick::tick();

#endif