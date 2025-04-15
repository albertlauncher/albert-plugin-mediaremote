// Copyright (c) 2017-2024 Manuel Schneider

// https://github.com/khangthk/SketchyBar/blob/5b7d0e07cf41ae8413393b1bb8f5c184a6615695/src/media.m

#include "mediaremote.h"
#include "plugin.h"
#include <Foundation/Foundation.h>
#include <albert/logging.h>
#include <objc/runtime.h>
#include <AppKit/AppKit.h>
using namespace albert;
using namespace std;
#if  ! __has_feature(objc_arc)
#error This file must be compiled with ARC.
#endif

struct Plugin::Private {};

static QString playerNamefromPid(int pid)
{
    if (pid)
        if (NSRunningApplication *app =
            [NSRunningApplication runningApplicationWithProcessIdentifier:pid];
            app)
            return QString::fromNSString(app.localizedName);
    return {};
}

Plugin::Plugin() : d(make_unique<Private>())
{
    MRMediaRemoteRegisterForNowPlayingNotifications(dispatch_get_main_queue());

    // Initialize is_playing_
    MRMediaRemoteGetNowPlayingApplicationIsPlaying(
        dispatch_get_main_queue(), ^(Boolean isPlaying) { is_playing_ = isPlaying; });

    // Watch is_playing_
    [[NSNotificationCenter defaultCenter]
        addObserverForName:kMRMediaRemoteNowPlayingApplicationIsPlayingDidChangeNotification
                    object:nil
                     queue:[NSOperationQueue mainQueue]
                usingBlock:^(NSNotification *note) {
                    bool v = [note.userInfo[kMRMediaRemoteNowPlayingApplicationIsPlayingUserInfoKey]
                        boolValue];
                    if (is_playing_ != v)
                    {
                        is_playing_ = v;
                        DEBG << "is_playing changed:" << v;
                        emit isPlayingChanged(v);
                    }
                }];

    // Initialize player_name_
    MRMediaRemoteGetNowPlayingApplicationPID(dispatch_get_main_queue(),
                                             ^(int pid) { player_ = playerNamefromPid(pid); });

    // Watch player_name_
    [[NSNotificationCenter defaultCenter]
        addObserverForName:kMRMediaRemoteNowPlayingApplicationDidChangeNotification
                    object:nil
                     queue:[NSOperationQueue mainQueue]
                usingBlock:^(NSNotification *note) {
                    int pid = [note.userInfo[kMRMediaRemoteNowPlayingApplicationPIDUserInfoKey]
                        intValue];
                    auto name = playerNamefromPid(pid);
                    if (player_ != name)
                    {
                        player_ = name;
                        DEBG << "player_name changed:" << name;
                        emit playerChanged(name);
                    }
                }];
}

Plugin::~Plugin() {}

QString Plugin::player() { return player_; }

bool Plugin::isPlaying() { return is_playing_; }

// void Plugin::playPause() { MRMediaRemoteSendCommand(kMRTogglePlayPause, nil); }

void Plugin::play() { MRMediaRemoteSendCommand(kMRPlay, nil); }

void Plugin::pause() { MRMediaRemoteSendCommand(kMRPause, nil); }

void Plugin::next() { MRMediaRemoteSendCommand(kMRNextTrack, nil); }

void Plugin::previous() { MRMediaRemoteSendCommand(kMRPreviousTrack, nil); }

bool Plugin::canPlay() { return true; }

bool Plugin::canPause() { return true; }

bool Plugin::canGoNext() { return true; }

bool Plugin::canGoPrevious() { return true; }















// class Player
// {
//     const QString dbus_service_name;
//     OrgMprisMediaPlayer2Interface player;
//     MediaPlayer2PlayerInterface control;
//     QString id;

// public:

//     Player(const QString &service_name,const QDBusConnection &session_bus):
//         dbus_service_name(service_name),
//         player(dbus_service_name, dbus_object_path, session_bus),
//         control(dbus_service_name, dbus_object_path, session_bus),
//         id(player.identity())
//     {
//         player.setTimeout(dbus_timeout);
//         control.setTimeout(dbus_timeout);

//         // TODO: it makes no sense to proceed here without dynamic items
//         // notes: qdbusxml2cpp created files are still low level. calls are sync there are no
//         // signals emitted for changed properties. this has to be done manually. also since qobject
//         // does not support multiple inheritance the way to go is probably another level on top
//         // facading all the dbus interfaces below.
//         // Helpful: QDBUS_DEBUG=1
//     }

//     inline shared_ptr<Item>
//     makeCtlItem(const QString &cmd, const QStringList &icon_urls, function<void()> &&action)
//     { return StandardItem::make(cmd, cmd, id, icon_urls, {{ cmd, cmd, ::move(action)}}); }

//     void addItems(vector<RankItem>& items, const QString &query)
//     {
//         if (!control.canControl())
//             return;

//         static const QString tr_raise = Plugin::tr("Raise");
//         static const QString tr_quit = Plugin::tr("Quit");
//         static const QString tr_play = Plugin::tr("Play");
//         static const QString tr_pause = Plugin::tr("Pause");
//         static const QString tr_stop = Plugin::tr("Stop");
//         static const QString tr_next = Plugin::tr("Next");
//         static const QString tr_prev = Plugin::tr("Previous");

//         static const QStringList iu_play = {"xdg:media-playback-start"};
//         static const QStringList iu_pause = {"xdg:media-playback-pause"};
//         static const QStringList iu_stop = {"xdg:media-playback-stop"};
//         static const QStringList iu_next = {"xdg:media-skip-forward"};
//         static const QStringList iu_prev = {"xdg:media-skip-backward"};
//         static const QStringList iu_player = {"xdg:multimedia-player"};

//         static const auto act_play = [this]{ control.Play(); };
//         static const auto act_pause = [this]{ control.Pause(); };
//         static const auto act_stop = [this]{ control.Stop(); };
//         static const auto act_next = [this]{ control.Next(); };
//         static const auto act_prev = [this]{ control.Previous(); };

//         enum PlaybackStatus { Playing, Paused, Stopped };
//         static const QString playback_status_strings[] = {
//             Plugin::tr("Playing"),
//             Plugin::tr("Paused"),
//             Plugin::tr("Stopped")
//         };

//         PlaybackStatus playback_status = Stopped;
//         if (control.playbackStatus() == QStringLiteral("Playing"))
//             playback_status = Playing;
//         else if (control.playbackStatus() == QStringLiteral("Paused"))
//             playback_status = Paused;
//         else if (control.playbackStatus() == QStringLiteral("Stopped"))
//             playback_status = Stopped;
//         else
//             WARN << "Invalid playback status received:" << control.playbackStatus();


//         Matcher matcher(query);

//         // Player item

//         if (auto m = matcher.match(id); m)
//         {
//             vector<Action> actions;

//             if (player.canRaise())
//                 actions.emplace_back(tr_raise, tr_raise, [this]{ player.Raise(); });

//             if (playback_status == Playing)
//             {
//                 if (control.canPause())
//                     actions.emplace_back(tr_pause, tr_pause, act_pause);

//                 actions.emplace_back(tr_stop, tr_stop, act_stop);
//             }
//             else
//                 if (control.canPlay())
//                     actions.emplace_back(tr_play, tr_play, act_play);

//             if (control.canGoNext())
//                 actions.emplace_back(tr_next, tr_next, act_next);

//             if (control.canGoPrevious())
//                 actions.emplace_back(tr_prev, tr_prev, act_prev);

//             if (player.canQuit())
//                 actions.emplace_back(tr_quit, tr_quit, [this]{ player.Quit(); });

//             QStringList icon_urls;
//             if (auto de = player.desktopEntry(); !de.isEmpty())
//                 icon_urls << QString("xdg:%1").arg(de);
//             icon_urls << iu_player;

//             // https://www.freedesktop.org/wiki/Specifications/mpris-spec/metadata/

//             auto md = control.metadata();
//             CRIT << md;
//             QStringList sl;

//             sl << playback_status_strings[playback_status];

//             // TODO: Dynamic items

//             if (auto it1 = md.find("xesam:title"), it2 = md.find("xesam:artist");
//                 it1 != md.end() && it2 != md.end() && it1->canConvert<QString>() && it2->canConvert<QString>())
//             {
//                 sl << it1->toString() << it2->toString();
//             }
//             else if (it1 = md.find("xesam:url"); it1 != md.end() && it1->canConvert<QString>())
//             {
//                 QFileInfo fi(QUrl(it1->toString()).toLocalFile());
//                 sl << fi.fileName() << fi.dir().dirName();
//             }

//             items.emplace_back(StandardItem::make(id, id, sl.join(" – "), icon_urls, actions), m);
//         }


//         // Control items

//         if (auto m = matcher.match(tr_next); m && control.canGoNext())
//             items.emplace_back(makeCtlItem(tr_next, iu_next, act_next), m);

//         if (auto m = matcher.match(tr_prev); m && control.canGoPrevious())
//             items.emplace_back(makeCtlItem(tr_prev, iu_prev, act_prev), m);

//         if (playback_status == Playing)
//         {
//             if (auto m = matcher.match(tr_stop); m)
//                 items.emplace_back(makeCtlItem(tr_stop, iu_stop, act_stop), m);

//             if (auto m = matcher.match(tr_pause); m && control.canPause())
//                 items.emplace_back(makeCtlItem(tr_pause, iu_pause, act_pause), m);
//         }
//         else
//         {
//             if (auto m = matcher.match(tr_play); m && control.canPlay())
//                 items.emplace_back(makeCtlItem(tr_play, iu_play, act_stop), m);
//         }

//     }
// };



// void fetchNowPlayingInfo() {
//     dispatch_queue_t queue = dispatch_get_main_queue();

//     MRMediaRemoteGetNowPlayingInfo(queue, ^(CFDictionaryRef info) {
//         if (!info) {
//             NSLog(@"No media info available");
//             return;
//         }

//         NSDictionary *dict = (__bridge NSDictionary *)info;
//         NSString *title  = dict[(__bridge NSString *)kMRMediaRemoteNowPlayingInfoTitle];
//         NSString *artist = dict[(__bridge NSString *)kMRMediaRemoteNowPlayingInfoArtist];
//         NSString *album  = dict[(__bridge NSString *)kMRMediaRemoteNowPlayingInfoAlbum];

//         CRIT << QString::fromNSString(title);
//         CRIT << QString::fromNSString(artist);
//         CRIT << QString::fromNSString(album);
//     });
// }
