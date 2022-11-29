#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

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
    connect(ui->intUpButton,SIGNAL(pressed()),this,SLOT(upPress()));
    connect(ui->intDownButton,SIGNAL(pressed()),this,SLOT(downPress()));

    //confirm press
    connect(ui->startButton,SIGNAL(pressed()),this,SLOT(confirmPress()));

    //left/right ear toggle

    //battery change
    connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(setBatteryUI()));
    connect(ui->saveButton,SIGNAL(pressed()),this,SLOT(savePress()));
    connect(ui->replayButton,SIGNAL(pressed()),this,SLOT(replayPress()));
    connect(ui->updateList,SIGNAL(pressed()),this,SLOT(updateList()));

}

void MainWindow::updateList(){
    int cur=ui->currentUser->toPlainText().toInt();
    oasis->setUser(cur);
    User* user=oasis->getUser();
    ui->sessionHistory->clear();
    for (int i = 0; i < user->getnumRecord(); i++) {
        Session* sess= user->replay(i);
        std::string str = "Session #:"+std::to_string(i)+" duration:"+std::to_string(sess->getDuration())+" type:"+std::to_string(sess->getType())+" intesity:"+std::to_string(sess->getIntensity());
        QString qstr = QString::fromStdString(str);
        ui->sessionHistory->append(qstr);
    }
}

void MainWindow::savePress(){
    if (oasis->getPower()){
        if (ui->currentUser->document()->isEmpty()){
            qDebug() << "Please specify user #";
            return;
        }
        oasis->setUser(ui->currentUser->toPlainText().toInt());
        oasis->record();
        oasis->endSession(); // for testing needs to be remove when endSession implement inside Oasis
        updateList();
    }
}

void MainWindow::replayPress(){
    if (oasis->getPower()){
        if (ui->replaySes->document()->isEmpty()){
            qDebug() << "Please specify user # and replay session #";
            return;
        }
        oasis->setUser(ui->currentUser->toPlainText().toInt());
        oasis->replay(ui->replaySes->toPlainText().toInt());
    }
}

void MainWindow::powerPress(){
    qDebug() << "power pressed (counting milliseconds)";
    et->start();
}

void MainWindow::powerRelease(){
    float elapsed = et->elapsed() / 1000.00;

    //Turn device on, if device is off.
    if (oasis->getPower() == OFF)
    {
        // Turn on lights to default
        setDefaultLEDs(true);
        //setConnectLEDs();

        qDebug() << "+turning device on: held " << elapsed << " seconds";
        oasis->turnOn();
        return;
    }
    //Device already on, turn off after holding power button for 1 second
    if (oasis->getPower() == ON && elapsed > 1)
    {
        setDefaultLEDs(false);
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
            Session* sess = oasis->getCurrSession();
            updateDurUI(sess->getDuration());
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
    if (oasis->getRunning()==false)
    {
        oasis->nextType();
        Session* sess = oasis->getCurrSession();
        updateTypeUI(sess->getType());
    }
    else
    {
        oasis->nextIntensity();
        Session* sess = oasis->getCurrSession();
        updateIntUI(sess->getIntensity());

    }
}
void MainWindow::downPress(){
    if (oasis->getRunning()==false)
    {
        oasis->prevType();
        Session* sess = oasis->getCurrSession();
        updateTypeUI(sess->getType());
    }
    else
    {
        oasis->prevIntensity();
        Session* sess = oasis->getCurrSession();
        updateIntUI(sess->getIntensity());
    }


}
void MainWindow::confirmPress(){
    oasis->setConnection(ui->connectBox->currentIndex());
    setConnectLEDs(ui->connectBox->currentIndex());
    delay(2);
    updateIntUI(1);

    oasis->runSession();
}

void MainWindow::toggleLeftEar(){

}
void MainWindow::toggleRightEar(){

}

void MainWindow::update(){
    //insert battery drain here
    oasis->useBattery();
}

void MainWindow::updateDurUI(int dur){
    if (dur == TWENTY){
        auto time20LED = findChild<QWidget*>("time20LED");
        time20LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(0, 255, 0, 100);width: 20px;}");
        auto time45LED = findChild<QWidget*>("time45LED");
        time45LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        auto timeUserLED = findChild<QWidget*>("timeUserLED");
        time45LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
    }
    else if (dur == FORTYFIVE){
        auto time20LED = findChild<QWidget*>("time20LED");
        time20LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        auto time45LED = findChild<QWidget*>("time45LED");
        time45LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(0, 255, 0, 100);width: 20px;}");
        auto timeUserLED = findChild<QWidget*>("timeUserLED");
        timeUserLED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
    }
    else{
        // user set duration
        return;
    }

}

void MainWindow::updateTypeUI(int type){
    if (type == MET){
        auto freq1LED = findChild<QWidget*>("freq1LED");
        freq1LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(0, 255, 0, 100);width: 20px;}");
        auto freq2LED = findChild<QWidget*>("freq2LED");
        freq2LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        auto freq3LED = findChild<QWidget*>("freq3LED");
        freq3LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        auto freq4LED = findChild<QWidget*>("freq4LED");
        freq4LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
    }
    else if (type == SUBDELTA){
        auto freq1LED = findChild<QWidget*>("freq1LED");
        freq1LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        auto freq2LED = findChild<QWidget*>("freq2LED");
        freq2LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(0, 255, 0, 100);width: 20px;}");
        auto freq3LED = findChild<QWidget*>("freq3LED");
        freq3LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        auto freq4LED = findChild<QWidget*>("freq4LED");
        freq4LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
    }
    else if (type == DELTA){
        auto freq1LED = findChild<QWidget*>("freq1LED");
        freq1LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        auto freq2LED = findChild<QWidget*>("freq2LED");
        freq2LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        auto freq3LED = findChild<QWidget*>("freq3LED");
        freq3LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(0, 255, 0, 100);width: 20px;}");
        auto freq4LED = findChild<QWidget*>("freq4LED");
        freq4LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
    }
    else if (type == THETA){
        auto freq1LED = findChild<QWidget*>("freq1LED");
        freq1LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        auto freq2LED = findChild<QWidget*>("freq2LED");
        freq2LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        auto freq3LED = findChild<QWidget*>("freq3LED");
        freq3LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        auto freq4LED = findChild<QWidget*>("freq4LED");
        freq4LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(0, 255, 0, 100);width: 20px;}");
    }
}

void MainWindow::updateIntUI(int j){
    auto lcdInt = findChild<QLCDNumber*>("lcdInt"+QString::number(j));
    lcdInt->setStyleSheet("QLCDNumber {background-color: rgba(201, 155, 28, 70);width: 20px;}");

    for (int i = 1; i <= 8; ++i){
        auto lcdInt = findChild<QLCDNumber*>("lcdInt"+QString::number(i));
        if (i != j){
            lcdInt->setStyleSheet("QLCDNumber {background-color: rgba(0, 0, 0, 70); width: 20px;}");
        }
    }
}

void MainWindow::setDefaultLEDs(bool on){
    if (on){
        auto powerLED = findChild<QWidget*>("powerLED");
        powerLED->setStyleSheet("QWidget {border: 2px solid grey; border-radius: 5px; background-color: rgba(0, 255,0, 100); width: 20px; }");
        auto time20LED = findChild<QWidget*>("time20LED");
        time20LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(0, 255, 0, 100);width: 20px;}");
        auto freq1LED = findChild<QWidget*>("freq1LED");
        freq1LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(0, 255, 0, 100);width: 20px;}");

        for (int i = 1; i <= 8; ++i){
            auto lcdInt = findChild<QLCDNumber*>("lcdInt"+QString::number(i));
            if (i==1){
                lcdInt->setStyleSheet("QLCDNumber {background-color: rgba(201, 155, 28, 70);width: 20px;}");
            }
            else{
                lcdInt->setStyleSheet("QLCDNumber {background-color: rgba(0, 0, 0, 70);width: 20px;}");
            }
        }
    }
    if (!on){
        auto powerLED = findChild<QWidget*>("powerLED");
        powerLED->setStyleSheet("QWidget {border: 2px solid grey; border-radius: 5px; background-color: rgba(255, 255,255, 100); width: 20px; }");
        auto time20LED = findChild<QWidget*>("time20LED");
        time20LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        auto time45LED = findChild<QWidget*>("time45LED");
        time45LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        auto freq1LED = findChild<QWidget*>("freq1LED");
        freq1LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        auto freq2LED = findChild<QWidget*>("freq2LED");
        freq2LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        auto freq3LED = findChild<QWidget*>("freq3LED");
        freq3LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        auto freq4LED = findChild<QWidget*>("freq4LED");
        freq4LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");

        auto leftLED = findChild<QLabel*>("leftLED");
        leftLED->setStyleSheet("QLabel {color: rgba(255, 255,255, 100);}");
        auto rightLED = findChild<QLabel*>("rightLED");
        rightLED->setStyleSheet("QLabel {color: rgba(255, 255,255, 100);}");

        auto shortWaveLED = findChild<QLabel*>("shortWaveLED");
        shortWaveLED->setStyleSheet("QLabel {color: rgba(255, 255,255, 100);}");
        auto otherWaveLED = findChild<QLabel*>("otherWaveLED");
        otherWaveLED->setStyleSheet("QLabel {color: rgba(255, 255,255, 100);}");

        for (int i = 1; i <= 8; ++i){
            auto lcdInt = findChild<QLCDNumber*>("lcdInt"+QString::number(i));
              lcdInt->setStyleSheet("QLCDNumber {background-color: rgba(0, 0, 0, 70);width: 20px;}");
            }
        }
}

void MainWindow::setConnectLEDs(int level){
    if (level == NO){
        for (int i = 1; i<= 8; ++i){
             auto lcdInt = findChild<QLCDNumber*>("lcdInt"+QString::number(i));
            if (i < 3){
                lcdInt->setStyleSheet("QLCDNumber {background-color: rgba(255, 0, 0, 70);width: 20px;}");
            }
            else{
                lcdInt->setStyleSheet("QLCDNumber {background-color: rgba(0, 0, 0, 70);width: 20px;}");
            }
        }
    }
    else if (level == OKAY){
        for (int i = 1; i<= 8; ++i){
             auto lcdInt = findChild<QLCDNumber*>("lcdInt"+QString::number(i));
            if (3 < i && i <= 6){
                lcdInt->setStyleSheet("QLCDNumber {background-color: rgba(201, 155, 28, 70);width: 20px;}");
            }
            else{
                lcdInt->setStyleSheet("QLCDNumber {background-color: rgba(0, 0, 0, 70);width: 20px;}");
            }
        }
    }
    else if (level == EXCELLENT){
        for (int i = 1; i <= 8; ++i){
            auto lcdInt = findChild<QLCDNumber*>("lcdInt"+QString::number(i));
            if (i > 6){
                lcdInt->setStyleSheet("QLCDNumber {background-color: rgba(0, 255, 0, 70);width: 20px;}");
            }
            else{
                lcdInt->setStyleSheet("QLCDNumber {background-color: rgba(0, 0, 0, 70);width: 20px;}");
            }
        }
    }
}

void MainWindow::setBatteryUI(){

    auto batteryEdit = findChild<QLineEdit*>("batteryEdit");
    /*QRegularExpression re("\\d*");  // a digit (\d), zero or more times (*)
    if (re.anchoredPattern(batteryEdit->text())){

    }*/
    auto battBar1 = findChild<QWidget*>("battBar1");
    auto battBar2 = findChild<QWidget*>("battBar2");
    auto battBar3 = findChild<QWidget*>("battBar3");
    int level = batteryEdit->text().toInt();
    if (level > 70){
        // change battery display to full and green
        battBar1->setStyleSheet("QWidget {background-color: rgba(0, 255, 0, 100);width: 20px;}");
        battBar2->setStyleSheet("QWidget {background-color: rgba(0, 255, 0, 100);width: 20px;}");
        battBar3->setStyleSheet("QWidget {background-color: rgba(0, 255, 0, 100);width: 20px;}");
    }
    else if (level > 40 && level <= 70){
        // change battery display to semi full and yellow
        battBar1->setStyleSheet("QWidget {background-color: rgba(201, 155, 28, 100);width: 20px;}");
        battBar2->setStyleSheet("QWidget {background-color: rgba(201, 155, 28, 100);width: 20px;}");
        battBar3->setStyleSheet("QWidget {background-color: rgba(201, 155, 28, 0);width: 20px;}");
    }
    else{
        // change battery display to low and red
        battBar1->setStyleSheet("QWidget {background-color: rgba(255, 0, 0, 100);width: 20px;}");
        battBar2->setStyleSheet("QWidget {background-color: rgba(201, 155, 28, 0);width: 20px;}");
        battBar3->setStyleSheet("QWidget {background-color: rgba(255, 0, 0, 0);width: 20px;}");
    }
}

void MainWindow::delay(int time)
{
    QTime dieTime= QTime::currentTime().addSecs(time);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


