#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initConnections();
    initTimer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTimer()
{
    t = new QTimer(this);
    et = new QElapsedTimer();
}


void MainWindow::initConnections()
{
    //elapsedButtonPressed will be the powerButton.
    //connect(ui->elapsedButtonTest,SIGNAL(pressed()),this,SLOT(powerPress()));
    //connect(ui->elapsedButtonTest,SIGNAL(released()),this,SLOT(powerRelease()));
}

void MainWindow::powerPress(){
    qDebug() << "power pressed (counting milliseconds)";
    et->start();
}

void MainWindow::powerRelease(){
    qDebug() << "power released after:" << et->elapsed() << " milliseconds";

}

void MainWindow::upPress(){

}
void MainWindow::downPress(){

}
void MainWindow::confirmPress(){

}

void MainWindow::toggleLeftEar(){

}
void MainWindow::toggleRightEar(){

}

void MainWindow::update(){

}
