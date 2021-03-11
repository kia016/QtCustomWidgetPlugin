#include "motorsettingdialog.h"
#include "ui_motorsettingdialog.h"

MotorSettingDialog::MotorSettingDialog(MotorControl *motor, SqliteClass *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MotorSettingDialog),
    motor_(motor)/*,
    db_(db)*/
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    Init();
    QApplication::processEvents();
    setFixedSize(size());
}

MotorSettingDialog::~MotorSettingDialog()
{
    delete ui;
}

void MotorSettingDialog::ReceiveFromMotor(QString msg)
{
    if (msg == "stop") {
//        SetText(QString("Cost :  %1").arg(t_.elapsed()));
        SetText(QString("Cost :  %1 %2").arg(QString::number(t_.restart()), msg));
//        motor_->Ardu_Clear();
    } else {
        count_++;
        SetText(QString("Cost :  %1 %2 %3").arg(QString::number(t_.restart()), msg, QString::number(count_)));
    }
}

void MotorSettingDialog::TimerTimeOut()
{
    timer_.stop();
    motor_->Ardu_Clear();
    ui->pushButton_home->setEnabled(true);
    ui->pushButton_left->setEnabled(true);
    ui->pushButton_clear->setEnabled(true);
    ui->pushButton_close->setEnabled(true);
    ui->pushButton_right->setEnabled(true);
}

void MotorSettingDialog::Init()
{
    t_.start();
    count_ = 0;
    ui->doubleSpinBox_speed->setValue(GetSpeed());
    ui->doubleSpinBox_interval->setValue(GetInterval());
    connect(&timer_, &QTimer::timeout, this, &MotorSettingDialog::TimerTimeOut);
}

double MotorSettingDialog::GetSpeed()
{
    return motor_->GetSpeed();
}

void MotorSettingDialog::SetSpeed(double value)
{
    motor_->SetSpeed(value);
}

double MotorSettingDialog::GetInterval()
{
    return motor_->GetInterval();
}

void MotorSettingDialog::SetInterval(double value)
{
    motor_->SetInterval(value);
}

void MotorSettingDialog::SetText(QString text)
{
    ui->textEdit->append(text);
    ui->textEdit->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->maximum());
}

void MotorSettingDialog::on_doubleSpinBox_speed_valueChanged(double arg1)
{
    SetSpeed(arg1);
    ui->horizontalSlider_speed->setValue(arg1 * 100);
//    db_->UpdateData("MOTOR_TABLE",
//                    {QString("SPEED = '%1'").arg(QString::number(arg1))});
}

void MotorSettingDialog::on_horizontalSlider_speed_valueChanged(int value)
{
    SetSpeed(value / 100.0);
    ui->doubleSpinBox_speed->setValue(value / 100.0);
//    db_->UpdateData("MOTOR_TABLE",
//                    {QString("SPEED = '%1'").arg(QString::number(value/100.0))});
}

void MotorSettingDialog::on_doubleSpinBox_interval_valueChanged(double arg1)
{
    SetInterval(arg1);
//    db_->UpdateData("MOTOR_TABLE",
//                    {QString("INTERVAL = '%1'").arg(QString::number(arg1))});
}

void MotorSettingDialog::on_pushButton_home_clicked()
{
    t_.start();
    count_ = 0;
    motor_->GoHmoe();
    timer_.start(2000);
    ui->pushButton_home->setEnabled(false);
    ui->pushButton_left->setEnabled(false);
    ui->pushButton_clear->setEnabled(false);
    ui->pushButton_close->setEnabled(false);
    ui->pushButton_right->setEnabled(false);
//    motor_->Ardu_TouchTrigger(0, 360/GetInterval());
//    motor_->motor_.MoveAbsolutelyDistance(GetSpeed(), 0);
}

void MotorSettingDialog::on_pushButton_left_clicked()
{
    t_.start();
    count_ = 0;
//    motor_->Move(false);
    motor_->GoAround(false);
    timer_.start(2000);
    ui->pushButton_home->setEnabled(false);
    ui->pushButton_left->setEnabled(false);
    ui->pushButton_clear->setEnabled(false);
    ui->pushButton_close->setEnabled(false);
    ui->pushButton_right->setEnabled(false);
//    motor_->motor_.MoveRelativelyDistance(GetSpeed(), -GetInterval() / 360);
}

void MotorSettingDialog::on_pushButton_right_clicked()
{
    t_.start();
    count_ = 0;
//    motor_->Move();
    motor_->GoAround();
    timer_.start(2000);
    ui->pushButton_home->setEnabled(false);
    ui->pushButton_left->setEnabled(false);
    ui->pushButton_clear->setEnabled(false);
    ui->pushButton_close->setEnabled(false);
    ui->pushButton_right->setEnabled(false);
//    motor_->motor_.MoveRelativelyDistance(GetSpeed(), GetInterval() / 360);
}

void MotorSettingDialog::on_pushButton_close_clicked()
{
//    motor_->GoHmoe();
//    motor_->motor_.MoveAbsolutelyDistance(GetSpeed(), 0);
    accept();
}

void MotorSettingDialog::on_pushButton_clear_clicked()
{
    count_ = 0;
    ui->textEdit->clear();
    motor_->Ardu_Clear();
}
