#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>

#include<QPushButton>
#include<QKeyEvent>
#include<QPainter>
#include<QPaintEvent>
#include<QDebug>
#include<QTimer>

#include<list>
using std::list;

#include"libs/constants.h"
#include"libs/gamemap.h"
#include"libs/gametick.h"
#include"libs/kbinput.h"

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(ev -> key() == Qt::Key_Q)
    {
        if(gameStatus != __gameTick::inGame)
            exit(0);
    }
    if(ev -> key() == Qt::Key_R)
    {
        if(gameStatus == __gameTick::paused || gameStatus == __gameTick::endGame)
            backToMenu();
    }
    if(ev -> key() == Qt::Key_Space)
    {
        switch (gameStatus)
        {
            case __gameTick::endGame:
                backToMenu();
            break;
            case __gameTick::inGame:
                pauseGame();
            break;
            case __gameTick::paused:
                resumeGame();
            break;
            default:
            break;
        }
    }
    if(gameStatus == __gameTick::menu)
    {
        if(ev -> key() == Qt::Key_1)
        {
            startGame(1);
        }
        if(ev -> key() == Qt::Key_2)
        {
            startGame(2);
        }
        if(ev -> key() == Qt::Key_3)
        {
            startGame(3);
        }
        if(ev -> key() == Qt::Key_4)
        {
            startGame(4);
        }
        if(ev -> key() == Qt::Key_5)
        {
            startGame(5);
        }
        if(ev -> key() == Qt::Key_6)
        {
            startGame(6);
        }
    }
    keyboardStatus.update(ev, true);
}
void MainWindow::keyReleaseEvent(QKeyEvent *ev)
{
    keyboardStatus.update(ev, false);
}

QImage MapSolid(){return gameMap.getWholeMapSolid();}
QImage MapLiquid(){return gameMap.getWholeMapLiquid();}
QImage PlayerState(int r1, int r2, int w1, int w2)//HP & MP 's percent
{
    QImage ret(screenWidth * 16, screenHeight * 16, QImage::Format_RGBA8888);
    QPainter temp(&ret);
    QPen pen;
    pen.setColor(QColor(0, 0, 0, barAlpha));
    QBrush brush;
    brush.setColor(QColor(125, 45, 45, barAlpha));// Back Color
    brush.setStyle(Qt::SolidPattern);
    temp.setPen(pen), temp.setBrush(brush);

    temp.drawRoundRect(barSideWidth, barSideHeight, barWidth, barHeight, barRoundX, barRoundY);
    temp.drawRoundRect(screenWidth * 16 - 1 - barSideWidth - barWidth, barSideHeight, barWidth, barHeight, barRoundX, barRoundY);
    //paint HP(empty ver)
    temp.drawRoundRect(barSideWidth, barSideHeight + barDistance, barWidth, barHeight, barRoundX, barRoundY);
    temp.drawRoundRect(screenWidth * 16 - 1 - barSideWidth - barWidth, barSideHeight + barDistance, barWidth, barHeight, barRoundX, barRoundY);
    //paint MP(empty ver)
    brush.setColor(QColor(255, 0, 0, barAlpha));// Red
    temp.setBrush(brush);
    temp.drawRoundRect(barSideWidth, barSideHeight, barWidth * r1 / 100, barHeight, barRoundX, barRoundY);
    temp.drawRoundRect(screenWidth * 16 - 1 - barSideWidth - barWidth * r2 / 100, barSideHeight, barWidth * r2 / 100, barHeight, barRoundX, barRoundY);
    //paint HP(full ver)
    brush.setColor(QColor(0, 0, 255, barAlpha));// Blue
    temp.setBrush(brush);
    temp.drawRoundRect(barSideWidth, barSideHeight + barDistance, barWidth * w1 / 100, barHeight, barRoundX, barRoundY);
    temp.drawRoundRect(screenWidth * 16 - 1 - barSideWidth - barWidth * w2 / 100, barSideHeight + barDistance, barWidth * w2 / 100, barHeight, barRoundX, barRoundY);
    //paint MP(full ver)
    std::string dir1 = "..\\QT\\resources\\images\\player\\";
    std::string dir2 = "..\\QT\\resources\\images\\player\\";
    if (r1 > 50)dir1 += "stay.png";
    else
        if (r1 > 0)dir1 += "injured_stay.png";
        else dir1 += "dead.png";
    if (r2 > 50)dir2 += "stay.png";
    else
        if (r2 > 0)dir2 += "injured_stay.png";
        else dir2 += "injured_dead.png";
    QImage h1(dir1.c_str());
    temp.drawPixmap(barSideWidth - headSize - 5, barSideHeight + barHeight - (headSize - barDistance) / 2 - 5, headSize, headSize, QPixmap::fromImage(h1));
    QImage h2(dir2.c_str());
    h2 = h2.mirrored(true, false);
    temp.drawPixmap(screenWidth * 16 - (barSideWidth - headSize - 5) - headSize, barSideHeight + barHeight - (headSize - barDistance) / 2 - 5, headSize, headSize, QPixmap::fromImage(h2));
    //paint head picture
    return ret;
}
QImage AllBullet()
{
    QImage ret(screenWidth * 16, screenHeight * 16, QImage::Format_RGBA8888);
    QPainter temp(&ret);
    QPixmap img("..\\QT\\resources\\images\\effects\\bullet.png");
    list<__player::bullet>::iterator it = __player::Bullet.begin();
    while (it != __player::Bullet.end())
    {
        temp.drawPixmap((int)round((it -> posX) * 16 - 3), (int)round(screenHeight * 16 - 1 - (it -> posY) * 16 + 3), 6, 6, img);
        it++;
    }
    return ret;
}
QImage AllParticle()
{
    QImage ret(screenWidth * 16, screenHeight * 16, QImage::Format_RGBA8888);
    QPainter temp(&ret);
    QPixmap img("..\\QT\\resources\\images\\effects\\particle.png");
    list<__player::particle>::iterator it = __player::Particle.begin();
    while (it != __player::Particle.end())
    {
        temp.drawPixmap((int)round((it -> posX) * 16 - 1), (int)round(screenHeight * 16 - 1 - (it -> posY) * 16 + 1), 2, 2, img);
        it++;
    }
    return ret;
}
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if (gameStatus == __gameTick::menu)
    {
        QPixmap bg("..\\QT\\resoureces\\images\\other\\start.png");
        painter.drawPixmap(0, 0, MainWindow::width(), MainWindow::height(), bg);
        return;
    }
    if (gameStatus == __gameTick::inGame || gameStatus == __gameTick::paused || gameStatus == __gameTick::endGame)//
    {
        QPixmap ms = QPixmap::fromImage(MapSolid()); //solid map
        QPixmap ml = QPixmap::fromImage(MapLiquid()); //liquid map
        QPixmap p1 = QPixmap::fromImage(__player::P1.GetPlayerState()); //player 1
        QPixmap p2 = QPixmap::fromImage(__player::P2.GetPlayerState()); //player 2
        QPixmap bl = QPixmap::fromImage(AllBullet());//bullet
        QPixmap pc = QPixmap::fromImage(AllParticle());//particle
        int r1 = __player::P1.GetPlayerHPRate(), r2 = __player::P2.GetPlayerHPRate();
        int w1 = __player::P1.GetPlayerMPRate(), w2 = __player::P2.GetPlayerMPRate();
        QPixmap st = QPixmap::fromImage(PlayerState(r1, r2, w1, w2));//state
        QImage result(screenWidth * 16, screenHeight * 16, QImage::Format_RGBA8888);
        QPainter temp(&result);
        temp.drawPixmap(0, 0, ms);// paint the map solid
        temp.drawPixmap(0, 0, p1);// paint player1
        temp.drawPixmap(0, 0, p2);// paint player2
        temp.drawPixmap(0, 0, ml);// paint the map liquid
        temp.drawPixmap(0, 0, bl);// paint the bullet
        temp.drawPixmap(0, 0, pc);// paint the particle
        temp.drawPixmap(0, 0, st);// paint players' state
        if (gameStatus == __gameTick::paused)
        {
            QPixmap ps("..\\QT\\resources\\images\\other\\pause.png");
            temp.drawPixmap(0, 0, ps);// paint the pause picture
        }
        if (gameStatus == __gameTick::endGame)
        {
            std::string dir = "..\\QT\\resoureces\\images\\other\\over";
            if (r1 == 0)
                dir += "2.png";
            else
                dir += "1.png";
            QPixmap ps(dir.c_str());
            temp.drawPixmap(0, 0, ps);// paint the end picture
        }
        painter.drawPixmap(0, 0, MainWindow::width(), MainWindow::height(), QPixmap::fromImage(result));
        //print
    }
}


void MainWindow::startGame(int gameThemeId)
{
    keyboardStatus.clear();
    __player::P1.initialize(1, screenWidth / 3 * 1 - 0.5 * playerheight, screenHeight - 2);
    __player::P2.initialize(0, screenWidth / 3 * 2 - 0.5 * playerheight, screenHeight - 2);
    __gameTick::gameStatus = inGame;
    if(gameThemeId == 1)setTheme(plain);
    if(gameThemeId == 2)setTheme(desert);
    if(gameThemeId == 3)setTheme(ocean);
    if(gameThemeId == 4)setTheme(mountains);
    if(gameThemeId == 5)setTheme(nether);
    if(gameThemeId == 6)setTheme(the_end);
    gameMap.mapInit();
    update();
}
void MainWindow::GlobalTick()
{
    if (__gameTick::gameStatus == inGame)
        __gameTick::tick();
    update();
    gameClock->start(timePerTick);
}
void MainWindow::backToMenu()
{
    __gameTick::gameStatus = menu;
    // printMenu();
}
void MainWindow::pauseGame()
{
    __gameTick::gameStatus = paused;
}

void MainWindow::resumeGame()
{
    __gameTick::gameStatus = inGame;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , gameClock(new QTimer(this))
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1280,960);
    gameClock->start(timePerTick);
    connect(gameClock, SIGNAL(timeout()), this, SLOT(GlobalTick()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

