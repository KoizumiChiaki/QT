#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QPushButton>
#include<QKeyEvent>
#include<QPainter>
#include<QPaintEvent>

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
    if (true)//
    {
        setTheme(desert);//Just for testing
        gameMap.mapInit();
        QPixmap map = QPixmap::fromImage(gameMap.getWholeMap());
        QPixmap p1 = QPixmap::fromImage(__gameTick::P1.GetPlayerState());
        QPixmap p2 = QPixmap::fromImage(__gameTick::P2.GetPlayerState());
        painter.drawPixmap(0, 0, MainWindow::width(), MainWindow::height(), map);
        painter.drawPixmap(0, 0, MainWindow::width(), MainWindow::height(), p1);
        painter.drawPixmap(0, 0, MainWindow::width(), MainWindow::height(), p2);
    }
}


void MainWindow::startGame()
{
    keyboardStatus.clear();
    __gameTick::P1.initialize(1, 16 - 0.5 * playerheight, 18);
    __gameTick::P2.initialize(0, 32 - 0.5 * playerheight, 18);
    update();
    __gameTick::gameStatus = inGame;
    int lastTickTime = clock();
    while(true)
    {
        if(gameStatus != inGame) break;
        int __Time = int(clock());
        if(__Time >= lastTickTime + int(CLOCKS_PER_SEC / tps))
        {
            lastTickTime = __Time;
            //__gameTick::tick();
            update();
        }
    }
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
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

