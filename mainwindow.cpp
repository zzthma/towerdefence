#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <dialog.h>
#include <QPainter>

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


void MainWindow::on_pushButton_clicked()
{
    class Dialog *a=new Dialog;
    this->close();
    a->show();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPixmap cachePix(":/1.jpg");
    QPainter cachePainter(&cachePix);
    QPainter painter(this);
    painter.drawPixmap(0, 0, cachePix);
}
