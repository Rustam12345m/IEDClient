/*
 *  Copyright 2023 Rustam Mustafin
 *
 *  This file is part of IEDClient.
 *
 *  IEDClient is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  IEDClient is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with IEDClient.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 * */

#pragma once

#include <QObject>
#include <QQmlContext>

#include "app/app_backend.hpp"
#include "app/ied_backend.hpp"
#include "app/ied_fs_backend.hpp"

namespace App
{
    /**
     * @brief MainPresenter for QML contains specific backends like FS, LD
     */
    class MainPresenter : public QObject
    {
        Q_OBJECT
    public:
        MainPresenter();
        ~MainPresenter() = default;

        void setQmlContextMembers(QQmlContext *t_context);

        Q_PROPERTY(QVariant iedConStatus READ getIEDConStatus NOTIFY sigIEDConChanged)
        Q_INVOKABLE QVariant getIEDConStatus();

        // API for QML
        Q_INVOKABLE void connectTo(const QVariantMap &t_data);
        Q_INVOKABLE void disconnectFrom();
        Q_INVOKABLE void toolDumpModel(const QVariantMap &t_data);
        Q_INVOKABLE bool isConnected() { return m_con.isConnected(); }

    public slots:
        void slotCmdEvent(Cmd::CmdEvent t_ev);
        void slotConClosed();

    signals:
        void sigCmdProgress(int t_perc, QString t_msg);
        void sigCmdFinished(bool t_done);
        void sigIEDConChanged(bool t_done);

    protected:
        IEDConContainer m_con; // Complex component of IED's stub
        EventStorage    m_events;

        // Backends for QML
        AppBackend      m_appBackend;
        IED_Backend     m_iedBackend;
        IED_FS_Backend  m_fsBackend;
    };
}
