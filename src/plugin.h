// Copyright (c) 2017-2025 Manuel Schneider

#pragma once
#include <albert/extensionplugin.h>
#include <albert/globalqueryhandler.h>
#include "player.h"

// Expected player interface
// class Player
// {
// public:
//     QString name() const;
//     QString iconUrl() const;
//     bool isPlaying() const;
//     bool canPlay() const;
//     bool canPause() const;
//     bool canGoNext() const;
//     bool canGoPrevious() const;
//     void play();
//     void pause();
//     void next();
//     void previous();
// };

class Plugin : public albert::util::ExtensionPlugin,
               public albert::GlobalQueryHandler
{
    ALBERT_PLUGIN

public:

    Plugin();
    ~Plugin();

    std::vector<albert::RankItem> handleGlobalQuery(const albert::Query &) override;

protected:

    std::map<QString, std::shared_ptr<Player>> players_;

    struct Private;
    std::unique_ptr<Private> d;

};
