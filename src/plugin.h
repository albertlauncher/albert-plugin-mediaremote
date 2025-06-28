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
    // TODO extensionplugin should not inherit QOBJECT
    QString id() const override;
    QString name() const override;
    QString description() const override;
    std::vector<albert::Extension*> extensions() override;

public:

    Plugin();
    ~Plugin();

    std::vector<albert::RankItem> handleGlobalQuery(const albert::Query &) override;
    QWidget *buildConfigWidget() override;
    const std::map<QString, std::unique_ptr<albert::plugin::mediaremote::IPlayer>> &players() override;

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

    std::map<QString, std::unique_ptr<albert::plugin::mediaremote::IPlayer>> players_;

    struct Private;
    std::unique_ptr<Private> d;

};
