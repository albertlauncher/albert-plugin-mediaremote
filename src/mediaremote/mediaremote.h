#pragma once
#include <objc/runtime.h>
#include <Foundation/Foundation.h>

extern "C" {

extern NSString* kMRMediaRemoteNowPlayingInfoDidChangeNotification;
extern NSString* kMRMediaRemoteNowPlayingPlaybackQueueDidChangeNotification;
extern NSString* kMRMediaRemotePickableRoutesDidChangeNotification;
extern NSString* kMRMediaRemoteNowPlayingApplicationDidChangeNotification;
extern NSString* kMRMediaRemoteNowPlayingApplicationIsPlayingDidChangeNotification;
extern NSString* kMRMediaRemoteRouteStatusDidChangeNotification;

extern NSString* kMRMediaRemoteNowPlayingApplicationPIDUserInfoKey;
extern NSString* kMRMediaRemoteNowPlayingApplicationIsPlayingUserInfoKey;
extern NSString* kMRMediaRemoteNowPlayingInfoAlbum;
extern NSString* kMRMediaRemoteNowPlayingInfoArtist;
extern NSString* kMRMediaRemoteNowPlayingInfoArtworkData;
extern NSString* kMRMediaRemoteNowPlayingInfoArtworkMIMEType;
extern NSString* kMRMediaRemoteNowPlayingInfoChapterNumber;
extern NSString* kMRMediaRemoteNowPlayingInfoComposer;
extern NSString* kMRMediaRemoteNowPlayingInfoDuration;
extern NSString* kMRMediaRemoteNowPlayingInfoElapsedTime;
extern NSString* kMRMediaRemoteNowPlayingInfoGenre;
extern NSString* kMRMediaRemoteNowPlayingInfoIsAdvertisement;
extern NSString* kMRMediaRemoteNowPlayingInfoIsBanned;
extern NSString* kMRMediaRemoteNowPlayingInfoIsInWishList;
extern NSString* kMRMediaRemoteNowPlayingInfoIsLiked;
extern NSString* kMRMediaRemoteNowPlayingInfoIsMusicApp;
extern NSString* kMRMediaRemoteNowPlayingInfoMediaType;
extern NSString* kMRMediaRemoteNowPlayingInfoPlaybackRate;
extern NSString* kMRMediaRemoteNowPlayingInfoProhibitsSkip;
extern NSString* kMRMediaRemoteNowPlayingInfoQueueIndex;
extern NSString* kMRMediaRemoteNowPlayingInfoRadioStationIdentifier;
extern NSString* kMRMediaRemoteNowPlayingInfoRepeatMode;
extern NSString* kMRMediaRemoteNowPlayingInfoShuffleMode;
extern NSString* kMRMediaRemoteNowPlayingInfoStartTime;
extern NSString* kMRMediaRemoteNowPlayingInfoSupportsFastForward15Seconds;
extern NSString* kMRMediaRemoteNowPlayingInfoSupportsIsBanned;
extern NSString* kMRMediaRemoteNowPlayingInfoSupportsIsLiked;
extern NSString* kMRMediaRemoteNowPlayingInfoSupportsRewind15Seconds;
extern NSString* kMRMediaRemoteNowPlayingInfoTimestamp;
extern NSString* kMRMediaRemoteNowPlayingInfoTitle;
extern NSString* kMRMediaRemoteNowPlayingInfoTotalChapterCount;
extern NSString* kMRMediaRemoteNowPlayingInfoTotalDiscCount;
extern NSString* kMRMediaRemoteNowPlayingInfoTotalQueueCount;
extern NSString* kMRMediaRemoteNowPlayingInfoTotalTrackCount;
extern NSString* kMRMediaRemoteNowPlayingInfoTrackNumber;
extern NSString* kMRMediaRemoteNowPlayingInfoUniqueIdentifier;
extern NSString* kMRMediaRemoteNowPlayingInfoRadioStationIdentifier;
extern NSString* kMRMediaRemoteNowPlayingInfoRadioStationHash;
extern NSString* kMRMediaRemoteOptionMediaType;
extern NSString* kMRMediaRemoteOptionSourceID;
extern NSString* kMRMediaRemoteOptionTrackID;
extern NSString* kMRMediaRemoteOptionStationID;
extern NSString* kMRMediaRemoteOptionStationHash;
extern NSString* kMRMediaRemoteRouteDescriptionUserInfoKey;
extern NSString* kMRMediaRemoteRouteStatusUserInfoKey;

typedef enum {
    /*
     * Use nil for userInfo.
     */
    kMRPlay = 0,
    kMRPause = 1,
    kMRTogglePlayPause = 2,
    kMRStop = 3,
    kMRNextTrack = 4,
    kMRPreviousTrack = 5,
    kMRToggleShuffle = 6,
    kMRToggleRepeat = 7,
    kMRStartForwardSeek = 8,
    kMREndForwardSeek = 9,
    kMRStartBackwardSeek = 10,
    kMREndBackwardSeek = 11,
    kMRGoBackFifteenSeconds = 12,
    kMRSkipFifteenSeconds = 13,

    /*
     * Use a NSDictionary for userInfo, which contains three keys:
     * kMRMediaRemoteOptionTrackID
     * kMRMediaRemoteOptionStationID
     * kMRMediaRemoteOptionStationHash
     */
    kMRLikeTrack = 0x6A,
    kMRBanTrack = 0x6B,
    kMRAddTrackToWishList = 0x6C,
    kMRRemoveTrackFromWishList = 0x6D
} MRCommand;

Boolean MRMediaRemoteSendCommand(MRCommand command, id userInfo);

void MRMediaRemoteSetPlaybackSpeed(int speed);
void MRMediaRemoteSetElapsedTime(double elapsedTime);

void MRMediaRemoteSetNowPlayingApplicationOverrideEnabled(Boolean enabled);

void MRMediaRemoteRegisterForNowPlayingNotifications(dispatch_queue_t queue);
void MRMediaRemoteUnregisterForNowPlayingNotifications();

void MRMediaRemoteBeginRouteDiscovery();
void MRMediaRemoteEndRouteDiscovery();

CFArrayRef MRMediaRemoteCopyPickableRoutes();

typedef void (^MRMediaRemoteGetNowPlayingInfoCompletion)(CFDictionaryRef information);
typedef void (^MRMediaRemoteGetNowPlayingApplicationPIDCompletion)(int PID);
typedef void (^MRMediaRemoteGetNowPlayingApplicationIsPlayingCompletion)(Boolean isPlaying);

void MRMediaRemoteGetNowPlayingApplicationPID(dispatch_queue_t queue, MRMediaRemoteGetNowPlayingApplicationPIDCompletion completion);
void MRMediaRemoteGetNowPlayingInfo(dispatch_queue_t queue, MRMediaRemoteGetNowPlayingInfoCompletion completion);
void MRMediaRemoteGetNowPlayingApplicationIsPlaying(dispatch_queue_t queue, MRMediaRemoteGetNowPlayingApplicationIsPlayingCompletion completion);

void MRMediaRemoteKeepAlive();
void MRMediaRemoteSetElapsedTime(double time);
void MRMediaRemoteSetShuffleMode(int mode);
void MRMediaRemoteSetRepeatMode(int mode);

/*
* The identifier can be obtained using MRMediaRemoteCopyPickableRoutes.
* Use the 'RouteUID' or the 'RouteName' key.
 */

int MRMediaRemoteSelectSourceWithID(CFStringRef identifier);
void MRMediaRemoteSetPickedRouteWithPassword(CFStringRef route, CFStringRef password);

CFArrayRef MRMediaRemoteCopyPickableRoutesForCategory(NSString *category);
Boolean MRMediaRemotePickedRouteHasVolumeControl();
void MRMediaRemoteSetCanBeNowPlayingApplication(Boolean can);
void MRMediaRemoteSetNowPlayingInfo(CFDictionaryRef information);
}
