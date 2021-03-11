#ifndef SMARTMOTORPORTHUB_H
#define SMARTMOTORPORTHUB_H

//#include "smartmotorporthub_global.h"
#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <QEventLoop>

class SmartMotorPortHub : public QObject
{
    Q_OBJECT

public:
//    SmartMotorPortHub(QObject *parent = nullptr):SmartMotorPortHub(1, parent){}
    SmartMotorPortHub(unsigned int motor_number);
    ~SmartMotorPortHub();
    bool Connect();
    bool Disconnect();
    bool IsConnected();
    unsigned int GetMotorNumber();

public slots:
    void TimeoutReceive();
    void Arrived();
    void Sent(std::string command);
signals:
    void RunOK();
    void EndOK();
    void Receive(QByteArray);
private:
    QSerialPort port_;
    bool is_connected_;
    bool timeout_flag_;
    bool waiting_;
    unsigned int motor_number_;
    QByteArray receive_;
    QTimer timer_;
    std::vector<bool> motorOK_;
//    QEventLoop loop_;
};

#endif // SMARTMOTORPORTHUB_H
