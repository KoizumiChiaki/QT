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
            if(movDir == DOWN)
            {
                vY -= normalSpeed / tps;
                if(vY < -verticalSpeedLimit)
                    vY = -verticalSpeedLimit;
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
            int p1=checkinblock(posX,posY),
                    p2=checkinblock(posX+playerheight,posY),
                    p3=checkinblock(posX,posY+playerheight),
                    p4=checkinblock(posX+playerheight,posY+playerheight);
            int cnt=p1+p2+p3+p4;
            if(!cnt)return;
            if(p1 && p2)
            {
                posY = int(posY) + 1;
                if(vY<0)vY = 0;//Down
            }
            if(p3 && p4)
            {
                posY = int(posY+playerheight) - playerheight;
                if(vY>0)vY = 0;//Up
            }
            if(p1 && p3)
            {
                posX = int(posX) + 1;
                if(vX<0)vX = 0;//Left
            }
            if(p2 && p4)
            {
                posX = int(posX+playerheight) - playerheight;
                if(vX>0)vX = 0;//Right
            }
            if(cnt>=2)return;
            if(p1)//leftdown
            {
                double L=int(posY)+1;
                double tmp=LX+(L-LY)*div(LX-posX,LY-posY);
                if(tmp<int(posX)||tmp>int(posX)+1)
                {
                    posX=int(posX)+1;
                    if(vX<0)vX=0;
                }
                else
                {
                    posY=L;
                    if(vY<0)vY=0;
                }
                return;
            }
            if(p3)//leftup
            {
                double L=int(posY+playerheight);
                double tmp=LX+(L-LY-playerheight)*div(LX-posX,LY-posY);
                if(tmp>=int(posX)&&tmp<=int(posX)+1)
                {
                    posY=L-playerheight;
                    if(vY>0)vY=0;
                }
                else
                {
                    posX=int(posX)+1;
                    if(vX<0)vX=0;
                }
                return;
            }
            if(p2)//rightdown
            {
                double L=int(posY)+1;
                double tmp=LX+playerheight+(L-LY)*div(LX-posX,LY-posY);
                if(tmp<int(posX+playerheight)||tmp>int(posX+playerheight)+1)
                {
                    posX=int(posX+playerheight)-playerheight;
                    if(vX>0)vX=0;
                }
                else
                {
                    posY=L;
                    if(vY<0)vY=0;
                }
                return;
            }
            if(p4)//rightup
            {
                double L=int(posY+playerheight);
                double tmp=LX+playerheight+(L-LY)*div(LX-posX,LY-posY);
                if(tmp>=int(posX+playerheight)&&tmp<=int(posX+playerheight)+1)
                {
                    posY=L-playerheight;
                    if(vY>0)vY=0;
                }
                else
                {
                    posX=int(posX+playerheight)-playerheight;
                    if(vX>0)vX=0;
                }
                return;
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
