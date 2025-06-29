// Copyright (c) 2017-2024 Manuel Schneider

#include "plugin.h"
#include <QDBusConnection>
// #include <QDBusConnectionInterface>
// #include <QDBusMetaType>
#include <QDBusServiceWatcher>
using namespace Qt::StringLiterals;
using namespace std;

struct Plugin::Private
{
    QDBusServiceWatcher service_watcher{
        u"org.mpris.MediaPlayer2*"_s,
        QDBusConnection::sessionBus(),
        QDBusServiceWatcher::WatchForOwnerChange
    };
};


Plugin::Plugin() : d(make_unique<Private>())
{
    if (!QDBusConnection::sessionBus().isConnected())
        throw runtime_error("Failed to connect to session bus.");

    connect(&d->service_watcher, &QDBusServiceWatcher::serviceOwnerChanged, this,
            [this](const QString &service, const QString &, const QString &new_owner){
        if (new_owner.isEmpty())
            players_.erase(service);
        else if (auto it = players_.find(service);
                 it == players_.end())
            players_.emplace(service, make_unique<Player>(service));
        else
            it->second = make_unique<Player>(service);
    });

    // Each media player must request a unique bus name which begins with org.mpris.MediaPlayer2
    if (auto reply = QDBusConnection::sessionBus().interface()->registeredServiceNames();
        !reply.isValid())
        throw runtime_error(reply.error().message().toStdString());
    else
        for (const auto &service : reply.value())
            if (service.startsWith(u"org.mpris.MediaPlayer2."_s))
                players_.emplace(service, make_unique<Player>(service));
}

Plugin::~Plugin() = default;

