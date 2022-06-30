#ifndef _QTHWK_CONSTANTS_H_
#define _QTHWK_CONSTANTS_H_

const int timePerTick = 15;
//the number of ms per tick
const int screenWidth = 48, screenHeight = 36;
const unsigned int tps = 60;
// gravity (vY diminishing speed per second) 
const double gravity = 200;
// speed det per second 
const double normalSpeed = 200;
const double dashSpeed = 600;
// vY increment per jump  
const double jumpSpeed = 40;
// speed limits 
const double normalSpeedLimit = 20;
const double dashSpeedLimit = 40;
const double particleLimit = 30;
const double verticalSpeedLimit = 40;
// jump cd 
const double playerheight = 0.8125;
const unsigned int jumpCoolDownTicks = 10;
const unsigned int dashCoolDownTicks = 10;
const unsigned int HurtCdTicks = 5;
// when not moving, Xspeed decrease alphaX times per tick
const double alphaX = 1.12;
const int MaxHp = 100;
const int MaxMp = 100;
const double TossSpeedX = 20;
const double TossSpeedY = 10;
const double ShootSpeed = 40;
const int BulletCdlimit=5;

const int barWidth = 200;
const int barHeight = 10;
const int barRoundX = 40;
const int barRoundY = 40;
const int barAlpha = 120;
const int barSideWidth = 50;
const int barSideHeight = 20;
const int barDistance = 16;
//HP & MP bar
const int headSize = 40;

const int themeNumber = 6;

#endif // _QTHWK_CONSTANTS_H_
