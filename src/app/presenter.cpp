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

#include "presenter.hpp"
#include "tools/dump_model.hpp"

namespace App
{
	Presenter::Presenter() : m_fsBackend(m_con), m_ldBackend(m_con), m_appBackend(m_con)
	{
		connect(&m_con, &AppConContainer::sigConnected, this, &Presenter::slotConnected);
	}

	void Presenter::setContextMembers(QQmlContext *t_context)
	{
		t_context->setContextProperty("presenter", this);
		t_context->setContextProperty("appBackend", &m_appBackend);
		t_context->setContextProperty("fsBackend", &m_fsBackend);
		t_context->setContextProperty("devBackend", &m_ldBackend);
	}

	void Presenter::connectTo(const QVariantMap &t_data)
	{
		Core::Cmd::ConCredentials cred(t_data);
		// m_appBackend.newConnection(cred);

		m_con.newConnection(cred);

		auto cmd = Core::Cmd::ConnectCmd::create(cred, m_con.m_iedObj);

		connect(cmd.get(), &Core::Cmd::ConnectCmd::sigFinished, this, &Presenter::slotConnected);
		connect(cmd.get(), &Core::Cmd::ConnectCmd::sigProgress, this, &Presenter::slotCmdProgress);
		connect(cmd.get(), &Core::Cmd::ConnectCmd::sigFinished, &m_ldBackend, &BackendBase::slotConnected);
		connect(cmd.get(), &Core::Cmd::ConnectCmd::sigFinished, &m_fsBackend, &BackendBase::slotConnected);

		m_con.m_cmdQueue->putCommand(cmd);
	}

	void Presenter::disconnectFrom()
	{
	}

	void Presenter::toolDumpModel(const QVariantMap &t_data)
	{
		Core::Cmd::ConCredentials con(t_data);
		QString dir = t_data.value("path").toString();

		Tools::DumpModel *dump = new Tools::DumpModel(this);

		connect(dump, &Tools::DumpModel::sigFinished, this, &Presenter::slotCmdFinished);
		connect(dump, &Tools::DumpModel::sigProgress, this, &Presenter::slotCmdProgress);

		// dump->init(t_dir, t_ip, t_port, t_tls, t_name, t_pass);
		// dump->start();
	}

	void Presenter::slotConnected(bool t_done)
	{
		emit sigConnected(t_done);

		m_appBackend.newConnection(m_con.m_cred);
	}
}