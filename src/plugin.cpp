// Copyright (c) 2017-2025 Manuel Schneider

#include "plugin.h"
#include "ui_configwidget.h"
#include <albert/logging.h>
#include <albert/matcher.h>
#include <albert/standarditem.h>
#include <QUrl>
ALBERT_LOGGING_CATEGORY("mediaplayerremote")
using namespace Qt::StringLiterals;
using namespace albert::util;
using namespace albert;
using namespace std;

static inline shared_ptr<Item> makeItem(const QString &cmd,
                                        const QString &player_name,
                                        const QStringList &icon_urls,
                                        function<void()> &&action)
{
    return StandardItem::make(cmd,
                              cmd,
                              player_name,
                              icon_urls,
                              {{ cmd, cmd, ::move(action)}});
}


static QStringList composedIcon(QString player_icon_url, const QString &control_emoji)
{
    return {
        u"comp:?src1=%1&src2=%2&size1=0.9&size2=0.6"_s
            .arg(QString::fromUtf8(QUrl::toPercentEncoding(player_icon_url)),
                 QString::fromUtf8(QUrl::toPercentEncoding(u"gen:?text="_s + control_emoji)))
    };
}

vector<RankItem> Plugin::handleGlobalQuery(const Query &query)
{
    Matcher matcher(query);
    vector<RankItem> results;

    for (const auto &player : players_)
    {
        auto &p = *player.second;

        if (auto m = matcher.match(strings.next, tr_strings.next, p.name());
            m && p.canGoNext())
            results.emplace_back(makeItem(tr_strings.next,
                                          p.name(),
                                          composedIcon(p.iconUrl(), u"⏭️"_s),
                                          [this, pid = p.name()] {
                                              if (auto it = players_.find(pid); it != players_.end())
                                                  it->second->next();
                                          }),
                                 m);

        if (auto m = matcher.match(strings.previous, tr_strings.previous, p.name());
            m && p.canGoPrevious())
            results.emplace_back(makeItem(tr_strings.previous,
                                          p.name(),
                                          composedIcon(p.iconUrl(), u"⏮️"_s),
                                          [this, pid=p.name()] {
                                              if (auto it = players_.find(pid);
                                                  it != players_.end())
                                                  it->second->previous();
                                          }),
                                 m);

        if (p.isPlaying())
        {
            if (auto m = matcher.match(strings.pause, tr_strings.pause, p.name());
                m && p.canPause())
                results.emplace_back(makeItem(tr_strings.pause,
                                              p.name(),
                                              composedIcon(p.iconUrl(), u"⏸️"_s),
                                              [this, pid=p.name()]
                                              {
                                                  if (auto it = players_.find(pid);
                                                      it != players_.end())
                                                      it->second->pause();
                                              }),
                                     m);
        }
        else
        {
            if (auto m = matcher.match(strings.play, tr_strings.play, p.name());
                m && p.canPlay())
                results.emplace_back(makeItem(tr_strings.play,
                                              p.name(),
                                              composedIcon(p.iconUrl(), u"▶️"_s),
                                              [this, pid=p.name()]
                                              {
                                                  if (auto it = players_.find(pid);
                                                      it != players_.end())
                                                      it->second->play();
                                              }),
                                     m);
        }
    }

    return results;
}

QWidget *Plugin::buildConfigWidget()
{
    auto *w = new QWidget();
    Ui::ConfigWidget ui;
    ui.setupUi(w);
    return w;
}
