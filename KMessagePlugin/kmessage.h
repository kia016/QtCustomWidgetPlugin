#ifndef KMESSAGE_H
#define KMESSAGE_H

#include <QWidget>
#include <QtUiPlugin/QDesignerExportWidget>

namespace Ui {
class KMessage;
}

class QDESIGNER_WIDGET_EXPORT KMessage : public QWidget
{
    Q_OBJECT

public:
    explicit KMessage(QWidget *parent = nullptr);
    ~KMessage();

private:
    Ui::KMessage *ui;
};

#endif // KMESSAGE_H
