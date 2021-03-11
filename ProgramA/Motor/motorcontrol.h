#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QCoreApplication>
#include <QSqlQuery>

#include <Motor/smartmotorporthub.h>
#include <Motor/smartmotor.h>
#include <Sqlite/sqliteclass.h>
#include <Motor/arduino_control.h>

class MotorControl : public QObject
{
    Q_OBJECT
public:
    explicit MotorControl(SqliteClass *db = nullptr, QObject *parent = nullptr);
    bool Connect();
    bool Disconnect();
    bool IsConnected();

    void SetSpeed(double speed);
    void SetInterval(double interval);
    double GetSpeed();
    double GetInterval();

    void SetADT(double value);

    bool Move(bool positive = true);
    bool GoHmoe();
    bool GoAround(bool positive = true);
    bool Stop();

    bool Ardu_Connect();
    bool Ardu_Clear(int id = 0);
    bool Ardu_SetInterval(int interval, int id = 0);
    bool Ardu_SetPluseWidth(int plusewidth, int id = 0);
    bool Ardu_TouchTrigger(int id = 0, int count = 8);

private:
    bool Init();

public slots:
    void Ardu_Log(QString msg);

signals:
    void Log(QString msg, QString type = "System");

public:
    SmartMotor motor_;

private:
    SmartMotorPortHub port_;
    SqliteClass *db_;
    double speed_;
    double interval_;
    ArduinoControl arduino_;
};

#endif // MOTORCONTROL_H
