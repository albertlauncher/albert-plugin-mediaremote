// Copyright (c) 2017-2025 Manuel Schneider

#include "mediaremote.h"
#include "plugin.h"
#include <mutex>
using namespace std;
#if  ! __has_feature(objc_arc)
#error This file must be compiled with ARC.
#endif

struct Plugin::Private {};

Plugin::Plugin() : d(make_unique<Private>())
{
    MRMediaRemoteRegisterForNowPlayingNotifications(dispatch_get_main_queue());

    auto updatePlayerBlock = ^(int pid) {
        scoped_lock lock(players_mtx_);
        players_.clear();
        if (pid != 0)
        {
            auto player = make_shared<Player>(pid);
            players_.emplace(player->name(), ::move(player));
        }
    };

    // Initialize player
    MRMediaRemoteGetNowPlayingApplicationPID(dispatch_get_main_queue(), updatePlayerBlock);

    // Watch player
    [[NSNotificationCenter defaultCenter]
        addObserverForName:kMRMediaRemoteNowPlayingApplicationDidChangeNotification
                    object:nil
                     queue:[NSOperationQueue mainQueue]
                usingBlock:^(NSNotification *note) {
                    int pid = [note.userInfo[kMRMediaRemoteNowPlayingApplicationPIDUserInfoKey]
                        intValue];
                    updatePlayerBlock(pid);
                }];
}

Plugin::~Plugin() {}


