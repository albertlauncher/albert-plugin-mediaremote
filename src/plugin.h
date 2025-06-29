// Copyright (c) 2017-2024 Manuel Schneider

#pragma once
#include <albert/extensionplugin.h>
#include <albert/globalqueryhandler.h>
#include "player.h"

// Expected player interface
// class Player : public QObject
// {
//     Q_OBJECT
// public:
//     QString name() const;
//     QString iconUrl() const;
//     bool isPlaying() const;
//     QString isPlayingTitle() const;
//     QString isPlayingInfo() const;
//     bool canPlay() const;
//     bool canPause() const;
//     bool canGoNext() const;
//     bool canGoPrevious() const;
//     void play();
//     void pause();
//     void next();
//     void previous();
// signals:
//     void isPlayingChanged(bool);
// };

class Plugin : public albert::util::ExtensionPlugin,
               public albert::GlobalQueryHandler

{
    ALBERT_PLUGIN

public:

    Plugin();
    ~Plugin();

    std::vector<albert::RankItem> handleGlobalQuery(const albert::Query &) override;
    QWidget *buildConfigWidget() override;

    struct {
        QString play     = QStringLiteral("Play");
        QString pause    = QStringLiteral("Pause");
        QString next     = QStringLiteral("Next");
        QString previous = QStringLiteral("Previous");
        QString playing  = QStringLiteral("Playing");
        QString paused   = QStringLiteral("Paused");
        QString stopped  = QStringLiteral("Stopped");
    } const strings;

    struct {
        QString play     = tr("Play");
        QString pause    = tr("Pause");
        QString next     = tr("Next");
        QString previous = tr("Previous");
        QString playing  = tr("Playing");
        QString paused   = tr("Paused");
        QString stopped  = tr("Stopped");
    } const tr_strings;

protected:

    std::map<QString, std::shared_ptr<Player>> players_;

    struct Private;
    std::unique_ptr<Private> d;

};
