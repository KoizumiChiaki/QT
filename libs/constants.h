#ifndef _QTHWK_CONSTANTS_H_
#define _QTHWK_CONSTANTS_H_

const int timePerTick = 2;
//the number of ms per tick
const int screenWidth = 48, screenHeight = 36;
const unsigned int tps = 60;
// gravity (vY diminishing speed per second) 
const double gravity = 200;
// speed det per second 
const double normalSpeed = 200;
const double dashSpeed = 400;
// vY increment per jump  
const double jumpSpeed = 40;
// speed limits 
const double normalSpeedLimit = 20;
const double dashSpeedLimit = 30;
const double verticalSpeedLimit = 40;
// jump cd 
const double playerheight = 0.8125;
const unsigned int jumpCoolDownTicks = 10;
const unsigned int HurtCdTicks = 10;
// when not moving, Xspeed decrease alphaX times per tick
const double alphaX = 1.12;
const int MaxHp = 100;
const int MaxMp = 100;
const double TossSpeed = 5;
const double ShootSpeed = 20;

#endif // _QTHWK_CONSTANTS_H_
