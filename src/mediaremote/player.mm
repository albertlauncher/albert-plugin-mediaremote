// Copyright (c) 2017-2025 Manuel Schneider

#include "player.h"
#include "mediaremote.h"
#include <AppKit/AppKit.h>
#include <albert/logging.h>
#if  ! __has_feature(objc_arc)
#error This file must be compiled with ARC.
#endif

Player::Player(int pid) //: d(make_unique<Private>())
{
    auto *app = [NSRunningApplication runningApplicationWithProcessIdentifier:pid];

    if (!app) {
        WARN << "Failed to get running application with PID:" << pid;
        return;
    }

    name_ = QString::fromNSString(app.localizedName);

    NSURL *bundleURL = app.bundleURL;
    NSString *bundlePath = bundleURL.path;
    icon_url_ = u"qfip:" + QString::fromNSString(bundlePath);

    // Initialize is_playing_
    MRMediaRemoteGetNowPlayingApplicationIsPlaying(dispatch_get_main_queue(),
                                                   ^(Boolean isPlaying) { is_playing_ = isPlaying; });

    // Watch is_playing_
    notificationObservation_ = [[NSNotificationCenter defaultCenter]
        addObserverForName:kMRMediaRemoteNowPlayingApplicationIsPlayingDidChangeNotification
                    object:nil
                     queue:[NSOperationQueue mainQueue]
                usingBlock:^(NSNotification *note) {
                    bool v = [note.userInfo[kMRMediaRemoteNowPlayingApplicationIsPlayingUserInfoKey]
                        boolValue];
                    if (is_playing_ != v) {
                        is_playing_ = v;
                        emit isPlayingChanged(v);
                    }
                }];
}

Player::~Player()
{
    [[NSNotificationCenter defaultCenter] removeObserver:notificationObservation_];
}

QString Player::name() const
{
    return name_;
}

QString Player::iconUrl() const
{
    return icon_url_;
}

bool Player::isPlaying() const
{
    return is_playing_;
}

bool Player::canPlay() const
{
    return true;
}

bool Player::canPause() const
{
    return true;
}

bool Player::canGoNext() const
{
    return true;
}

bool Player::canGoPrevious() const
{
    return true;
}

void Player::play()
{
    MRMediaRemoteSendCommand(kMRPlay, nil);
}

void Player::pause()
{
    MRMediaRemoteSendCommand(kMRPause, nil);
}

void Player::next()
{
    MRMediaRemoteSendCommand(kMRNextTrack, nil);
}

void Player::previous()
{
    MRMediaRemoteSendCommand(kMRPreviousTrack, nil);
}
