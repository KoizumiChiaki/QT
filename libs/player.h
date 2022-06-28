#ifndef _QTHWK_PLAYER_H_
#define _QTHWK_PLAYER_H_

#include "gamemap.h"
#include "gamemath.h"

namespace __player
{
    enum direction
    {LEFT, RIGHT, UP, DOWN};
    class bullet
    {
    public:
        double posX, posY;
        double vX, vY;
        double gra;
        int owner;
        void move()
        {
            posX+=vX/tps;
            posY+=vY/tps;
            vY+=gra/tps;
        }
    };
    std::list<bullet>L;

    class player
    {
    public:
        // Hp
        int Hp,Mp;
        bool Direction;//0 Left 1 Right
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
        // initialize player
        int HurtCd;
        // TODO:
        // Hp, Mp
        // Special CD (if have)
        // ...
        void initialize(int D,int X,int Y)
        {
            posX = X,posY = Y,Direction = D;
            vX = 0, vY = 0;
            jumpCount = 0, jumpCoolDown = 0;
            Hp = MaxHp , Mp = MaxMp;
            inDash = false;
        }

        // add speed to the player
        void addMove(const direction movDir)
        {
            if(HurtCd||Hp<=0)return;
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
                    vY = jumpSpeed;
                    if(vY > verticalSpeedLimit)
                        vY = verticalSpeedLimit;
                    jumpCount++;
                    jumpCoolDown = jumpCoolDownTicks;
                }
            }
        }
        // check if a real position is in block
        bool checkinblock(double X,double Y)
        {
            return gameMap.getBlockType(int(X),int(Y)) == solid;
        }
        // fix player position when hitting blocks
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
        void updatedirection()
        {
            if(Direction==1&&vX<0)Direction=0;
            if(Direction==0&&vX>0)Direction=1;
        }
        // check if the player is on ground
        bool onGround()
        {
            if(checkinblock(posX,posY-eps)||checkinblock(posX+playerheight,posY-eps))return true;
            return false;
        }
        // decrease X exponently
        void diminishX()
        {
            vX = vX / alphaX;
        }
        void move()
        {
            if(jumpCoolDown) jumpCoolDown--;
            if(HurtCd) HurtCd--;
            double LposX = posX,LposY = posY;
            posX += vX / tps, posY += vY / tps;
            vY -= gravity / tps;
            fixStatus(LposX,LposY);
            if(onGround())jumpCount = 0;
        }
        void Toss(int tmp)
        {
            if(HurtCd)return;
            L.push_back((bullet){posX+playerheight/2,posY+playerheight/2,TossSpeed,0,gravity,tmp});
        }
        int GetPlayerHPRate(){return Hp * 100 / MaxHp;}
        QImage GetPlayerState()
        {
            QImage ret(screenWidth * 16, screenHeight * 16, QImage::Format_RGBA8888);
            std::string dir = "..\\QT\\resources\\images\\player\\";
            if(Hp<=MaxHp/2)dir+="injured_";
            if(Hp<=0)dir+="dead";
            else if(HurtCd)dir+="hurt";
            else if(!onGround())
            {
                if(vY>=0)dir+="jump";
                else dir+="fall";
            }
            else dir+="stay";
            QImage tmp;
            tmp.load((dir+".png").c_str());
            if (Direction == 0)
                tmp = tmp.mirrored(true, false);
            QPainter painter(&ret);
            painter.drawPixmap((int)round(posX * 16), (int)round((screenHeight - posY) * 16) - 16 * playerheight, 16 * playerheight, 16 * playerheight, QPixmap::fromImage(tmp));
            return ret;
        }

    }P1, P2;
}

using __player::P1;
using __player::P2;
using __player::direction;
using __player::LEFT;
using __player::RIGHT;
using __player::UP;
using __player::DOWN;

#endif // _QTHWK_PLAYER_H_
