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
        emit sigProgress(0, QString("Connecting to %1:%2").arg(m_cred.ip()).arg(m_cred.port()));

        if (t_api->connect(m_cred)) {
            emit sigProgress(10, QString("Successfully connected to %1:%2. Fetch the data model")
                                        .arg(m_cred.ip()).arg(m_cred.port()));

            // New Data Model
            Core::DataModelBuilder builder;
            t_api->model().fetchDataModel(builder);
            m_ied->setModel(builder.build(QCoreApplication::instance()->thread()));

            auto ident = t_api->getServIdentity();
            m_ied->setIdentify(ident);

            emit sigProgress(100, QString("Data model and other stuff were received from %1:%2")
                                         .arg(m_cred.ip()).arg(m_cred.port()));
            emit sigFinished(true);
            return;
        } else {
            qDebug() << QString("Cannot connect to %1:%2").arg(m_cred.ip()).arg(m_cred.port());
            QThread::sleep(1);
        }
		emit sigFinished(false);
	}

	void ConnectCmd::slotMsgProgress(const QString &t_msg)
	{
		emit sigProgress(77, t_msg);
	}
}