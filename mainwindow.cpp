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
        QPixmap p1 = QPixmap::fromImage(P1.GetPlayerState());
        QPixmap p2 = QPixmap::fromImage(P2.GetPlayerState());
        painter.drawPixmap(0, 0, MainWindow::width(), MainWindow::height(), map);
        painter.drawPixmap(0, 0, MainWindow::width(), MainWindow::height(), p1);
        painter.drawPixmap(0, 0, MainWindow::width(), MainWindow::height(), p2);
    }
}


void MainWindow::startGame()
{
    keyboardStatus.clear();
    P1.initialize(1, screenWidth / 3 * 1 - 0.5 * playerheight, screenHeight - 1);
    P2.initialize(0, screenWidth / 3 * 2 - 0.5 * playerheight, screenHeight - 1);
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

