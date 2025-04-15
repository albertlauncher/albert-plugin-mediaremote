// Copyright (c) 2017-2024 Manuel Schneider

#pragma once
#include "mediaremote.h"
#include <albert/plugininstance.h>
#include <albert/globalqueryhandler.h>

class Plugin : public albert::plugin::mediaremote::IPlugin,
               public albert::PluginInstance,
               public albert::GlobalQueryHandler

{
    ALBERT_PLUGIN

public:

    Plugin();
    ~Plugin();

    // TODO extensionplugin should not inherit QOBJECT
    QString id() const override;
    QString name() const override;
    QString description() const override;
    std::vector<albert::Extension*> extensions() override;

    std::vector<albert::RankItem> handleGlobalQuery(const albert::Query &) override;
    QWidget *buildConfigWidget() override;

    QString player() override;
    bool isPlaying() override;

    void next() override;
    void pause() override;
    void play() override;
    void previous() override;

    bool canGoNext() override;
    bool canGoPrevious() override;
    bool canPause() override;
    bool canPlay() override;

    struct {
        QString play = "Play";
        QString pause = "Pause";
        QString next = "Next";
        QString previous = "Previous";
        QString playing = "Playing";
        QString paused = "Paused";
        QString stopped = "Stopped";
    } const strings;

    struct {
        QString play = tr("Play");
        QString pause = tr("Pause");
        QString next = tr("Next");
        QString previous = tr("Previous");
        QString playing = tr("Playing");
        QString paused = tr("Paused");
        QString stopped = tr("Stopped");
    } const ui_strings;

private:

    struct Private;
    std::unique_ptr<Private> d;
    QString player_;
    bool is_playing_;

};
