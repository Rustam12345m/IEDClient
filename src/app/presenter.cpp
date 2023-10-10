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

#include "presenter.hpp"
#include "builder_information.hpp"

namespace App
{
	Presenter::Presenter()
		: m_fsBackend(m_con), m_ldBackend(m_con), m_comBackend(m_con)
	{
	}

	void Presenter::setContext(QQmlContext *t_context)
	{
		t_context->setContextProperty("presenter", this);
		t_context->setContextProperty("comBackend", &m_comBackend);
		t_context->setContextProperty("fsBackend", &m_fsBackend);
		t_context->setContextProperty("ldBackend", &m_ldBackend);
	}

	QString Presenter::getAppVersion()
	{
		return QString("IEDClient v%1").arg(PROJECT_VERSION);
	}

	void Presenter::connectTo(const QString &t_ip, unsigned int t_port, bool t_tls,
							const QString &t_name, const QString &t_pass)
	{
		m_con.reset();

		auto cmd = Core::Cmd::ConnectCmd::create(t_ip, t_port, t_tls, t_name,
												t_pass, m_con.m_ied->tree());

		connect(cmd.get(), &Core::Cmd::ConnectCmd::sigFinished, &m_fsBackend, &BackendBase::slotNewIED);
		connect(cmd.get(), &Core::Cmd::ConnectCmd::sigFinished, &m_ldBackend, &BackendBase::slotNewIED);

		connect(cmd.get(), &Core::Cmd::ConnectCmd::sigFinished, this, &Presenter::slotConnected);
		connect(cmd.get(), &Core::Cmd::ConnectCmd::sigProgress, this, &Presenter::slotConProcess);

		m_con.m_cmdQueue->putCommand(cmd);
	}

	void Presenter::disconnectFrom()
	{
	}
}