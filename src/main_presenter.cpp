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

MainPresenter::MainPresenter(QObject *t_parent) : QObject(t_parent)
{
	m_dev = new DeviceAgent(this);
	m_eventsModel = new EventsTableModel(this);

	m_fsModel = new FilesTableModel(this, m_dev->getFileTree());
	m_ldModel = new LD_ListModel(this, m_dev->getObjectTree());
	m_lnModel = new LN_TableModel(this, m_dev->getObjectTree());
	m_doModel = new DO_TableModel(this, m_dev->getObjectTree());
	m_sortDOModel = new SimpleProxyModel(this);
	m_lastConnModel = new LastConn_TableModel(this, m_ini);

	m_sortDOModel->setSourceModel(m_doModel);
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
											t_pass, m_dev->getObjectTree());
	putCmdToQueue(cmd);
}

void MainPresenter::disconnectFrom()
{
}

void MainPresenter::updateFilesDirectory(const QString &t_path)
{
	auto cmd = Core::Cmd::GetFileList::create(m_dev->getFileTree(), t_path);
	putCmdToQueue(cmd);
}

void MainPresenter::downloadFile(const QString &t_filename)
{
	qDebug() << "MainPresenter: Download file " << t_filename;
	auto cmd = Core::Cmd::DownloadFile::create(t_filename);
	putCmdToQueue(cmd);
}

void MainPresenter::removeFile(const QString &t_filename)
{
	qDebug() << "MainPresenter: Remove file " << t_filename;
	auto cmd = Core::Cmd::RemoveFile::create(t_filename);
	putCmdToQueue(cmd);
}

void MainPresenter::updateLNodeData(int t_ldIndex, int t_lnIndex)
{
	if (t_ldIndex < 0 || t_lnIndex < 0) {
		return;
	}

	auto cmd = Core::Cmd::UpdateLNode::create(m_dev->getObjectTree(), t_ldIndex, t_lnIndex);
	putCmdToQueue(cmd);
}

void MainPresenter::putCmdToQueue(Core::Cmd::ptrCMD t_cmd)
{
	connect(t_cmd.get(), SIGNAL(sigProgress(int,QString)), this, SLOT(slotCmdProcess(int,QString)));
	connect(t_cmd.get(), SIGNAL(sigFinished()), this, SLOT(slotCmdFinished()));

	m_dev->putCommand(t_cmd);
}

void MainPresenter::slotCmdProcess(int t_proc, QString t_msg)
{
	emit sigProgress(t_proc, t_msg);
}

void MainPresenter::slotCmdFinished()
{
	emit sigFinished();
}