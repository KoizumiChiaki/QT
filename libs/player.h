#ifndef _QTHWK_PLAYER_H_
#define _QTHWK_PLAYER_H_

#include "gamemap.h"
#include "gamemath.h"
#include <QDebug>

namespace __player
{
    double max(double x,double y)
    {return x>y?x:y;}
    double min(double x,double y)
    {return x<y?x:y;}
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
            vY-=gra/tps;
        }
        bool checkinblock()
        {
            return gameMap.getBlockType(int(posX),int(posY)) == solid;
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
        int DashCd;
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
                    jumpCount++;
                    jumpCoolDown = jumpCoolDownTicks;
                }
            }
            if(movDir == DOWN)
            {
                vY -= normalSpeed / tps;
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
                double L1=int(posY)+1,L2=int(posX)+1;
                double tmp1=LX+(L1-LY)*div(LX-posX,LY-posY);
                double tmp2=LY+(L2-LX)*div(LY-posY,LX-posX);
                double cp1=INF,cp2=INF;
                if(tmp1>=L2-1&&tmp1<=L2)cp1=div(LY-L1,LY-posY);
                if(tmp2>=L1-1&&tmp2<=L1)cp2=div(LX-L2,LX-posX);
                if(cp1<cp2)
                {
                    posY=L1;
                    if(vY<0)vY=0;
                }
                else
                {
                    posX=L2;
                    if(vX<0)vX=0;
                }
                return;
            }
            if(p2)//rightdown
            {
                double L1=int(posY)+1,L2=int(posX+playerheight);
                double tmp1=LX+playerheight+(L1-LY)*div(LX-posX,LY-posY);
                double tmp2=LY+(L2-LX-playerheight)*div(LY-posY,LX-posX);
                double cp1=INF,cp2=INF;
                if(tmp1>=L2&&tmp1<=L2+1)cp1=div(LY-L1,LY-posY);
                if(tmp2>=L1-1&&tmp2<=L1)cp2=div(LX+playerheight-L2,LX-posX);
                if(cp1<cp2)
                {
                    posY=L1;
                    if(vY<0)vY=0;
                }
                else
                {
                    posX=L2-playerheight;
                    if(vX>0)vX=0;
                }
                return;
            }
            if(p3)//leftup
            {
                double L1=int(posY+playerheight+eps),L2=int(posX)+1;
                if(checkinblock(LX+vX/tps,LY+playerheight)&&!checkinblock(LX,LY+playerheight+vY/tps))
                {
                    vX=0;
                    posX=L2;
                    return;
                }
                if(checkinblock(LX,LY+playerheight+vY/tps)&&!checkinblock(LX+vX/tps,LY+playerheight))
                {
                    vY=0;
                    posY=L1-playerheight;
                    return;
                }
                if(checkinblock(LX+vX/tps,LY+playerheight)&&checkinblock(LX,LY+playerheight+vY/tps))
                {
                    vX=vY=0;
                    posX=L2,posY=L1-playerheight;
                    return;
                }
                return;
            }

            if(p4)//rightup
            {
                double L1=int(posY+playerheight+eps),L2=int(posX+playerheight+eps);
                if(checkinblock(LX+playerheight+vX/tps,LY+playerheight)&&!checkinblock(LX+playerheight,LY+playerheight+vY/tps))
                {
                    vX=0;
                    posX=L2-playerheight;
                    return;
                }
                if(checkinblock(LX+playerheight,LY+playerheight+vY/tps)&&!checkinblock(LX+playerheight+vX/tps,LY+playerheight))
                {
                    vY=0;
                    posY=L1-playerheight;
                    return;
                }
                if(checkinblock(LX+playerheight+vX/tps,LY+playerheight)&&checkinblock(LX+playerheight,LY+playerheight+vY/tps))
                {
                    vX=vY=0;
                    posX=L2-playerheight,posY=L1-playerheight;
                    return;
                }
                return;
            }
        }
        void updatedirection(bool p1,bool p2)
        {
            if(p1&&!p2&&Direction)Direction=0;
            if(!p1&&p2&&!Direction)Direction=1;
        }
        // check if the player is on ground
        bool onGround()
        {
            if(checkinblock(posX+eps,posY-eps)||checkinblock(posX+playerheight-eps,posY-eps))return true;
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
            if(DashCd)DashCd--;
            if(DashCd<10)inDash=0;
            double LposX = posX,LposY = posY;
            if(!inDash)vY -= gravity / tps;
            if(vY < -verticalSpeedLimit)vY = -verticalSpeedLimit;
            posX += vX / tps;if(!inDash)posY += vY / tps;
            fixStatus(LposX,LposY);
            if(onGround())jumpCount = 0;
        }
        void Toss(int tmp)
        {
            if(HurtCd)return;
            L.push_back((bullet){
                    posX+playerheight/2,
                    posY+playerheight,
                    Direction==1?std::max(TossSpeedX+vX,TossSpeedX):std::min(-TossSpeedX,-TossSpeedX+vX),                      TossSpeedY,
                    gravity,
                    tmp});
        }
        void Shoot(int tmp)
        {
            if(HurtCd)return;
            L.push_back((bullet){posX+playerheight/2,posY+playerheight,Direction==1?ShootSpeed:-ShootSpeed,0,0,tmp});
        }
        void Dash(bool p1,bool p2)
        {
            if(DashCd||HurtCd)return;
            else if(p1&&!p2){vX=-dashSpeedLimit;}
            else if(!p1&&p2){vX=dashSpeedLimit;}
            else {vX=(Direction==1?dashSpeedLimit:-dashSpeedLimit);}
            vY=0;
            inDash=1;DashCd=dashCoolDownTicks;
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

using __player::bullet;
using __player::L;
using __player::P1;
using __player::P2;
using __player::direction;
using __player::LEFT;
using __player::RIGHT;
using __player::UP;
using __player::DOWN;

#endif // _QTHWK_PLAYER_H_
