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

#include "dump_model.hpp"
#include "dump_writer.hpp"
#include "lib61850_adapter/lib61850_tools.hpp"

#include <QDebug>

namespace Tools
{
	DumpModel::DumpModel(QObject *t_parent) : QThread(t_parent)
	{
	}

	DumpModel::~DumpModel()
	{
		qDebug() << "DumpModel: Destructor";
	}

	void DumpModel::init(const QString &t_dir, const QString &t_ip, unsigned int t_port,
						 bool t_tls, const QString &t_name, const QString &t_pass)
	{
		m_fileDirectory = t_dir;
		m_ip = t_ip;
		m_port = t_port;
		m_tls = t_tls;
		m_user = t_name;
		m_password = t_pass;
	}

	void DumpModel::run()
	{
		qDebug() << "DumpModel start process";
		for (int i=0;i<10;i++) {
			emit sigProgress(i * 5, QString("Progress is %1").arg(i * 5));
			QThread::sleep(1);
		}
		emit sigFinished(true);

		qDebug() << "DumpModel finish";
		this->deleteLater();
	}
}