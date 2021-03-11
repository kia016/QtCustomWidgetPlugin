#include "kmessageplugin.h"

KMessagePlugin::KMessagePlugin(QObject *parent)
    : QObject(parent)
{
}

void KMessagePlugin::initialize(QDesignerFormEditorInterface */*core*/)
{
    if (initialized)
        return;
    initialized = true;
}
bool KMessagePlugin::isInitialized() const
{
    return initialized;
}

QWidget *KMessagePlugin::createWidget(QWidget *parent)
{
    return new KMessage(parent);
}

QString KMessagePlugin::name() const
{
    return QStringLiteral("KMessage");
}

QString KMessagePlugin::group() const
{
    return QStringLiteral("KUiTools Widgets");
}

QIcon KMessagePlugin::icon() const
{
    return QIcon();
}

QString KMessagePlugin::toolTip() const
{
    return QString();
}

QString KMessagePlugin::whatsThis() const
{
    return QString();
}

bool KMessagePlugin::isContainer() const
{
    return false;
}

QString KMessagePlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"KMessage\" name=\"kmessage\">\n"
           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>800</width>\n"
           "    <height>200</height>\n"
           "   </rect>\n"
           "  </property>\n"
           "  <property name=\"toolTip\" >\n"
           "   <string>Messagebox</string>\n"
           "  </property>\n"
           "  <property name=\"whatsThis\" >\n"
           "   <string>The messagebox widget displays the message.</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}

QString KMessagePlugin::includeFile() const
{
    return QStringLiteral("KUiTools/kmessage.h");
}
