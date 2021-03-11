#include "smartmotor.h"

#include <QSerialPort>
#include <QDebug>
#include <QCoreApplication>

SmartMotor::~SmartMotor()
{

}

SmartMotor::SmartMotor(unsigned int motor_index, double mm_to_pulse, double speed_to_mm_ps_, unsigned int millisec, std::string name)
    :motor_index_(motor_index), mm_to_pulse_(mm_to_pulse), speed_to_mm_ps_(speed_to_mm_ps_), is_moving_(false), timeout_flag_(false),
    wait_for_sync_(false), wait_for_home_sync_(false), wait_for_return_(false), start_moving_(false), is_connected_(false), timeout_millisec_(millisec)
    , command_(""), name_(name)
{
}

bool SmartMotor::MoveRelativelyDistance(double speed, double relative_distance, bool sync)
{
    if(wait_for_sync_ && start_moving_)
    {
        emit SentToMotor(std::to_string(motor_index_) + "BT;");
        qDebug()<<"Not In Position Yet!";
        return false;
    }

    relative_distance *= mm_to_pulse_;
    speed *= speed_to_mm_ps_;
    emit SentToMotor(std::to_string(motor_index_) + "MPR " + std::to_string(static_cast<int>(speed)) + "," + std::to_string(static_cast<int>(relative_distance)) + ";");
    if(sync)
    {
        wait_for_sync_ = true;
        while (wait_for_sync_)
        {
            if(!wait_for_return_)
                IsMotorMoving();
            QCoreApplication::processEvents();
        }
        wait_for_sync_ = false;
        qDebug()<<"Arrive Destination";
        return true;
    }
    return true;
}

bool SmartMotor::MoveAbsolutelyDistance(double speed, double absolute_distance, bool sync)
{
    if(wait_for_sync_ && start_moving_)
    {
        emit SentToMotor(std::to_string(motor_index_) + "BT;");
        qDebug()<<"Not In Position Yet!";
        return false;
    }

    absolute_distance *= mm_to_pulse_;
    speed *= speed_to_mm_ps_;
    emit SentToMotor(std::to_string(motor_index_) + "MPA " + std::to_string(static_cast<int>(speed)) + "," + std::to_string(static_cast<int>(absolute_distance)) + ";");
    if(sync)
    {
        wait_for_sync_ = true;
        while (wait_for_sync_)
        {
            if(!wait_for_return_)
                IsMotorMoving();
            QCoreApplication::processEvents();
        }
        wait_for_sync_ = false;
        qDebug()<<"Arrive Destination";
        return true;
    }
    return true;
}


bool SmartMotor::SetMovingLimit(double min, double max)
{
    min *= mm_to_pulse_;
    max *= mm_to_pulse_;
//    qDebug()<<motor_index_ << "LIMIT " << min << "," << max << "; SENT";
    emit SentToMotor(std::to_string(motor_index_) + "LIMIT " + std::to_string(static_cast<int>(min)) + "," + std::to_string(static_cast<int>(max)) + ";");
    return true;
}


bool SmartMotor::MoveConstantSpeed(double speed)
{
    if(wait_for_sync_)
    {
        emit SentToMotor(std::to_string(motor_index_) + "BT;");
        qDebug()<<"Not In Position Yet!";
        return false;
    }

    speed *= speed_to_mm_ps_;
    emit SentToMotor(std::to_string(motor_index_) + "MV " + std::to_string(static_cast<int>(speed)) + ";");
    return true;
}

bool SmartMotor::SetHome(SmartMotor::Direction dir)
{
    if(wait_for_sync_)
    {
        emit SentToMotor(std::to_string(motor_index_) + "BT;");
        qDebug()<<"Not In Position Yet!";
        return false;
    }

    if(dir == SmartMotor::Direction::Positive)
        command_ = std::to_string(motor_index_) + "H+;";
    else if(dir == SmartMotor::Direction::Negative)
        command_ = std::to_string(motor_index_) + "H-;";
    emit SentToMotor(command_);

    wait_for_sync_ = true;
    wait_for_home_sync_ = true;
    while (wait_for_home_sync_)
    {
        if(!wait_for_return_)
            IsMotorMoving();
        QCoreApplication::processEvents();
    }
    wait_for_sync_ = false;
    wait_for_home_sync_ = false;

    return true;
}

bool SmartMotor::SetEncoderDivide(unsigned int encode_divide)
{
    emit SentToMotor(std::to_string(motor_index_) + "ENCODE " + std::to_string(encode_divide) + ";");
    return true;
}

bool SmartMotor::SetReportPosition(bool set_status)
{
    if(set_status)
        command_ = std::to_string(motor_index_) + "PT ON;";
    else if(!set_status)
        command_ = std::to_string(motor_index_) + "PT OFF;";
//    qDebug()<<QString::fromStdString(command_);
    emit SentToMotor(command_);
    return true;
}


bool SmartMotor::IsMotorMoving()
{
    if(wait_for_return_)
    {
//        SentToMotor(std::to_string(motor_index_) + "BT;");
        qDebug()<<"Waitting for return!";
        return false;
    }

    emit SentToMotor(std::to_string(motor_index_) + "BT;");

    timeout_flag_ = false;
    wait_for_return_ = true;
    timer_.start(static_cast<int>(timeout_millisec_));

    while(wait_for_return_)
    {
        QCoreApplication::processEvents();
    }

    if(timeout_flag_)
    {
        qDebug()<<"Is Moving Time out!";
        //exception
    }
    return is_moving_;
}

bool SmartMotor::Init()
{
    connect(&timer_, SIGNAL(timeout()), this, SLOT(TimeoutReceive()),Qt::UniqueConnection);
//    connect(&timer_, SIGNAL(timeout()), &loop_, SLOT(quit()));
//    connect(this, SIGNAL(IsMovingResult()), &timer_, SLOT(stop()));
//    connect(this, SIGNAL(IsMovingResult()), &loop_, SLOT(quit()));
//    connect(this, SIGNAL(InPosition()), &loop_, SLOT(quit()));
    return true;
}

bool SmartMotor::IsConnected()
{
    return is_connected_;
}

void SmartMotor::DisConnect()
{
    is_connected_ = false;
}

void SmartMotor::ReceiveFromMotor(QByteArray message)
{
    QByteArray m;
    for(int i=0 ; i<message.size() ; i++)
    {
        if(message.at(i) == static_cast<char>(motor_index_ + '0')) //motor index
        {
            i++; //remove motor index
            for(; i<message.size() ; i++)
            {
                if(message.at(i) == ';')
                {
                    if(m == "OK")
                    {
                        is_connected_ = true;
                        qDebug()<<"slot "<<message<<" is connect "<<IsConnected();
                    }
                    else if(m == "Moving")
                    {
                        is_moving_ = true;
                        wait_for_return_ = false;
//                        wait_for_sync_ = false;
                        timer_.stop();
//                        emit IsMovingResult();
                    }
                    else if(m == "Still")
                    {
                        is_moving_ = false;
                        wait_for_return_ = false;
                        wait_for_sync_ = false;
                        timer_.stop();
//                        emit IsMovingResult();
                    }
                    else if(m == "start moving")
                    {
                        start_moving_ = true;
                        is_moving_ = true;
                    }
                    else if(m == "stop moving")
                    {
                        start_moving_ = false;
                        wait_for_sync_ = false;
                        is_moving_ = false;
                        emit MotorCallBack("stop");
                    }
                    else if (m == "HOME- OK")
                    {
                        emit HomeDone();
                        wait_for_home_sync_ = false;
                        qDebug()<< "motor " << motor_index_ << " HOME- OK ";
                    }
                    else if (m == "HOME+ OK")
                    {
                        emit HomeDone();
                        wait_for_home_sync_ = false;
                        qDebug()<< "motor " << motor_index_ << " HOME+ OK ";
                    }
                    else if (m == "AT EDGE")
                    {
                        emit AtEdge();
                        qDebug()<< "motor " << motor_index_ << " at edge ";
                    }
                    m.clear();
                    break;
                }
                else if(message.at(i) == '=')
                {
                    if(m == "PA")
                    {
                        m.clear();
                        for(int j=1 ; j+i < message.size() ; j++)
                        {
                            if(message.at(i+j) != ';')
                            {
                                m.append(message.at(i+j));
                            }
                            else
                            {
                                bool ok;
                                qDebug()<< "motor " << motor_index_ << " PA = " << m.toInt(&ok,10);
                            }
                        }
                    }
                    break;
                }
                else
                {
                    //receive message
                    m.append(message.at(i));
                }
            }
        }
//        else //not this motor's return message
//        {
//            for(; i<message.size() ; i++)
//            {
//                if(message.at(i) == ';')
//                    break;
//            }
//        }
    }
    message.clear();
}

void SmartMotor::TimeoutReceive()
{
    wait_for_sync_ = false;
    wait_for_return_ = false;
    timeout_flag_ = true;
}

bool SmartMotor::SetTimeoutMillisec(unsigned int millisec)
{
    timeout_millisec_ = millisec;
    return true;
}

bool SmartMotor::SetAccelerate(double acc)
{
    if(wait_for_sync_)
    {
        emit SentToMotor(std::to_string(motor_index_) + "BT;");
        qDebug()<<"Not In Position Yet!";
        return false;
    }


    acc *= speed_to_mm_ps_;
    emit SentToMotor(std::to_string(motor_index_) + "AT " + std::to_string(static_cast<int>(acc)) + ";");
    return true;
}

bool SmartMotor::SetDecelerate(double dec)
{
    if(wait_for_sync_)
    {
        emit SentToMotor(std::to_string(motor_index_) + "BT;");
        qDebug()<<"Not In Position Yet!";
        return false;
    }

    dec *= speed_to_mm_ps_;
    emit SentToMotor(std::to_string(motor_index_) + "DT " + std::to_string(static_cast<int>(dec)) + ";");
    return true;
}

bool SmartMotor::SetADT(double adt)
{
    if(wait_for_sync_)
    {
        emit SentToMotor(std::to_string(motor_index_) + "BT;");
        qDebug()<<"Not In Position Yet!";
        return false;
    }

    adt *= speed_to_mm_ps_;
    emit SentToMotor(std::to_string(motor_index_) + "ADT " + std::to_string(static_cast<int>(adt)) + ";");
    return true;
}

bool SmartMotor::MotorFreeMode()
{
    if(wait_for_sync_)
    {
        emit SentToMotor(std::to_string(motor_index_) + "BT;");
        qDebug()<<"Not In Position Yet!";
        return false;
    }
    emit SentToMotor(std::to_string(motor_index_) + "SVOFF;");
    return true;
}
