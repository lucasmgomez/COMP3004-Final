#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    //
    //include "defs.h"
    QTimer* t;
    QElapsedTimer* et;
    int deviceStatus = OFF; //using defs.h
    void initTimer();
    void initConnections();
    void drainBattery();
    void temp();
    //

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
	
};


#endif // MAINWINDOW_H
