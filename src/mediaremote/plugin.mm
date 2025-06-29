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

struct Plugin::Private
{
public:

};

Plugin::Plugin() : d(make_unique<Private>())
{
    MRMediaRemoteRegisterForNowPlayingNotifications(dispatch_get_main_queue());

    auto updatePlayerBlock = ^(int pid) {
        players_.clear();
        if (pid != 0)
        {
            auto player = make_unique<Player>(pid);
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


