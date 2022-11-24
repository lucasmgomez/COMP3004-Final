#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    oasis = new Oasis();
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
    connect(ui->powerButton,SIGNAL(pressed()),this,SLOT(powerPress()));
    connect(ui->powerButton,SIGNAL(released()),this,SLOT(powerRelease()));

    connect(ui->intUpButton,SIGNAL(pressed()),this,SLOT(upPress()));
    connect(ui->intDownButton,SIGNAL(pressed()),this,SLOT(downPress()));
}

void MainWindow::powerPress(){
    qDebug() << "power pressed (counting milliseconds)";
    et->start();
}

void MainWindow::powerRelease(){
    float elapsed = et->elapsed() / 1000.00;
    qDebug() << "power released after:" << elapsed << " seconds";

    //Turn device on, if device is off.
    if (oasis->getPower() == OFF)
    {
        qDebug() << "+turning device on";
        oasis->turnOn();
        return;
    }
    //Device already on, turn off after holding power button for 1 second
    if (oasis->getPower() == ON && elapsed > 1)
    {

        qDebug() << "+turning device off, held button for:" << elapsed << "seconds.";
        oasis->turnOff();
        return;
    }
    //Device already on, change duration type ("If device in the correct mode")
    else if (oasis->getPower() == ON && elapsed < 1)
    {

        qDebug() << "      ++'tapped'for:" << elapsed << "seconds.changing session type:";
        //if (oasis->getDuration() != USERDESIGNATED)
        {
            oasis->nextDuration();
        }

//        // durations
//        #define TWENTY 1
//        #define FORTYFIVE 2
//        #define USERDESIGNATED 3

//        // types
//        #define MET 1
//        #define SUBDELTA 2
//        #define DELTA 3
//        #define THETA 4
//        #define ALPHA 5
//        #define SMR 6
//        #define BETA 7
//        #define HUNDREDHZ 8
        return;
    }
    //other cases...
}

void MainWindow::upPress(){
    oasis->nextType();
}
void MainWindow::downPress(){
    oasis->prevType();
}
void MainWindow::confirmPress(){

}

void MainWindow::toggleLeftEar(){

}
void MainWindow::toggleRightEar(){

}

void MainWindow::update(){

}
