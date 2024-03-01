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

#include "connect_cmd.hpp"
#include "core/data_model_builder.hpp"

#include <QThread>
#include <QCoreApplication>
#include <QDebug>

namespace Cmd
{
	void ConnectCmd::execute(Cmd::Interface::ptrIEC61850_API t_api)
	{
        emit sigCmdEvent(CmdEvent::StartEvent(m_cred.ip(),
                QString(tr("Try to connect to %1:%2")).arg(m_cred.ip()).arg(m_cred.port())));

        if (!t_api->connect(m_cred)) {
            emit sigCmdEvent(CmdEvent::FinishEvent(m_cred.ip(),
                    QString(tr("Can't connect to %1:%2")).arg(m_cred.ip()).arg(m_cred.port()),
                    false));
            return;
        }

        connect(&t_api->model(), &Cmd::Interface::IED_ModelAPI::sigFoundNode,
                this, &ConnectCmd::slotFoundNewLN);

        m_percProgress = 50;
        emit sigCmdEvent(CmdEvent::ProcessEvent(m_cred.ip(),
                QString(tr("Successfully connected to %1:%2. Fetch the data model"))
                        .arg(m_cred.ip()).arg(m_cred.port()), m_percProgress));

        Core::DataModelBuilder builder;
        t_api->model().fetchDataModel(builder);
        m_ied->setModel(builder.build(QCoreApplication::instance()->thread()));

        auto ident = t_api->getServIdentity();
        m_ied->setIdentify(ident);

        emit sigCmdEvent(CmdEvent::FinishEvent(m_cred.ip(), 
                QString(tr("IEDClient successfully connected to %1:%2"))
                        .arg(m_cred.ip()).arg(m_cred.port()),
                true));
	}

	void ConnectCmd::slotFoundNewLN(const QString &t_msg)
	{
        m_percProgress += 5;
        if (m_percProgress >= 99) {
            m_percProgress = 99;
        }
        emit sigCmdEvent(CmdEvent::ProcessEvent(m_cred.ip(), t_msg, m_percProgress));
	}
}