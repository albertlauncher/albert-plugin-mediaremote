// Copyright (c) 2017-2025 Manuel Schneider

// https://github.com/khangthk/SketchyBar/blob/5b7d0e07cf41ae8413393b1bb8f5c184a6615695/src/media.m

#pragma once
#include <QObject>
#include "mpris.h"


class Player : public QObject
{
    Q_OBJECT

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

signals:

    void isPlayingChanged(bool);

private:

    const QString dbus_service_name;
    OrgMprisMediaPlayer2Interface player;
    OrgMprisMediaPlayer2PlayerInterface control;
    QString name_;
    QString icon_url_;

};


