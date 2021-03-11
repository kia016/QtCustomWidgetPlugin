#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBuffer>
#include <QImageReader>
#include <QDebug>

#include <Motor/motorcontrol.h>
#include <Motor/motorsettingdialog.h>
#include <global_define.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Init();

public slots:
    void ReceiveFromMotor(QString msg);
    void Log(QString msg, QString type = "");

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    MotorControl *motor_;
};
#endif // MAINWINDOW_H
