#include "SkinPlugin.h"
#include <QImage>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QWidget>
#include <QEvent>
#include <windows.h>
#include <dwmapi.h>

// Windows draws the title bar itself (blue accent when active), so stylesheets can't reach it.
// ponytail: caption colours hard-coded to the Midnight palette; read them from the skin if you add more skins.
static void DarkCaption(QWidget* w)
{
    HWND     hwnd = reinterpret_cast<HWND>(w->winId());
    BOOL     dark = TRUE;
    COLORREF bg   = RGB(0x14, 0x16, 0x1B);
    COLORREF fg   = RGB(0xE6, 0xE8, 0xEC);
    DwmSetWindowAttribute(hwnd, 20 /* DWMWA_USE_IMMERSIVE_DARK_MODE */, &dark, sizeof(dark));
    DwmSetWindowAttribute(hwnd, 35 /* DWMWA_CAPTION_COLOR */,           &bg,   sizeof(bg));
    DwmSetWindowAttribute(hwnd, 36 /* DWMWA_TEXT_COLOR */,              &fg,   sizeof(fg));
}

bool SkinPlugin::eventFilter(QObject* obj, QEvent* event)
{
    if(event->type() == QEvent::Show && obj->isWidgetType())
    {
        QWidget* w = static_cast<QWidget*>(obj);
        if(w->windowType() == Qt::Window || w->windowType() == Qt::Dialog) DarkCaption(w);
    }
    return false;
}

OpenRGBPluginInfo SkinPlugin::GetPluginInfo()
{
    OpenRGBPluginInfo info;
    info.Name            = "Skin";
    info.Description     = "Applies skin.qss from the OpenRGB config folder (live reload on save) and darkens the title bar";
    info.Version         = PLUGIN_VERSION;
    info.Commit          = PLUGIN_COMMIT;
    info.URL             = PLUGIN_URL;
    info.Location        = 0xFFFFFFFF;  // invalid on purpose: no tab, this plugin has no UI
    info.Label           = "Skin";
    info.ProtocolVersion = 0;
    info.Icon            = QImage(":/openrgb-skin/icon.png");
    return info;
}

void SkinPlugin::Load(OpenRGBPluginAPIInterface* api)
{

    path = QString::fromStdWString((api->GetConfigurationDirectory() / "skin.qss").wstring());
    // Lets the stylesheet reference images beside it as url(skin:name.png).
    QDir::setSearchPaths("skin", { QFileInfo(path).absolutePath() });
    // Editors often replace the file on save, which drops the watch, so re-add it every time.
    connect(&watcher, &QFileSystemWatcher::fileChanged, this, [this] { Apply(); });
    Apply();

    qApp->installEventFilter(this);  // dark title bar on every window/dialog as it opens
    for(QWidget* w : qApp->topLevelWidgets())
    {
        if(w->isVisible() && (w->windowType() == Qt::Window || w->windowType() == Qt::Dialog)) DarkCaption(w);
    }
}

void SkinPlugin::Apply()
{
    QFile f(path);
    if(f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qApp->setStyleSheet(QString::fromUtf8(f.readAll()));
    }
    if(!watcher.files().contains(path) && QFile::exists(path))
    {
        watcher.addPath(path);
    }
}

QWidget* SkinPlugin::GetWidget()
{
    if(!widget) widget = new QWidget();  // never shown (no tab), returned only so the host never gets null
    return widget;
}

void SkinPlugin::Unload()
{
    qApp->removeEventFilter(this);
    qApp->setStyleSheet(QString());
}
