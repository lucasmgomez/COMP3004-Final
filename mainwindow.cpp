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
    //power button slot signal connections
    connect(ui->powerButton,SIGNAL(pressed()),this,SLOT(powerPress()));
    connect(ui->powerButton,SIGNAL(released()),this,SLOT(powerRelease()));

    //up/down button slot signal connections
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

    //Turn device on, if device is off.
    if (oasis->getPower() == OFF)
    {
        qDebug() << "+turning device on: held " << elapsed << " seconds";
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

        return;
    }
    //other cases...
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
    if(oasis->getPower()==ON && oasis->getRunning()==false){
        if (shutdownCounter < 120)
        {
         shutdownCounter++;
         qDebug() << "              shutdown counter:" << shutdownCounter;
        }
        else
        {
            shutdownCounter = 0;
            oasis->turnOff();
            return;
        }
    }
    else if(oasis->getPower()==ON && oasis->getRunning()==true){
        //check how long a session is running for (or how much time is left)
    }
    oasis->useBattery();
}

//...
void MainWindow::softOff()
{

}
