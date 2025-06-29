// Copyright (c) 2017-2025 Manuel Schneider

#pragma once
#include "mpris.h"
#include <QString>

class Player
{
public:

    Player(const QString &service_name);

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

    QString dbus_service_name;
    OrgMprisMediaPlayer2Interface player;
    OrgMprisMediaPlayer2PlayerInterface control;
    QString name_;
    QString icon_url_;

};

