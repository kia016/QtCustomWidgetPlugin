#include "smartmotorporthub.h"
#include <QDebug>
#include <QEventLoop>
#include <QTimer>
#include <QThread>
#include <QSerialPortInfo>
#include <QCoreApplication>
//#include <SerialStream.h>

SmartMotorPortHub::SmartMotorPortHub(unsigned int motor_number)
    :is_connected_(false), timeout_flag_(false), waiting_(false),motor_number_(motor_number), receive_(""), motorOK_(static_cast<unsigned int>(motor_number))
{
    for(unsigned int i=1 ; i <= motor_number ; i++)
        motorOK_[i-1] = false;
}

SmartMotorPortHub::~SmartMotorPortHub()
{
    if(is_connected_)
        Disconnect();
}

bool SmartMotorPortHub::Connect()
{
    if(!waiting_)
    {
        if(is_connected_)
        {
            qDebug()<<"Already connected";
            return false;
        }
        else
        {
            if(!port_.isOpen())
            {
                //            foreach (const QSerialPortInfo &qspinfo, QSerialPortInfo::availablePorts())
                //            {
                //                qDebug()<<qspinfo.portName();
                //            }

//                port_.setPortName("ttyS0");
                port_.setPortName("COM4");
                port_.setBaudRate(QSerialPort::Baud9600);
                port_.setDataBits(QSerialPort::Data8);
                port_.setStopBits(QSerialPort::OneStop);
                port_.setParity(QSerialPort::NoParity);
                port_.setFlowControl(QSerialPort::NoFlowControl);
                connect(&port_, &QSerialPort::readyRead, this, &SmartMotorPortHub::Arrived);
                if(!port_.open(QIODevice::ReadWrite))
                {
                    qDebug()<<"port connect fail!";
                    qDebug()<<port_.error();
                    return false;
                }
                port_.setDataTerminalReady(true);
                port_.setRequestToSend(true);

                connect(&timer_, &QTimer::timeout, this, &SmartMotorPortHub::TimeoutReceive);
//                connect(&timer_, SIGNAL(timeout()), &loop_, SLOT(quit()));
//                connect(this, SIGNAL(RunOK()), &loop_, SLOT(quit()));
//                connect(this, SIGNAL(RunOK()), &timer_, SLOT(stop()));
//                connect(this, SIGNAL(EndOK()), &loop_, SLOT(quit()));
//                connect(this, SIGNAL(EndOK()), &timer_, SLOT(stop()));
            }

            //        QByteArray data4;
            //        data4[0] = 128;
            //        port_.write(data4+"EIGN(2) ");
            //        port_.write(data4+"EIGN(3) ");
            //        port_.write(data4+"ZS ");
            //        port_.write(data4+"MP ");
            //        port_.write(data4+"ADT=60000 ");
            //        port_.write(data4+"VT=65000 ");
            //        port_.write(data4+"PT=0 ");
            //        port_.write(data4+"G ");

            //        QThread::msleep(50);

            port_.write("RUN ");
            port_.write(";");
            timeout_flag_ = false;
            waiting_ = true;
            timer_.start(2000);
            while(waiting_)
            {
                QCoreApplication::processEvents();
            }


//            loop_.exec();
            if(timeout_flag_)
            {
                qDebug()<<"Run Connect Timeout!";
                return false;
            }
            is_connected_ = true;
        }
        return true;
    }
    else {
        qDebug()<<"Connecting Wait For Reply";
        return false;
    }
}

bool SmartMotorPortHub::Disconnect()
{
    if(!waiting_)
    {
        if(is_connected_)
        {
            //        command_ = std::to_string( static_cast<int>(motor_index_) ) + "b;";
            //        const char * c = command_.c_str();
            port_.write("1b;");

            timeout_flag_ = false;
            waiting_ = true;
            timer_.start(2000);
//            loop_.exec();
            while(waiting_)
            {
                QCoreApplication::processEvents();
            }

            if(timeout_flag_)
            {
                qDebug()<<"Motor Disconnect Time out!";
                return false;
            }
            is_connected_ = false;
        }
        else
        {
            qDebug()<<"Motor not connected";
            return false;
        }


        QByteArray data4;
//        data4[0] = 128;
        data4.append(0x80);
        //serial_->write(data4);
        port_.write(data4+"EIGN(2) ");
        port_.write(data4+"EIGN(3) ");
        port_.write(data4+"ZS ");
        port_.write(data4+"MP ");
        port_.write(data4+"ADT=60000 ");
        port_.write(data4+"VT=65000 ");
        port_.write(data4+"PT=0 ");
        port_.write(data4+"G ");

        return true;
    }
    else {
        qDebug()<<"Connecting Wait For Reply";
        return false;
    }
}

bool SmartMotorPortHub::IsConnected()
{
    return is_connected_;
}

unsigned int SmartMotorPortHub::GetMotorNumber()
{
    return motor_number_;
}

void SmartMotorPortHub::TimeoutReceive()
{
    timeout_flag_ = true;
    waiting_ = false;
}

void SmartMotorPortHub::Arrived()
{
    QByteArray ba = port_.readAll();

    //    qDebug() << ba;

    if(ba.size() > 0)
    {
        //        if(ba.contains("RUN"))
        //        {
        //            emit RunOK();
        //        }
        //        else if(ba.contains("END"))
        //        {
        //            emit EndOK();
        //        }
        //        else
        {
            receive_.append(ba);

            if(receive_.contains("RUN"))
            {
                waiting_ = false;
                timer_.stop();
//                emit RunOK();
            }
            else if(receive_.contains("END"))
            {
                waiting_ = false;
                timer_.stop();
//                emit EndOK();
            }

            if(receive_.back() == ';')
            {
                emit Receive(receive_);
                qDebug() << receive_;
                receive_.clear();
            }
        }
    }
}

void SmartMotorPortHub::Sent(std::string command)
{
    if(is_connected_)
    {
        const char * c = command.c_str();
        port_.write(c);
        command.clear();
    }
    else
        qDebug()<<"Not Connected, Sent Error!";
}
