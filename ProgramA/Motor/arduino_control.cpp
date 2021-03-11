#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include "arduino_control.h"

ArduinoControl::ArduinoControl(QObject *parent)
    : QObject(parent), is_connected_(false), timeout_flag_(false), waiting_(false), receive_("")
{
}
ArduinoControl::~ArduinoControl()
{
    if (is_connected_)
    {
        Disconnect();
    }
}
bool ArduinoControl::Connect()
{
    if (!waiting_)
    {
        if (is_connected_)
        {
            qDebug() << "Arduino Already connected";
            return true;
        }
        else
        {
            if (!port_.isOpen())
            {
                port_.setPortName("COM2");
                port_.setBaudRate(QSerialPort::Baud9600);
                port_.setDataBits(QSerialPort::Data8);
                port_.setStopBits(QSerialPort::OneStop);
                port_.setParity(QSerialPort::NoParity);
                port_.setFlowControl(QSerialPort::NoFlowControl);
//                port_.setDataTerminalReady(true);
//                port_.setRequestToSend(true);
                connect(&port_, &QSerialPort::readyRead, this, &ArduinoControl::Arrived);
//                connect(&timer_, SIGNAL(timeout()), this, SLOT(TimeoutReceive()));
//                timeout_flag_ = false;
                if (!port_.open(QIODevice::ReadWrite))
                {
                    qDebug() << "Arduino port connect fail!";
                    qDebug() << port_.error();
                    return false;
                }
//                QThread::sleep(2);
                port_.setDataTerminalReady(true);
                port_.setRequestToSend(true);
            }

//            port_.write("RUN ");
//            port_.write(";");

//            waiting_ = true;
//            timer_.start(2000);
//            while (waiting_ && !timeout_flag_)
//            {
//                QCoreApplication::processEvents();
//            }
//            if (timeout_flag_)
//            {
//                qDebug() << "Arduino Run Connect Timeout!";
//                return false;
//            }
            is_connected_ = true;
        }
        qDebug() << "Arduino port connect ok!";
        return true;
    }
    else
    {
        qDebug() << "Connecting Wait For Reply";
        return false;
    }
}
bool ArduinoControl::Disconnect()
{
//    port_.close();
    return true;
}
bool ArduinoControl::IsConnected() { return is_connected_; }
bool ArduinoControl::Clear(uint motor_id)
{
    auto val = port_.write(QString(QString::number(motor_id) + "CLR\n").toLocal8Bit());
//    QThread::msleep(10);
    return val != -1 ? true : false;
}
bool ArduinoControl::SetInterval(uint interval, uint motor_id)
{
    auto val = port_.write(QString(QString::number(motor_id) + "INTERVAL " + QString::number(interval) + " \n").toLocal8Bit());
//    QThread::msleep(10);
    return val != -1 ? true : false;
}

bool ArduinoControl::SetPluseWidth(uint plusewidth, uint motor_id)
{
    auto val = port_.write(QString(QString::number(motor_id) + "PLUSEWIDTH " + QString::number(plusewidth) + " \n").toLocal8Bit());
//    QThread::msleep(10);
    return val != -1 ? true : false;
}

bool ArduinoControl::TouchTrigger(uint motor_id, uint count)
{
    auto val = port_.write(QString(QString::number(motor_id) + "TR %1\n").arg(QString::number(count)).toLocal8Bit());
//    QThread::msleep(10);
    return val != -1 ? true : false;
}
bool ArduinoControl::SetEncoder(uint motor_id)
{
    auto val = port_.write(QString(QString::number(motor_id) + "EN\n").toLocal8Bit());
    QThread::msleep(10);
    return val != -1 ? true : false;
}
bool ArduinoControl::GetTriggerInCount(uint motor_id)
{
    auto val = port_.write(QString(QString::number(motor_id) + "GIN\n").toLocal8Bit());
    QThread::msleep(10);
    return val != -1 ? true : false;
}
bool ArduinoControl::GetTriggerOutCount(uint motor_id)
{
    auto val = port_.write(QString(QString::number(motor_id) + "GOUT\n").toLocal8Bit());
    QThread::msleep(10);
    return val != -1 ? true : false;
}
void ArduinoControl::TimeoutReceive()
{
    timeout_flag_ = true;
}

void ArduinoControl::Arrived()
{
    QByteArray ba = port_.readAll();
    if (ba.size() > 0)
    {
        receive_.append(ba);
//        qDebug() << "Arduino send:" << ba;
        emit Log("Arduino send:" + ba);
        if (receive_.contains("Start:\n"))
        {
            waiting_ = false;
            timer_.stop();
        }
        receive_.clear();
    }
}
