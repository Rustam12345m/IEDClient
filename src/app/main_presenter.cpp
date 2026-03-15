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

#include "main_presenter.hpp"
#include "tools/dump_model.hpp"

namespace App
{
    MainPresenter::MainPresenter(): m_appBackend(m_con, m_events),
        m_iedBackend(m_con, m_events), m_fsBackend(m_con, m_events)
    {
        connect(&m_con, &IEDConContainer::sigConClosed, this, &MainPresenter::slotConClosed);
    }

    void MainPresenter::setQmlContextMembers(QQmlContext *t_context)
    {
        t_context->setContextProperty("presenter", this);
        t_context->setContextProperty("appBackend", &m_appBackend);
        t_context->setContextProperty("iedBackend", &m_iedBackend);
        t_context->setContextProperty("fsBackend", &m_fsBackend);
    }

    QVariant MainPresenter::getIEDConStatus()
    {
        QVariantMap retval;
        if (isConnected()) {
            retval["isConnected"] = true;
            retval["text"] = QString(tr("Connected to %1:%2"))
                                    .arg(m_con.m_cred.ip())
                                    .arg(m_con.m_cred.port());
        } else if (!m_con.m_cred.ip().isEmpty()) {
            retval["isConnected"] = false;
            retval["text"] = QString(tr("Disconnected from %1:%2"))
                                    .arg(m_con.m_cred.ip())
                                    .arg(m_con.m_cred.port());
        } else {
            retval["isConnected"] = false;
            retval["text"] = tr("Not connected");
        }
        return retval;
    }

    void MainPresenter::connectTo(const QVariantMap &t_data)
    {
        qDebug() << "MainPresenter: Connect cmd";

        Cmd::IEDCredentials cred(t_data);
        m_con.createNewConnection(cred);

        auto cmd = Cmd::ConnectCmd::create(m_con.m_cred, m_con.m_ied);

        connect(cmd.get(), &Cmd::ConnectCmd::sigCmdEvent, this, &MainPresenter::slotCmdEvent);

        m_con.m_cmdThread->putCommand(cmd);
    }

    void MainPresenter::disconnectFrom()
    {
        qDebug() << "MainPresenter: Disconnect cmd";

        auto cmd = Cmd::DisConnectCmd::create(m_con.m_ied);
        m_con.m_cmdThread->putCommand(cmd);
    }

    void MainPresenter::toolDumpModel(const QVariantMap &t_data)
    {
        /*
        Cmd::IEDCredentials con(t_data);
        QString dir = t_data.value("path").toString();

        Tools::DumpModel *dump = new Tools::DumpModel(this);

        connect(dump, &Tools::DumpModel::sigFinished, this, &MainPresenter::slotCmdFinished);
        connect(dump, &Tools::DumpModel::sigProgress, this, &MainPresenter::slotCmdProgress);
        */

        // dump->init(t_dir, t_ip, t_port, t_tls, t_name, t_pass);
        // dump->start();
    }

    void MainPresenter::slotCmdEvent(Cmd::CmdEvent t_ev)
    {
        m_events.putEventToStorage(t_ev);

        switch (t_ev.m_type) {
        case Cmd::PROCESS_EVENT: {
            emit sigCmdProgress(t_ev.m_perc, t_ev.m_msg);
            break;
        }
        case Cmd::FINISH_EVENT: {
            m_iedBackend.slotConnected(t_ev.m_result);
            m_fsBackend.slotConnected(t_ev.m_result);
            m_appBackend.saveCredsToHistory(m_con.m_cred);

            emit sigCmdFinished(t_ev.m_result);
            emit sigIEDConChanged(t_ev.m_result);
            break;
        }
        case Cmd::START_EVENT:
        case Cmd::UNDEFINED_EVENT: {
            break;
        }
        }
    }

    void MainPresenter::slotConClosed()
    {
        m_iedBackend.slotConnected(false);
        m_fsBackend.slotConnected(false);

        emit sigIEDConChanged(false);
    }
}
