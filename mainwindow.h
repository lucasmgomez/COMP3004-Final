#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QRegularExpression>
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
    void updateDurUI(int);
    void updateTypeUI(int);
    void updateIntUI(int);
<<<<<<< Updated upstream
=======
    void updateEarUI();
    void updateWaveUI(bool);
    void updateBatteryUI(float level);
>>>>>>> Stashed changes
    void setDefaultLEDs(bool);
    void setConnectLEDs(int);
    void setBatteryUI();

    void delay(int);
    void savePress();
    void replayPress();
    void updateList();
};

#endif // MAINWINDOW_H
