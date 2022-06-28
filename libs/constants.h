#ifndef _QTHWK_CONSTANTS_H_
#define _QTHWK_CONSTANTS_H_

const int screenWidth = 48, screenHeight = 36;
const unsigned int tps = 20;
// gravity (vY diminishing speed per second) 
const double gravity = 3.5;
// speed det per second 
const double normalSpeed = 3;
const double dashSpeed = 5;
// vY increment per jump  
const double jumpSpeed = 5;
// speed limits 
const double normalSpeedLimit = 6;
const double dashSpeedLimit = 9;
const double verticalSpeedLimit = 6;
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
