// Copyright (c) 2017-2025 Manuel Schneider

// https://github.com/khangthk/SketchyBar/blob/5b7d0e07cf41ae8413393b1bb8f5c184a6615695/src/media.m

#include "mediaremote.h"
#include "plugin.h"
#include <AppKit/AppKit.h>
#include <Foundation/Foundation.h>
#include <QCoreApplication>
#include <QUrl>
#include <albert/logging.h>
#include <objc/runtime.h>
using namespace albert;
using namespace std;
#if  ! __has_feature(objc_arc)
#error This file must be compiled with ARC.
#endif

struct Plugin::Private
{
};

Plugin::Plugin() : d(make_unique<Private>())
{
    MRMediaRemoteRegisterForNowPlayingNotifications(dispatch_get_main_queue());

    // Initialize is_playing_
    MRMediaRemoteGetNowPlayingApplicationIsPlaying(dispatch_get_main_queue(),
                                                   ^(Boolean isPlaying) { is_playing_ = isPlaying; });

    // Watch is_playing_
    [[NSNotificationCenter defaultCenter]
        addObserverForName:(__bridge NSString*)kMRMediaRemoteNowPlayingApplicationIsPlayingDidChangeNotification
                    object:nil
                     queue:[NSOperationQueue mainQueue]
                usingBlock:^(NSNotification *note) {

                    for (::id key in note.userInfo)
                        NSLog(@"%@: %@", key, note.userInfo[key]);

                    bool v = [note.userInfo[(__bridge NSString*)kMRMediaRemoteNowPlayingApplicationIsPlayingUserInfoKey]
                        boolValue];

                    if (is_playing_ != v)
                    {
                        is_playing_ = v;
                        emit isPlayingChanged(v);
                    }
                }];

    // Initialize player_name_

    static const auto playerFromPID = [this](int pid) {
        QString player_name, player_path;
        if (pid)
        {
            if (NSRunningApplication *app =
                [NSRunningApplication runningApplicationWithProcessIdentifier:pid];
                !app)
                WARN << "Failed getting app for PID";
            else
            {
                player_name = QString::fromNSString(app.localizedName);
                const auto qurl = QUrl::fromEncoded(QByteArray(app.bundleURL.absoluteString.UTF8String));
                player_path = qurl.toLocalFile();
                CRIT << player_path;
            }
        }

        if (player_ != player_name)
        {
            player_          = player_name;
            player_icon_url_ = {QStringLiteral("qfip:") + player_path};
            DEBG << "Player changed:" << player_;
            emit playerChanged(player_);
        }
    };

    MRMediaRemoteGetNowPlayingApplicationPID(dispatch_get_main_queue(),
                                             ^(int pid) { playerFromPID(pid); });

    // Watch player_name_
    [[NSNotificationCenter defaultCenter]
        addObserverForName:(__bridge NSString*)kMRMediaRemoteNowPlayingApplicationDidChangeNotification
                    object:nil
                     queue:[NSOperationQueue mainQueue]
                usingBlock:^(NSNotification *note) {

                    for (::id key in note.userInfo)
                        NSLog(@"%@: %@", key, note.userInfo[key]);

                    int pid = [note.userInfo[(__bridge NSString*)kMRMediaRemoteNowPlayingApplicationPIDUserInfoKey]
                        intValue];

                    playerFromPID(pid);
                }];


    // Initialize info…

    const auto getNowPlayingBlock = ^(CFDictionaryRef information) {
        if (information)
        {
            const auto dict = (__bridge NSDictionary *)information;

            for (::id key in dict)
                NSLog(@"%@: %@", key, dict[key]);

            NSString *title = dict[(__bridge NSString *)kMRMediaRemoteNowPlayingInfoTitle];
            NSString *artist = dict[(__bridge NSString *)kMRMediaRemoteNowPlayingInfoArtist];
            // NSString *album = dict[(__bridge NSString *)kMRMediaRemoteNowPlayingInfoAlbum];
            now_playing_info_ = QStringLiteral("%1 · %2")
                                    .arg(QString::fromNSString(title),
                                         QString::fromNSString(artist));
                                         // QString::fromNSString(album));
            DEBG << "Now playing info:" << now_playing_info_;
        }
    };

    MRMediaRemoteGetNowPlayingInfo(dispatch_get_main_queue(), getNowPlayingBlock);


    // Watch info
    [[NSNotificationCenter defaultCenter]
        addObserverForName:(__bridge NSString*)kMRMediaRemoteNowPlayingInfoDidChangeNotification
                    object:nil
                     queue:[NSOperationQueue mainQueue]
                usingBlock:^(NSNotification *) {
                    // Suboptimal but the less private API touched the better.
                    MRMediaRemoteGetNowPlayingInfo(dispatch_get_main_queue(), getNowPlayingBlock);
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

bool Plugin::canPlay() { return !player_.isNull(); }

bool Plugin::canPause() { return !player_.isNull(); }

bool Plugin::canGoNext() { return !player_.isNull(); }

bool Plugin::canGoPrevious() { return !player_.isNull(); }
