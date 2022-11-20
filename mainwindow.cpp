#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConnections()
{
    //et->start();
    //et->elapsed()
}

void MainWindow::powerPress(){

}

void MainWindow::powerRelease(){

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
