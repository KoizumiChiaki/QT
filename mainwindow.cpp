#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>

#include<QPushButton>
#include<QKeyEvent>
#include<QPainter>
#include<QPaintEvent>
#include<QDebug>
#include<QTimer>

#include"libs/gametick.h"
#include"libs/kbinput.h"
#include"libs/gamemap.h"
#include"libs/constants.h"

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(ev -> key() == Qt::Key_Q)
    {
        if(gameStatus != __gameTick::inGame)
            exit(0);
    }
    if(ev -> key() == Qt::Key_Space)
    {
        if(gameStatus == __gameTick::menu)
            startGame();
        if(gameStatus == __gameTick::endGame)
            backToMenu();
        if(gameStatus == __gameTick::inGame)
            pauseGame();
    }
    keyboardStatus.update(ev, true);
}
void MainWindow::keyReleaseEvent(QKeyEvent *ev)
{
    keyboardStatus.update(ev, false);
}
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if (gameStatus == __gameTick::menu)
    {
        QPixmap bg(MainWindow::width(), MainWindow::height());
        bg.fill(Qt::white);
        painter.drawPixmap(0, 0, MainWindow::width(), MainWindow::height(), bg);
    }
    if (gameStatus == __gameTick::inGame)//
    {
        QPixmap map = QPixmap::fromImage(gameMap.getWholeMap());
        QPixmap p1 = QPixmap::fromImage(__player::P1.GetPlayerState());
        QPixmap p2 = QPixmap::fromImage(__player::P2.GetPlayerState());
        int r1 = __player::P1.GetPlayerHPRate(), r2 = __player::P2.GetPlayerHPRate();
        QImage result(screenWidth * 16, screenHeight * 16, QImage::Format_RGBA8888);
        QPainter temp(&result);

        temp.drawPixmap(0, 0, map);// paint the map
        temp.drawPixmap(0, 0, p1);// paint player1
        temp.drawPixmap(0, 0, p2);// paint player2

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
        temp.drawRoundRect(barSideWidth, barSideHeight + barDistance, barWidth * r1 / 100, barHeight, barRoundX, barRoundY);
        temp.drawRoundRect(screenWidth * 16 - 1 - barSideWidth - barWidth * r2 / 100, barSideHeight + barDistance, barWidth * r2 / 100, barHeight, barRoundX, barRoundY);
        //paint MP(full ver)

        QImage h1("..\\QT\\resources\\images\\player\\stay.png");
        temp.drawPixmap(barSideWidth - headSize - 5, barSideHeight + barHeight - (headSize - barDistance) / 2 - 5, headSize, headSize, QPixmap::fromImage(h1));
        QImage h2("..\\QT\\resources\\images\\player\\stay.png");
        h2 = h2.mirrored(true, false);
        temp.drawPixmap(screenWidth * 16 - (barSideWidth - headSize - 5) - headSize, barSideHeight + barHeight - (headSize - barDistance) / 2 - 5, headSize, headSize, QPixmap::fromImage(h2));
        //paint head picture

        painter.drawPixmap(0, 0, MainWindow::width(), MainWindow::height(), QPixmap::fromImage(result));
        //print
    }
}


void MainWindow::startGame()
{
    keyboardStatus.clear();
    __player::P1.initialize(1, screenWidth / 3 * 1 - 0.5 * playerheight, screenHeight - 3);
    __player::P2.initialize(0, screenWidth / 3 * 2 - 0.5 * playerheight, screenHeight - 3);
    __gameTick::gameStatus = inGame;
    setTheme(nether);//Just for testing
    gameMap.mapInit();
    repaint();
}
void MainWindow::GlobalTick()
{
    if (__gameTick::gameStatus == inGame)
    {
        __gameTick::tick();
        repaint();
    }
    gameClock->start(timePerTick);
}
void MainWindow::backToMenu()
{
    // TODO
}
void MainWindow::pauseGame()
{
    // TODO
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

