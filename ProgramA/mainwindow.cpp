#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QApplication::processEvents();
    Init();
}

MainWindow::~MainWindow()
{
    if (motor_->IsConnected()) {
//        motor_->GoHmoe();
        motor_->Disconnect();
    }
    delete ui;
}

void MainWindow::Init()
{
    motor_ = new MotorControl();
    connect(&motor_->motor_, &SmartMotor::MotorCallBack, this, &MainWindow::ReceiveFromMotor);
    connect(motor_, &MotorControl::Log, this, &MainWindow::Log);
    QApplication::processEvents();
    if (motor_->Ardu_Connect()) {
        Log("Arduino connect success.");
//        qDebug() << (QString(tr("Arduino connect success.")));
    } else {
        Log("Arduino connect fail.");
//        qDebug() << (QString(tr("Arduino connect fail.")));
    }
    if (motor_->Connect()) {
        Log("Motor connect success.");
//        qDebug() << (QString(tr("Motor connect success.")));
    } else {
        Log("Motor connect fail.");
//        qDebug() << (QString(tr("Motor connect fail.")));
    }
}

void MainWindow::ReceiveFromMotor(QString msg)
{
    Log(msg);
}

void MainWindow::Log(QString msg, QString type)
{
    ui->textEdit->append(msg);
}

void MainWindow::on_pushButton_clicked()
{
    MotorSettingDialog *motorset = new MotorSettingDialog(motor_);
    motorset->exec();
}

void MainWindow::on_pushButton_2_clicked()
{
    if (motor_->Ardu_Connect()) {
        Log("Arduino connect success.");
//        qDebug() << (QString(tr("Arduino connect success.")));
    } else {
        Log("Arduino connect fail.");
//        qDebug() << (QString(tr("Arduino connect fail.")));
    }
    if (motor_->Connect()) {
        Log("Motor connect success.");
//        qDebug() << (QString(tr("Motor connect success.")));
    } else {
        Log("Motor connect fail.");
//        qDebug() << (QString(tr("Motor connect fail.")));
    }
}
