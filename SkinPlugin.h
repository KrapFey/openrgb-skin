#pragma once
#include <QObject>
#include <QFileSystemWatcher>
#include "OpenRGBPluginInterface.h"

// Loads <OpenRGB config dir>/skin.qss as the app stylesheet (live-reloads on save) and darkens window title bars.
class SkinPlugin : public QObject, public OpenRGBPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID OpenRGBPluginInterface_IID FILE "SkinPlugin.json")
    Q_INTERFACES(OpenRGBPluginInterface)

public:
    OpenRGBPluginInfo   GetPluginInfo() override;
    unsigned int        GetPluginAPIVersion() override { return OPENRGB_PLUGIN_API_VERSION; }
    void                Load(OpenRGBPluginAPIInterface* api) override;
    QWidget*            GetWidget() override;
    QMenu*              GetTrayMenu() override { return nullptr; }
    void                Unload() override;

    void                OnProfileAboutToLoad() override {}
    void                OnProfileLoad(nlohmann::json) override {}
    nlohmann::json      OnProfileSave() override { return nlohmann::json(); }
    unsigned char*      OnSDKCommand(unsigned int, unsigned char*, unsigned int*) override { return nullptr; }
    void                ProfileManagerUpdated(unsigned int) override {}
    void                ResourceManagerUpdated(unsigned int) override {}
    void                SettingsManagerUpdated(unsigned int) override {}

protected:
    bool                eventFilter(QObject* obj, QEvent* event) override;

private:
    void                Apply();
    QString             path;
    QFileSystemWatcher  watcher;
    QWidget*            widget = nullptr;
};
