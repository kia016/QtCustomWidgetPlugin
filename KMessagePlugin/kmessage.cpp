#include "kmessage.h"
#include "ui_kmessage.h"

KMessage::KMessage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KMessage)
{
    ui->setupUi(this);
}

KMessage::~KMessage()
{
    delete ui;
}
