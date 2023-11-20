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
#include <QDebug>

namespace Core::Cmd
{
	void ConnectCmd::execute(LibInterface &t_con)
	{
		for (int i=0;i<3;i++) {
			emit sigProgress(0, QString("Connecting to %1:%2. Attempt (%3 / 3)").arg(m_ip).arg(m_port).arg(i + 1));

			if (t_con.connect(m_ip, m_port, m_tls, m_user, m_password)) {
				emit sigProgress(10, QString("Successfully connected to %1:%2").arg(m_ip).arg(m_port));
				// QThread::sleep(1);

				emit sigProgress(20, QString("Fetch data model from %1:%2").arg(m_ip).arg(m_port));
				// QThread::sleep(1);

				// New Data Model
				Core::DataModelBuilder builder;
				t_con.fetchDataModel(builder);
				m_ied->setModel(builder.build());

				// Debug
				//m_ied->model().print();

				emit sigProgress(100, QString("Data model and other stuff were received from %1:%2").arg(m_ip).arg(m_port));
				emit sigFinished(true);
				return;
			} else {
				qDebug() << QString("Cannot connect to %1:%2").arg(m_ip).arg(m_port);
				QThread::sleep(3);
			}
		}
		emit sigFinished(false);
	}

	void ConnectCmd::slotMsgProgress(const QString &t_msg)
	{
		emit sigProgress(77, t_msg);
	}
}