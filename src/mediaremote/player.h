// Copyright (c) 2017-2025 Manuel Schneider

// https://github.com/khangthk/SketchyBar/blob/5b7d0e07cf41ae8413393b1bb8f5c184a6615695/src/media.m

#pragma once
#include <QObject>
#include <memory>
#include <objc/runtime.h>


class Player : public QObject
{
    Q_OBJECT

public:

    Player(int pid);
    ~Player();

    QString name() const;
    QString iconUrl() const;

    bool isPlaying() const;
    // QString isPlayingTitle() const { return isPlayingTitle_; }
    // QString isPlayingInfo() const { return isPlayingInfo_; }

    bool canPlay() const;
    bool canPause() const;
    bool canGoNext() const;
    bool canGoPrevious() const;

    void play();
    void pause();
    void next();
    void previous();

signals:

    void isPlayingChanged(bool);

private:

    bool is_playing_;
    // QString id_;
    QString name_;
    QString icon_url_;
    QString isPlayingTitle_;
    QString isPlayingInfo_;
    ::id notificationObservation_;

};


