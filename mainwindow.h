#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>
#include <QTimer>
#include <QDebug>
#include "defs.h"
#include "oasis.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Oasis* oasis;
    QTimer* t;
    QElapsedTimer* et;
    int deviceStatus = OFF; //using defs.h
    int shutdownCounter = 0; //increase by 1 per second, reset when appropriate
                                //2mins = 120seconds, if we hit 120, the device turns off. if no session is running.
                                //todo:
                                //warn low battery function?
                                //warn lowest battery function? force user to change battery and wont start sessions.
                                //user stuff...
    int sessionRunTime = 0; //counter for session runtime? per second.

    void initTimer();
    void initConnections();
    void drainBattery();


private:
    Ui::MainWindow *ui;


private slots:
    void powerPress();
    void powerRelease();

    void upPress();
    void downPress();
    void confirmPress();

    void toggleLeftEar();
    void toggleRightEar();

    void update();
    void softOff();
};

#endif // MAINWINDOW_H
