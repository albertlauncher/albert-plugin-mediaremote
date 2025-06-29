// Copyright (c) 2017-2025 Manuel Schneider

#pragma once
#include <QString>
#include <objc/runtime.h>

class Player
{
public:

    Player(int pid);
    ~Player();

    QString name() const;
    QString iconUrl() const;

    bool isPlaying() const;
    bool canPlay() const;
    bool canPause() const;
    bool canGoNext() const;
    bool canGoPrevious() const;

    void play();
    void pause();
    void next();
    void previous();

private:

    bool is_playing_;
    QString name_;
    QString icon_url_;
    ::id notificationObservation_;

};


