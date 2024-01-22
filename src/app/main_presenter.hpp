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

#pragma once

#include <QObject>
#include <QQmlContext>

#include "app/app_status.hpp"

// other specific presenters for GUI
#include "app/app_presenter.hpp"
#include "app/fs_presenter.hpp"
#include "app/ied_presenter.hpp"

namespace App
{
	/*
	 * MainPresenter for QML contains specific backends like FS, LD
	 * */
	class MainPresenter : public QObject
	{
		Q_OBJECT
	public:
		MainPresenter();
		~MainPresenter() = default;

		void 		setContextMembers(QQmlContext *t_context);

		Q_PROPERTY(bool isConnected READ isConnected NOTIFY sigConnected)
		bool 		isConnected() {
			return m_con.isConnected();
		}

		// API for QML
		Q_INVOKABLE void 	connectTo(const QVariantMap &t_data);
		Q_INVOKABLE void 	disconnectFrom();

		Q_INVOKABLE void 	toolDumpModel(const QVariantMap &t_data);

	public slots:
		void		slotCmdProgress(int t_proc, QString t_msg) {
			emit sigCmdProgress(t_proc, t_msg);
		}
		void		slotCmdFinished(bool t_done) {
			emit sigCmdFinished(t_done);
		}
		void 		slotConnected(bool t_done);

	signals:
		void		sigCmdProgress(int t_perc, QString t_msg);
		void		sigCmdFinished(bool t_done);
		void 		sigConnected(bool t_done);

	protected:
		AppConContainer	m_con; // Complex component of IED's stub

		// Backends for QML
		AppBackend		m_appBackend;
		IED_Presenter	m_ldBackend;
		FS_Backend		m_fsBackend;
	};
}