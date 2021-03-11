#ifndef ARDUINO_CONTROL_H
#define ARDUINO_CONTROL_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <QByteArray>
class ArduinoControl : public QObject
{
    Q_OBJECT
public:
    explicit ArduinoControl(QObject *parent = nullptr);
    ~ArduinoControl();
    bool Connect();
    bool Disconnect();
    bool IsConnected();
    bool Clear(uint motor_id);
    bool SetInterval(uint interval, uint motor_id);
    bool SetPluseWidth(uint plusewidth, uint motor_id);
    bool TouchTrigger(uint motor_id, uint count);
    bool SetEncoder(uint motor_id);
    bool GetTriggerInCount(uint motor_id);
    bool GetTriggerOutCount(uint motor_id);

    ArduinoControl(const ArduinoControl &) = delete;
    ArduinoControl &operator=(const ArduinoControl &) = delete;

signals:
    void Log(QString msg);

public slots:
    void TimeoutReceive();
    void Arrived();

private:
    QSerialPort port_;
    bool is_connected_;
    bool timeout_flag_;
    bool waiting_;
    QByteArray receive_;
    QTimer timer_;
};

#endif // ARDUINO_CONTROL_H
