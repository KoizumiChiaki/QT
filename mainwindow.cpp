#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QPushButton>
#include<QKeyEvent>
#include<QPainter>
#include<QPaintEvent>

#include"libs/gametick.h"
#include"libs/kbinput.h"
#include"libs/gamemap.h"

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
    setTheme(desert);//Just for testing
    gameMap.mapInit();
    QPixmap p = QPixmap::fromImage(gameMap.getWholeMap());
    painter.drawPixmap(0, 0, MainWindow::width(), MainWindow::height(), p);
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

