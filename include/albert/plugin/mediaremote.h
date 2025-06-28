// Copyright (c) 2024-2025 Manuel Schneider

#pragma once
#include <QObject>
#include <QString>
#include <map>
#include <albert/export.h>
class QNetworkRequest;

namespace albert::plugin::mediaremote {

class ALBERT_EXPORT IPlayer : public QObject
{
    Q_OBJECT
public:

    // virtual QString id() const = 0;
    virtual QString name() const = 0;
    virtual QString iconUrl() const = 0;

    virtual bool isPlaying() const = 0;
    // virtual QString isPlayingTitle() const = 0;
    // virtual QString isPlayingInfo() const = 0;

    virtual bool canPlay() const = 0;
    virtual bool canPause() const = 0;
    virtual bool canGoNext() const = 0;
    virtual bool canGoPrevious() const = 0;

    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void next() = 0;
    virtual void previous() = 0;


    virtual ~IPlayer() = default;;
signals:

    void isPlayingChanged(bool);

protected:


};


class ALBERT_EXPORT IPlugin : public QObject
{
    Q_OBJECT

public:

    virtual const std::map<QString, std::unique_ptr<IPlayer>> &players() = 0;

signals:

    void playersChanged();

protected:

    virtual ~IPlugin() = 0;

};

}

