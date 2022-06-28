#ifndef _QTHWK_CONSTANTS_H_
#define _QTHWK_CONSTANTS_H_

const int timePerTick = 5;
//the number of ms per tick
const int screenWidth = 48, screenHeight = 36;
const unsigned int tps = 20;
// gravity (vY diminishing speed per second) 
const double gravity = 15;
// speed det per second 
const double normalSpeed = 15;
const double dashSpeed = 30;
// vY increment per jump  
const double jumpSpeed = 12;
// speed limits 
const double normalSpeedLimit = 8;
const double dashSpeedLimit = 12;
const double verticalSpeedLimit = 12;
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
