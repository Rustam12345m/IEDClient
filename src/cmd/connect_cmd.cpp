/*
 *  main.cpp
 *
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
#include "core/ied_tree.hpp"

#include <QThread>
#include <QDebug>

namespace Core::Cmd
{
	void ConnectCmd::execute(LibInterface &t_con)
	{
		/*
		qDebug() << QString("Connect request to %1:%2").arg(m_ip).arg(m_port);
		for (int i=0;i<10;i++) {
			emit sigProgress(i * 10, "Perc msg = " + QString::number(i * 10));

			QThread::msleep(500);
		}
		emit sigFinished();
		return;
		*/
		//QThread::msleep(10 * 1000);

		if (t_con.connect(m_ip, m_port, m_tls, m_user, m_password)) {
			qDebug() << QString("Connected to %1:%2").arg(m_ip).arg(m_port);

			emit sigProgress(30, QString("Successfully connected to %1:%2").arg(m_ip).arg(m_port));
			QThread::sleep(1); // Debug

			// Get LD & LN list
			int retval = t_con.getLD_List(m_tree);
			if (retval == 0) {

				for (size_t i=0;i<m_tree.getChildCount();i++) {
					auto ld = m_tree.getChild<LogicalDevice>(i);

					emit sigProgress(50, QString("Received %1 for LD: %2").arg(ld->getChildCount()).arg(ld->name()));

					for (size_t j=0;j<ld->getChildCount();j++) {
						auto ln = ld->getChild<LogicalNode>(j);

						// Get LN's DataObjects
						retval = t_con.getDO_List(ln);
						if (retval == 0) {
							// Tables
							auto doTable = LN_FlatBuilder::create(ln);
							ln->setDO_Table(doTable);
						}

						emit sigProgress(50, QString("Found %1 data object for %2/%3")
												.arg(ln->getChildCount()).arg(ld->name(), ln->name()));
					}
				}
			}
			m_tree.update();
			//m_tree.printTree();

			emit sigProgress(100, QString("Successfully connected to %1:%2").arg(m_ip).arg(m_port));
		} else {
			qDebug() << QString("Cannot connect to %1:%2").arg(m_ip).arg(m_port);

			emit sigProgress(100, QString("Cannot connect to %1:%2").arg(m_ip).arg(m_port));
		}
		emit sigFinished();
	}
}