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
        pen.setColor(QColor(0, 0, 0, 255));
        QBrush brush;
        brush.setColor(QColor(125, 45, 45, 255));
        brush.setStyle(Qt::SolidPattern);
        temp.setPen(pen), temp.setBrush(brush);
        temp.drawRect(20, 20, 200, 8);
        temp.drawRect(screenWidth * 16 - 221, 20, 200, 8);
        //paint HP(empty ver)
        brush.setColor(QColor(255, 0, 0, 255));
        temp.setBrush(brush);
        temp.drawRect(20, 20, 2 * r1, 8);
        temp.drawRect(screenWidth * 16 - 21 - 2 * r2, 20, 2 * r2, 8);
        //paint HP(full ver)
        painter.drawPixmap(0, 0, MainWindow::width(), MainWindow::height(), QPixmap::fromImage(result));
        //print
    }
}


void MainWindow::startGame()
{
    keyboardStatus.clear();
    __player::P1.initialize(1, screenWidth / 3 * 1 - 0.5 * playerheight, screenHeight - 1);
    __player::P2.initialize(0, screenWidth / 3 * 2 - 0.5 * playerheight, screenHeight - 1);
    __gameTick::gameStatus = inGame;
    setTheme(desert);//Just for testing
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

