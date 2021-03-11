#include "motorcontrol.h"

MotorControl::MotorControl(SqliteClass *db, QObject *parent) :
    QObject(parent),
    motor_(1, 4000*10, 32768*10, 2000, "MOTOR"),
    port_(1)/*,
    db_(db)*/
{
//    QSqlQuery query = db_->Select("MOTOR_TABLE");
//    if (query.next()) {
//        speed_ = query.value("SPEED").toDouble();
//        interval_ = query.value("INTERVAL").toDouble();
//    }
    speed_ = 95;
    interval_ = 30;
}

bool MotorControl::Connect()
{
    Init();
    if (port_.Connect()) {
        for (int i=0; i<10000; i++)
        {
            QCoreApplication::processEvents();
            if (IsConnected()) {
                return true;
            }
        }
    }
    port_.Sent("RUN ");
    port_.Sent(";");
    for (int i=0; i<10000; i++)
    {
        QCoreApplication::processEvents();
        if (IsConnected()) {
            return true;
        }
    }
    port_.Disconnect();
    return false;
}

bool MotorControl::Disconnect()
{
    if (port_.Disconnect()) {
//        GoHmoe();
        motor_.DisConnect();
        return true;
    }
    return false;
}

bool MotorControl::IsConnected()
{
    if (!port_.IsConnected()) {
        return false;
    }
    if (!motor_.IsConnected()) {
        return false;
    }
//    if (!arduino_.IsConnected()) {
//        return false;
//    }
    return true;
}

void MotorControl::SetSpeed(double speed)
{
    speed_ = speed;
}

void MotorControl::SetInterval(double interval)
{
    interval_ = interval;
    Ardu_SetInterval(interval_);
}

double MotorControl::GetSpeed()
{
    return speed_;
}

double MotorControl::GetInterval()
{
    return interval_;
}

void MotorControl::SetADT(double value)
{
    motor_.SetADT(value);
}

bool MotorControl::Move(bool positive)
{
    return motor_.MoveRelativelyDistance(speed_/60, (positive ? 1 : -1) * interval_ / 360);
}

bool MotorControl::GoHmoe()
{
    return motor_.MoveAbsolutelyDistance(speed_/60, 0);
}

bool MotorControl::GoAround(bool positive)
{
    return motor_.MoveRelativelyDistance(speed_/60, (positive ? 1.02 : -1.02));
}

bool MotorControl::Stop()
{
    return motor_.MoveConstantSpeed(0);
}

bool MotorControl::Ardu_Connect()
{
    if (arduino_.Connect()) {
        connect(&arduino_, &ArduinoControl::Log, this, &MotorControl::Ardu_Log);
        Ardu_Clear();
        Ardu_SetInterval(interval_);
        Ardu_SetPluseWidth(10);
        return true;
    }
    emit Log(QString(tr("Ardu connect fail.")));
    return false;
}

bool MotorControl::Ardu_Clear(int id)
{
    return arduino_.Clear(id);
}

bool MotorControl::Ardu_SetInterval(int interval, int id)
{
    return arduino_.SetInterval(10*1000*interval/360, id);
}

bool MotorControl::Ardu_SetPluseWidth(int plusewidth, int id)
{
    return arduino_.SetPluseWidth(plusewidth, id);
}

bool MotorControl::Ardu_TouchTrigger(int id, int count)
{
    return arduino_.TouchTrigger(id, count);
}

bool MotorControl::Init()
{
    if (motor_.IsConnected()) {
        qDebug() << "already connected";
        return false;
    }
    connect(&motor_, &SmartMotor::SentToMotor, &port_, &SmartMotorPortHub::Sent, Qt::DirectConnection);
    connect(&port_, &SmartMotorPortHub::Receive, &motor_, &SmartMotor::ReceiveFromMotor, Qt::DirectConnection);
    motor_.Init();
//    QSqlQuery query = db_->Select("MOTOR_TABLE");
//    if (query.next()) {
//        speed_ = query.value("SPEED").toDouble();
//        interval_ = query.value("INTERVAL").toDouble();
//    }
//    if (!Ardu_Connect()) {
//        emit Log(QString(tr("Ardu connect fail.")));
//        return false;
//    }
//    speed_ = 1.0;
//    interval_ = 45.0;
    return true;
}

void MotorControl::Ardu_Log(QString msg)
{
    emit Log(msg);
}
