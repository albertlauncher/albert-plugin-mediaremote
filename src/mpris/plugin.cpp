// Copyright (c) 2017-2024 Manuel Schneider

#include "mpris.h"
#include "plugin.h"
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusMetaType>
#include <QDBusServiceWatcher>
#include <albert/logging.h>
#include <albert/matcher.h>
#include <albert/standarditem.h>
using namespace Qt::StringLiterals;
using namespace albert;
using namespace std;
using MediaPlayer2Interface = OrgMprisMediaPlayer2Interface;
using MediaPlayer2PlayerInterface = OrgMprisMediaPlayer2PlayerInterface;

static const auto dbus_timeout = 100;
static const auto dbus_object_path = QStringLiteral("/org/mpris/MediaPlayer2");


class Player : public albert::plugin::mediaremote::IPlayer
{
public:
    const QString dbus_service_name;
    OrgMprisMediaPlayer2Interface player;
    MediaPlayer2PlayerInterface control;
    QString name_;
    QString icon_url_;

    Player(const QString &service_name):
        dbus_service_name(service_name),
        player(dbus_service_name, dbus_object_path, QDBusConnection::sessionBus()),
        control(dbus_service_name, dbus_object_path, QDBusConnection::sessionBus()),
        name_(player.identity())
    {

        if (auto de = player.desktopEntry();
            !de.isEmpty())
            icon_url_ = u"xdg:%1"_s.arg(de);
        else
            icon_url_ = u"xdg:media-player"_s;


        player.setTimeout(dbus_timeout);
        control.setTimeout(dbus_timeout);
    }

    QString name() const override { return name_; }
    QString iconUrl() const override  { return icon_url_; }

    bool isPlaying() const override {
        //     enum PlaybackStatus { Playing, Paused, Stopped };
        //     static const QString playback_status_strings[] = {
        //         Plugin::tr("Playing"),
        //         Plugin::tr("Paused"),
        //         Plugin::tr("Stopped")
        //     };
        //     PlaybackStatus playback_status = Stopped;
        //     if (control.playbackStatus() == QStringLiteral("Playing"))
        //         playback_status = Playing;
        //     else if (control.playbackStatus() == QStringLiteral("Paused"))
        //         playback_status = Paused;
        //     else if (control.playbackStatus() == QStringLiteral("Stopped"))
        //         playback_status = Stopped;
        //     else
        //         WARN << "Invalid playback status received:" << control.playbackStatus();
        //     return playback_status == Playing;
        return control.playbackStatus() == QStringLiteral("Playing");
    }
    // QString isPlayingTitle() const override { return isPlayingTitle_; }
    // QString isPlayingInfo() const override { return isPlayingInfo_; }

    bool canPlay() const override { return control.canPlay(); }
    bool canPause() const override { return control.canPause(); }
    bool canGoNext() const override { return control.canGoNext(); }
    bool canGoPrevious() const override { return control.canGoPrevious(); }

    void play() override { control.Play(); }
    void pause() override { control.Pause(); }
    void next() override { control.Next(); }
    void previous() override { control.Previous(); }

    //     Matcher matcher(query);

    //     // Player item

    //     if (auto m = matcher.match(id); m)
    //     {
    //         vector<Action> actions;

    //         if (player.canRaise())
    //             actions.emplace_back(tr_raise, tr_raise, [this]{ player.Raise(); });

    //         if (playback_status == Playing)
    //         {
    //             if (control.canPause())
    //                 actions.emplace_back(tr_pause, tr_pause, act_pause);

    //             actions.emplace_back(tr_stop, tr_stop, act_stop);
    //         }
    //         else
    //             if (control.canPlay())
    //                 actions.emplace_back(tr_play, tr_play, act_play);

    //         if (control.canGoNext())
    //             actions.emplace_back(tr_next, tr_next, act_next);

    //         if (control.canGoPrevious())
    //             actions.emplace_back(tr_prev, tr_prev, act_prev);

    //         if (player.canQuit())
    //             actions.emplace_back(tr_quit, tr_quit, [this]{ player.Quit(); });

    //         QStringList icon_urls;

    //         // https://www.freedesktop.org/wiki/Specifications/mpris-spec/metadata/

    //         auto md = control.metadata();
    //         CRIT << md;
    //         QStringList sl;

    //         sl << playback_status_strings[playback_status];

    //         // TODO: Dynamic items

    //         if (auto it1 = md.find("xesam:title"), it2 = md.find("xesam:artist");
    //             it1 != md.end() && it2 != md.end() && it1->canConvert<QString>() && it2->canConvert<QString>())
    //         {
    //             sl << it1->toString() << it2->toString();
    //         }
    //         else if (it1 = md.find("xesam:url"); it1 != md.end() && it1->canConvert<QString>())
    //         {
    //             QFileInfo fi(QUrl(it1->toString()).toLocalFile());
    //             sl << fi.fileName() << fi.dir().dirName();
    //         }

    //         items.emplace_back(StandardItem::make(id, id, sl.join(" – "), icon_urls, actions), m);
    //     }


    //     // Control items

    //     if (auto m = matcher.match(tr_next); m && control.canGoNext())
    //         items.emplace_back(makeCtlItem(tr_next, iu_next, act_next), m);

    //     if (auto m = matcher.match(tr_prev); m && control.canGoPrevious())
    //         items.emplace_back(makeCtlItem(tr_prev, iu_prev, act_prev), m);

    //     if (playback_status == Playing)
    //     {
    //         if (auto m = matcher.match(tr_stop); m)
    //             items.emplace_back(makeCtlItem(tr_stop, iu_stop, act_stop), m);

    //         if (auto m = matcher.match(tr_pause); m && control.canPause())
    //             items.emplace_back(makeCtlItem(tr_pause, iu_pause, act_pause), m);
    //     }
    //     else
    //     {
    //         if (auto m = matcher.match(tr_play); m && control.canPlay())
    //             items.emplace_back(makeCtlItem(tr_play, iu_play, act_stop), m);
    //     }

    // }
};


struct Plugin::Private
{
    QDBusServiceWatcher service_watcher{
        QStringLiteral("org.mpris.MediaPlayer2*"),
        QDBusConnection::sessionBus(),
        QDBusServiceWatcher::WatchForOwnerChange
    };

    void serviceOwnerChanged(const QString &service, const QString &, const QString &new_owner)
    {
    //     if (new_owner.isEmpty())
    //         player.reset();
    //     else
    //         player = make_unique<Player>(service);
    }
};


Plugin::Plugin() : d(make_unique<Private>())
{
    if (!QDBusConnection::sessionBus().isConnected())
        throw runtime_error("Failed to connect to session bus.");

    connect(&d->service_watcher, &QDBusServiceWatcher::serviceOwnerChanged, this,
            [this](const QString &service, const QString &old_owner, const QString &new_owner)
            { d->serviceOwnerChanged(service, old_owner, new_owner); } );

    // Each media player must request a unique bus name which begins with org.mpris.MediaPlayer2
    if (auto reply = QDBusConnection::sessionBus().interface()->registeredServiceNames();
        !reply.isValid())
        throw runtime_error(reply.error().message().toStdString());
    else
        for (const auto &service : reply.value())
            if (service.startsWith(QStringLiteral("org.mpris.MediaPlayer2.")))
            {
                auto player = make_unique<Player>(service);
                players_.emplace(player->name(), ::move(player));
            }
}

Plugin::~Plugin() = default;

