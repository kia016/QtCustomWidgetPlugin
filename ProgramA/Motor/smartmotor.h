#ifndef SMARTMOTOR_H
#define SMARTMOTOR_H

//#include "smartmotor_global.h"
#include <QObject>
#include <QTimer>
#include <QEventLoop>

class QSerialPort;
class SmartMotor : public QObject
{
    Q_OBJECT
public:
    enum struct Direction{Positive, Negative};
    ~SmartMotor();
    SmartMotor(unsigned int motor_index = 1, double mm_to_pulse = 800, double speed_to_mm_ps_ = 830.410542321, unsigned int millisec = 2000):
    SmartMotor(motor_index, mm_to_pulse, speed_to_mm_ps_, millisec, std::to_string(motor_index)){}
    SmartMotor(unsigned int motor_index, double mm_to_pulse, double speed_to_mm_ps_, unsigned int millisec, std::string name);
    bool MoveRelativelyDistance(double speed, double relative_distance, bool sync = false);
    bool MoveAbsolutelyDistance(double speed, double absolute_distance, bool sync = false);
    bool MoveConstantSpeed(double speed);
    bool SetHome(Direction dir);
    bool SetEncoderDivide(unsigned int encode_divide);
    bool SetReportPosition(bool set_status);
    bool SetMovingLimit(double min, double max);
    bool SetMovingDirection(Direction dir);
    bool SetTimeoutMillisec(unsigned int millisec);
    bool SetAccelerate(double);
    bool SetDecelerate(double);
    bool SetADT(double);
    bool MotorFreeMode();
    bool IsMotorMoving();
    bool Init();
    bool IsConnected();
    void DisConnect();

signals:
    void InPosition();
    void HomeDone();
    void AtEdge();
    void IsMovingResult();
    void SentToMotor(std::string);
    void MotorCallBack(QString msg);

public slots:
    void ReceiveFromMotor(QByteArray message);
    void TimeoutReceive();

private:
    const unsigned int motor_index_;
    const double mm_to_pulse_;
    const double speed_to_mm_ps_;
    bool is_moving_;
    bool timeout_flag_;
    bool wait_for_sync_;
    bool wait_for_home_sync_;
    bool wait_for_return_;
    bool start_moving_;
    bool is_connected_;
    unsigned int timeout_millisec_;

    QTimer timer_;
//    QEventLoop loop_;
    std::string command_;
    std::string name_;
};

#endif // SMARTMOTOR_H
