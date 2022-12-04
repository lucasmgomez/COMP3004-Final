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
    //left and right ear check boxes
    connect(ui->leftBox, SIGNAL(stateChanged(int)), this, SLOT(updateEarUI()));
    connect(ui->rightBox, SIGNAL(stateChanged(int)), this, SLOT(updateEarUI()));

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
            //qDebug() << "Please specify user #";
            cout << "Please specify user #" << endl;
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
            //qDebug() << "Please specify user # and replay session #";
            cout << "Please specify user # and replay session #" << endl;
            return;
        }
        oasis->setUser(ui->currentUser->toPlainText().toInt());
        oasis->replay(ui->replaySes->toPlainText().toInt());
    }
}

void MainWindow::powerPress(){
    //qDebug() << "power pressed (counting milliseconds)";
    cout << "power pressed (counting milliseconds)" << endl;
    et->start();
    //case for if user never releases power button?
}

void MainWindow::powerRelease(){

    float elapsed = et->elapsed() / 1000.00;

    //Turn on
    if (oasis->getPower() == OFF){
        // Turn on lights to default
        setDefaultLEDs(true);
        //setConnectLEDs();

        //qDebug() << "+turnOn: held button:" << elapsed << " sec.";
        cout << "+turnOn: held button:" << elapsed << " sec." << endl;
        oasis->turnOn();
        return;
    }
    //Force turn off
    if (oasis->getPower() == ON && elapsed > 1){
        setDefaultLEDs(false);
        //qDebug() << "+force turnOff, held button:" << elapsed << "sec.";
        cout << "+force turnOff, held button:" << elapsed << "sec." << endl;
        oasis->turnOff();
        return;
    }
    //Change duration
    else if (oasis->getPower() == ON && elapsed < 1 && oasis->getRunning()==false){
        //qDebug() << "      ++'tapped'for:" << elapsed << "sec. [changing session type]:";
        cout << "      ++'tapped'for:" << elapsed << "sec. [changing session type]:" << endl;
        {
            oasis->nextDuration();
            Session* sess = oasis->getCurrSession();
            updateDurUI(sess->getDuration());
        }
        return;
    }
    else if (oasis->getPower() == ON && elapsed < 1 && oasis->getRunning()==true){
        //qDebug() << "      ++'tapped'for:" << elapsed << "sec. [initiating softOff]:";
        cout << "      ++'tapped'for:" << elapsed << "sec. [initiating softOff]:" << endl;
        {
            interruptSession=true;
        }
        return;
    }
}

void MainWindow::upPress(){
    if(!ui->leftBox->checkState() && !ui->rightBox->checkState() && oasis->getRunning()==true && oasis->getPower()==ON){
        cout<<"     Unattached ears: cannot change intensity"<<endl;
        return;
    }
    if (oasis->getRunning()==false && oasis->getPower()==ON)
    {
        oasis->nextType();
        Session* sess = oasis->getCurrSession();
        updateTypeUI(sess->getType());
    }
    else if (oasis->getRunning()==true && oasis->getPower()==ON)
    {
        oasis->nextIntensity();
        Session* sess = oasis->getCurrSession();
        updateIntUI(sess->getIntensity());

    }
}
void MainWindow::downPress(){
    if(!ui->leftBox->checkState() && !ui->rightBox->checkState() && oasis->getRunning()==true && oasis->getPower()==ON){
        cout<<"     Unattached ears: cannot change intensity"<<endl;
        return;
    }
    if (oasis->getRunning()==false && oasis->getPower()==ON)
    {
        oasis->prevType();
        Session* sess = oasis->getCurrSession();
        updateTypeUI(sess->getType());
    }
    else if (oasis->getRunning()==true && oasis->getPower()==ON)
    {
        oasis->prevIntensity();
        Session* sess = oasis->getCurrSession();
        updateIntUI(sess->getIntensity());
    }


}
void MainWindow::confirmPress(){
    if (oasis->getRunning()==false && oasis->getPower()==ON){
        oasis->setConnection(ui->connectBox->currentIndex());
        setConnectLEDs(ui->connectBox->currentIndex());
        delay(2000);
        updateIntUI(1);
        oasis->runSession(); //runSession checks if connection > 0 before running
        sessionRunTime = 0;
    }

}

void MainWindow::toggleLeftEar(){
    oasis->toggleLeftEar();
}
void MainWindow::toggleRightEar(){
    oasis->toggleRightEar();
}

void MainWindow::update(){
    /*
        Add left and right ear connection stuff pg.6
    */
    if (oasis->getBatteryState() == LOW)
    {
        //BLINKING HERE
    }
    else if (oasis->getBatteryState() == CRITICAL)
    {
        //BLINKING HERE
    }


    if (!ui->leftBox->checkState() && !ui->rightBox->checkState() && oasis->getRunning()==true){
        handleDisconnect();
    }

    //reset counters if power off, adjust gui
    if(oasis->getPower()==OFF){
        if (shutdownCounter > 0){shutdownCounter=0;}
        if (sessionRunTime > 0){sessionRunTime=0;}
        setDefaultLEDs(false);
        return;
    }
    //idle shutdown counter
    if(oasis->getPower()==ON && oasis->getRunning()==false){
        if (shutdownCounter < 120){
         shutdownCounter++;
         //qDebug() << "              " << shutdownCounter<<"sec./"<<"2min.[idle shutdown]";
         cout<< "       " << shutdownCounter<<"sec./"<<"2min.[idle shutdown]" << endl;
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
        //qDebug() << "runTime:"<< sessionRunTime << "sec./" << oasis->getDurationInMin() << "min.";
        cout << "runTime:"<< sessionRunTime << "sec./" << oasis->getDurationInMin() << "min." << endl;

        if (interruptSession == true) //interrupting a session and initiate softOff
        {
            sessionRunTime++;
            if (oasis->getIntensity() > 1){
                softOff();
            }
            else{
                interruptSession = false;
                oasis->endSession();
            }
        }
        else //determining length of session before initiating softOff when it almost finishes
        {
            if (oasis->getDurationInMin()*60 - sessionRunTime > 0){
                if(oasis->getDurationInMin()*60 - sessionRunTime < 10){
                    softOff();
                }
                sessionRunTime++; //session runtime counter
            }
            else{
                oasis->endSession();
            }
        }

    }
    //drain battery
    oasis->useBattery();
}

//decreasing the intensity for softOff
void MainWindow::softOff()
{
    //experimental code
    if(oasis->getIntensity()>1)
    {
        oasis->prevIntensity();
        updateIntUI(oasis->getIntensity());
    }
}
void MainWindow::updateDurUI(int dur){
    if (dur == TWENTY){
        ui->time20LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(0, 255, 0, 100);width: 20px;}");
        ui->time45LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        ui->time45LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
    }
    else if (dur == FORTYFIVE){
        ui->time20LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        ui->time45LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(0, 255, 0, 100);width: 20px;}");
        ui->timeUserLED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
    }
    else{
        // user set duration
        return;
    }

}

void MainWindow::updateTypeUI(int type){
    if (type == MET){
        ui->freq1LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(0, 255, 0, 100);width: 20px;}");
        ui->freq2LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        ui->freq3LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        ui->freq4LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        updateWaveUI(false);
    }
    else if (type == SUBDELTA){
        ui->freq1LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        ui->freq2LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(0, 255, 0, 100);width: 20px;}");
        ui->freq3LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        ui->freq4LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        updateWaveUI(true);
    }
    else if (type == DELTA){
        ui->freq1LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        ui->freq2LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        ui->freq3LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(0, 255, 0, 100);width: 20px;}");
        ui->freq4LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        updateWaveUI(false);
    }
    else if (type == THETA){
        ui->freq1LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        ui->freq2LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        ui->freq3LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        ui->freq4LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(0, 255, 0, 100);width: 20px;}");
        updateWaveUI(false);
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
        ui->powerLED->setStyleSheet("QWidget {border: 2px solid grey; border-radius: 5px; background-color: rgba(0, 255,0, 100); width: 20px; }");
        ui->time20LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(0, 255, 0, 100);width: 20px;}");
        ui->freq1LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(0, 255, 0, 100);width: 20px;}");

        for (int i = 1; i <= 8; ++i){
            auto lcdInt = findChild<QLCDNumber*>("lcdInt"+QString::number(i));
            lcdInt->setStyleSheet("QLCDNumber {background-color: rgba(0, 0, 0, 70);width: 20px;}");
        }

        ui->shortLED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(0, 255, 0, 100);width: 20px;}");
        ui->fiftyLED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
    }
    if (!on){
        ui->powerLED->setStyleSheet("QWidget {border: 2px solid grey; border-radius: 5px; background-color: rgba(255, 255,255, 100); width: 20px; }");
        ui->time20LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        ui->time45LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        ui->freq1LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        ui->freq2LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        ui->freq3LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        ui->freq4LED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");

        ui->leftLED->setStyleSheet("QLabel {color: rgba(255, 255,255, 100);}");
        ui->rightLED->setStyleSheet("QLabel {color: rgba(255, 255,255, 100);}");

        ui->shortLED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        ui->fiftyLED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");

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
            if (i > 6){
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
            if (i <= 3){
                lcdInt->setStyleSheet("QLCDNumber {background-color: rgba(0, 255, 0, 70);width: 20px;}");
            }
            else{
                lcdInt->setStyleSheet("QLCDNumber {background-color: rgba(0, 0, 0, 70);width: 20px;}");
            }
        }
    }
}

void MainWindow::setBatteryUI(){
    int level = ui->batteryEdit->text().toInt();

    //force the battery power in oasis to change
    oasis->setBattery(level);

    if (level > 70){
        // change battery display to full and green
        ui->battBar1->setStyleSheet("QWidget {background-color: rgba(0, 255, 0, 100);width: 20px;}");
        ui->battBar2->setStyleSheet("QWidget {background-color: rgba(0, 255, 0, 100);width: 20px;}");
        ui->battBar3->setStyleSheet("QWidget {background-color: rgba(0, 255, 0, 100);width: 20px;}");
    }
    else if (level > CRITICALTHRESHOLD && level <= LOWTHRESHOLD){
        // change battery display to semi full and yellow
        ui->battBar3->setStyleSheet("QWidget {background-color: rgba(201, 155, 28, 0);width: 20px;}");
        ui->battBar1->setStyleSheet("QWidget {background-color: rgba(201, 155, 28, 100);width: 20px;}");
        ui->battBar2->setStyleSheet("QWidget {background-color: rgba(201, 155, 28, 100);width: 20px;}");
        delay(500);
        ui->battBar1->setStyleSheet("QWidget {background-color: rgba(201, 155, 28, 0);width: 20px;}");
        ui->battBar2->setStyleSheet("QWidget {background-color: rgba(201, 155, 28, 0);width: 20px;}");
        delay(500);
        ui->battBar1->setStyleSheet("QWidget {background-color: rgba(201, 155, 28, 100);width: 20px;}");
        ui->battBar2->setStyleSheet("QWidget {background-color: rgba(201, 155, 28, 100);width: 20px;}");
        delay(500);
        ui->battBar1->setStyleSheet("QWidget {background-color: rgba(201, 155, 28, 0);width: 20px;}");
        ui->battBar2->setStyleSheet("QWidget {background-color: rgba(201, 155, 28, 0);width: 20px;}");
        delay(500);
        ui->battBar1->setStyleSheet("QWidget {background-color: rgba(201, 155, 28, 100);width: 20px;}");
        ui->battBar2->setStyleSheet("QWidget {background-color: rgba(201, 155, 28, 100);width: 20px;}");
    }
    else if (level <= CRITICALTHRESHOLD){
        // change battery display to low and red
        ui->battBar3->setStyleSheet("QWidget {background-color: rgba(255, 0, 0, 0);width: 20px;}");
        ui->battBar2->setStyleSheet("QWidget {background-color: rgba(201, 155, 28, 0);width: 20px;}");
        ui->battBar1->setStyleSheet("QWidget {background-color: rgba(255, 0, 0, 70);width: 20px;}");
        delay(500);
        ui->battBar1->setStyleSheet("QWidget {background-color: rgba(255, 0, 0, 0);width: 20px;}");
        delay(500);
        ui->battBar1->setStyleSheet("QWidget {background-color: rgba(255, 0, 0, 70);width: 20px;}");
        delay(500);
        ui->battBar1->setStyleSheet("QWidget {background-color: rgba(255, 0, 0, 0);width: 20px;}");
        delay(500);
        ui->battBar1->setStyleSheet("QWidget {background-color: rgba(255, 0, 0, 70);width: 20px;}");
    }
}

void MainWindow::updateEarUI(){

    if (ui->leftBox->checkState()){
        ui->leftLED->setStyleSheet("QLabel {color: rgba(0, 255,0, 100);}");
    }
    else{
        ui->leftLED->setStyleSheet("QLabel {color: rgba(0, 0 , 0, 100);}");
    }

    if (ui->rightBox->checkState()){
        ui->rightLED->setStyleSheet("QLabel {color: rgba(0, 255,0, 100);}");
    }
    else{
        ui->rightLED->setStyleSheet("QLabel {color: rgba(0, 0 , 0, 100);}");
    }

}

void MainWindow::updateWaveUI(bool subDelt){
    if (subDelt){
        ui->shortLED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
        ui->fiftyLED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(0, 255, 0, 100);width: 20px;}");
    }
    else{
        ui->shortLED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(0, 255, 0, 100);width: 20px;}");
        ui->fiftyLED->setStyleSheet("QWidget {border: 2px solid grey;border-radius: 5px;background-color: rgba(255, 255, 255, 100);width: 20px;}");
    }
}

void MainWindow::handleDisconnect(){
    for (int i = 1; i <= 8; ++i){
        auto lcdInt = findChild<QLCDNumber*>("lcdInt"+QString::number(i));
        lcdInt->setStyleSheet("QLCDNumber {background-color: rgba(0, 0, 0, 70);width: 20px;}");
    }
    ui->lcdInt7->setStyleSheet("QLCDNumber {background-color: rgba(255, 0, 0, 70);width: 20px;}");
    ui->lcdInt8->setStyleSheet("QLCDNumber {background-color: rgba(255, 0, 0, 70);width: 20px;}");
    delay(1000);
    ui->lcdInt7->setStyleSheet("QLCDNumber {background-color: rgba(0, 0, 0, 70);width: 20px;}");
    ui->lcdInt8->setStyleSheet("QLCDNumber {background-color: rgba(0, 0, 0, 70);width: 20px;}");
    delay(1000);
    ui->lcdInt7->setStyleSheet("QLCDNumber {background-color: rgba(255, 0, 0, 70);width: 20px;}");
    ui->lcdInt8->setStyleSheet("QLCDNumber {background-color: rgba(255, 0, 0, 70);width: 20px;}");
    delay(1000);
    ui->lcdInt7->setStyleSheet("QLCDNumber {background-color: rgba(0, 0, 0, 70);width: 20px;}");
    ui->lcdInt8->setStyleSheet("QLCDNumber {background-color: rgba(0, 0, 0, 70);width: 20px;}");
    delay(1000);
    updateIntUI(1);

}

void MainWindow::delay(int time)
{
    QTime dieTime= QTime::currentTime().addMSecs(time);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


