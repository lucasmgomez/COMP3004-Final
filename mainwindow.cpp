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

    //call the update function per second
    connect(t, SIGNAL(timeout()), this, SLOT(update()));
    t->start(1000);
}


void MainWindow::initConnections()
{
    //power button
    connect(ui->powerButton,SIGNAL(pressed()),this,SLOT(powerPress()));
    connect(ui->powerButton,SIGNAL(released()),this,SLOT(powerRelease()));

    //up/down button
    //connect(ui->intUpButton,SIGNAL(clicked()),this,SLOT(upPress()));
    connect(ui->intUpButton,SIGNAL(pressed()),this,SLOT(upPress()));
    connect(ui->intDownButton,SIGNAL(pressed()),this,SLOT(downPress()));

    //confirm press
    connect(ui->startButton,SIGNAL(pressed()),this,SLOT(confirmPress()));

    //left/right ear toggle
}

void MainWindow::powerPress(){
    qDebug() << "power pressed (counting milliseconds)";
    et->start();
    //case for if user never releases power button?
}

void MainWindow::powerRelease(){

    float elapsed = et->elapsed() / 1000.00;

    //Turn on
    if (oasis->getPower() == OFF){
        qDebug() << "+turning device on: held " << elapsed << " seconds";
        oasis->turnOn();
        return;
    }
    //Force turn off
    if (oasis->getPower() == ON && elapsed > 1){
        qDebug() << "+turning device off, held button for:" << elapsed << "seconds.";
        oasis->turnOff();
        return;
    }
    //Change duration
    else if (oasis->getPower() == ON && elapsed < 1){
        qDebug() << "      ++'tapped'for:" << elapsed << "seconds.changing session type:";   
        {
            oasis->nextDuration();
        }
        return;
    }
}

void MainWindow::upPress(){
    if (oasis->getRunning()==false)
    {
        oasis->nextType();
    }
    else
    {
        oasis->nextIntensity();
    }
}
void MainWindow::downPress(){
    if (oasis->getRunning()==false)
    {
        oasis->prevType();
    }
    else
    {
        oasis->prevIntensity();
    }


}
void MainWindow::confirmPress(){
    oasis->runSession(); //runSession checks if connection > 0 before running
}

void MainWindow::toggleLeftEar(){
    oasis->toggleLeftEar();
}
void MainWindow::toggleRightEar(){
    oasis->toggleRightEar();
}

void MainWindow::update(){
    //reset counters if power off
    if(oasis->getPower()==OFF){
        if (shutdownCounter > 0){shutdownCounter=0;}
        if (sessionRunTime > 0){sessionRunTime=0;}
        return;
    }
    //idle shutdown counter
    if(oasis->getPower()==ON && oasis->getRunning()==false){
        if (shutdownCounter < 120){
         shutdownCounter++;
         qDebug() << "              shutdown counter:" << shutdownCounter;
        }
        else{
            shutdownCounter = 0;
            oasis->turnOff();
            return;
        }
    }
    //session runtime counter
    else if(oasis->getPower()==ON && oasis->getRunning()==true){
        if (shutdownCounter>0){shutdownCounter=0;}//reset idle shutdown counter
        /*
        if (oasis->getD){
        }
        else if(){
        }
        else if(){
        }
        */
        sessionRunTime++; //session runtime counter
    }
    //drain battery
    oasis->useBattery();
}

//...
void MainWindow::softOff()
{

}
