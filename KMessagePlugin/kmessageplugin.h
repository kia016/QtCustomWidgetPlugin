#ifndef KMESSAGEPLUGIN_H
#define KMESSAGEPLUGIN_H

//#include <QGenericPlugin>
#include <QDesignerCustomWidgetInterface>
#include <QDesignerFormEditorInterface>
#include <kmessage.h>

class KMessagePlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface" FILE "KMessagePlugin.json")
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    explicit KMessagePlugin(QObject *parent = nullptr);

    void initialize(QDesignerFormEditorInterface *core) override;
    bool isInitialized() const override;
    QWidget *createWidget(QWidget *parent) override;
    QString name() const override;
    QString group() const override;
    QIcon icon() const override;
    QString toolTip() const override;
    QString whatsThis() const override;
    bool isContainer() const override;
    QString domXml() const override;
    QString includeFile() const override;

private:
    bool initialized = false;

private:
//    QObject *create(const QString &name, const QString &spec) override;
};

#endif // KMESSAGEPLUGIN_H
