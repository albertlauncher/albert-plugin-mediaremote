// Copyright (c) 2024-2025 Manuel Schneider

#pragma once
#include <QObject>
#include <QString>
#include <albert/export.h>
class QNetworkRequest;

namespace albert::plugin::mediaremote {

class ALBERT_EXPORT IPlugin : public QObject
{
    Q_OBJECT
public:

    virtual QString player() = 0;  // may be null
    virtual bool isPlaying() = 0;

    virtual bool canPlay() = 0;
    virtual bool canPause() = 0;
    virtual bool canGoNext() = 0;
    virtual bool canGoPrevious() = 0;

    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void next() = 0;
    virtual void previous() = 0;

signals:

    void isPlayingChanged(bool);
    void playerChanged(QString);  // may be null

protected:

    virtual ~IPlugin() = 0;

};

}

