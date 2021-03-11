#ifndef MOTORSETTINGDIALOG_H
#define MOTORSETTINGDIALOG_H

#include <QDialog>
#include <QElapsedTimer>
#include <QDebug>
#include <QScrollBar>
#include <QSqlQuery>
#include <QTimer>

#include <Motor/motorcontrol.h>
#include <Sqlite/sqliteclass.h>

namespace Ui {
class MotorSettingDialog;
}

class MotorSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MotorSettingDialog(MotorControl *motor, SqliteClass *db = nullptr, QWidget *parent = nullptr);
    ~MotorSettingDialog();

public slots:
    void ReceiveFromMotor(QString msg);
    void TimerTimeOut();

private slots:
    void on_doubleSpinBox_speed_valueChanged(double arg1);

    void on_horizontalSlider_speed_valueChanged(int value);

    void on_doubleSpinBox_interval_valueChanged(double arg1);

    void on_pushButton_home_clicked();

    void on_pushButton_left_clicked();

    void on_pushButton_right_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_clear_clicked();

private:
    void Init();

    double GetSpeed();
    void SetSpeed(double value);
    double GetInterval();
    void SetInterval(double value);

    void SetText(QString text);

private:
    Ui::MotorSettingDialog *ui;
    MotorControl *motor_;
    SqliteClass *db_;
    QElapsedTimer t_;
    int count_;
    QTimer timer_;
};

#endif // MOTORSETTINGDIALOG_H
