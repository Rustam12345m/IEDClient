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

#include "main_presenter.hpp"
#include "builder_information.hpp"

MainPresenter::MainPresenter(AppCore &t_core)
	: QObject(nullptr),
	m_core(t_core),
	m_eventsModel(this),
	m_fsModel(this, m_core.getFSTree()),
	m_ldModel(this, m_core.getObjectTree()),
	m_lnModel(this, m_core.getObjectTree()),
	m_doModel(this, m_core.getObjectTree()),
	m_sortDOModel(this)
{
	m_sortDOModel.setSourceModel(&m_doModel);

	qDebug() << "Application: " << PROJECT_VERSION << "\r\n";
}

MainPresenter::~MainPresenter()
{
}

void MainPresenter::connectTo(const QString &t_ip, int t_port, bool t_tls,
						const QString &t_name, const QString &t_pass)
{
	qDebug() << QString("Presenter: ConnectTo %1:%2 %3 %4 %5").arg(t_ip).arg(t_port)
				.arg(t_tls).arg(t_name).arg(t_pass);

	auto cmd = Core::Cmd::ConnectCmd::create(t_ip, t_port, t_tls, t_name,
											t_pass, m_core.getObjectTree());
	putCmdToCore(cmd);
}

void MainPresenter::disconnectFrom()
{
}

void MainPresenter::viewFilesDirectory(const QString &t_path)
{
	auto cmd = Core::Cmd::GetFileList::create(m_core.getFSTree(), t_path);
	putCmdToCore(cmd);
}

void MainPresenter::updateLNodeData(int t_ldIndex, int t_lnIndex)
{
	if (t_ldIndex < 0 || t_lnIndex < 0) {
		return;
	}

	auto cmd = Core::Cmd::UpdateLNode::create(m_core.getObjectTree(), t_ldIndex, t_lnIndex);
	putCmdToCore(cmd);
}

void MainPresenter::putCmdToCore(Core::Cmd::ptrCMD t_cmd)
{
	connect(t_cmd.get(), SIGNAL(sigProgress(int,QString)), this, SLOT(slotCmdProcess(int,QString)));
	connect(t_cmd.get(), SIGNAL(sigFinished()), this, SLOT(slotCmdFinished()));

	m_core.putCommand(t_cmd);
}

void MainPresenter::slotCmdProcess(int t_proc, QString t_msg)
{
	emit sigProgress(t_proc, t_msg);
}

void MainPresenter::slotCmdFinished()
{
	emit sigFinished();
}
