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
    bool interruptSession = false; //when to initiate soft off
    int shutdownCounter = 0; //120secs force turn off; in seconds
    int sessionRunTime = 0; //session run time; in seconds

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
