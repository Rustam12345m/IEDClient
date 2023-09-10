/*
 *  main.cpp
 *
 *  Copyright 2023-2023 Rustam Mustafin
 *
 *  This file is part of IEDMaster.
 *
 *  IEDMaster is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  IEDMaster is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with IEDMaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 * */

#pragma once

#include "app_core.h"

#include "models/fs_tablemodel.h"
#include "models/events_tablemodel.h"
#include "models/ld_listmodel.h"
#include "models/ln_tablemodel.h"
#include "models/do_tablemodel.h"

/*
 * Presenter for QML part
 * */
class MainPresenter : public QObject
{
	Q_OBJECT
	Q_PROPERTY(FilesTableModel* filesModel READ getFilesModel CONSTANT)
	Q_PROPERTY(EventsTableModel* eventsModel READ getEventsModel CONSTANT)
	Q_PROPERTY(LD_ListModel* ldModel READ getLD_Model CONSTANT)
	Q_PROPERTY(LN_TableModel* lnModel READ getLN_Model CONSTANT)
	Q_PROPERTY(DO_TableModel* doModel READ getDO_Model CONSTANT)

private:
	AppCore&			m_core;

	// Models for Tables in GUI
	EventsTableModel	m_eventsModel;
	FilesTableModel		m_fsModel;
	LD_ListModel		m_ldModel;
	LN_TableModel		m_lnModel;
	DO_TableModel		m_doModel;

public:
	FilesTableModel*	getFilesModel() {
		return &m_fsModel;
	}
	EventsTableModel*	getEventsModel() {
		return &m_eventsModel;
	}
	LD_ListModel*		getLD_Model() {
		return &m_ldModel;
	}
	LN_TableModel*		getLN_Model() {
		return &m_lnModel;
	}
	DO_TableModel*		getDO_Model() {
		return &m_doModel;
	}

public:
	MainPresenter(AppCore &t_core);
	~MainPresenter();

	Q_INVOKABLE void connectTo(const QString &t_ip, int t_port, bool t_tls,
								const QString &t_name, const QString &t_pass);
	Q_INVOKABLE void disconnectFrom();

	Q_INVOKABLE void viewFilesDirectory(const QString &t_path);
	Q_INVOKABLE void updateLNodeData(int t_ldIndex, int t_lnIndex);

private:
	void	putCmdToCore(Core::Cmd::ptrCMD t_cmd);

signals:
	void	sigProgress(int t_perc, QString t_msg);
	void	sigFinished();

public slots:
	void	slotCmdProcess(int t_proc, QString t_msg);
	void	slotCmdFinished();
};
