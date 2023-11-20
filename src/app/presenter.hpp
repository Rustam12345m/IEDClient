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

// bk
#include "app/com_backend.hpp"
#include "app/fs_backend.hpp"
#include "app/ld_backend.hpp"

namespace App
{
	/*
	 * Presenter for QML contains specific backends like FS, LD
	 * */
	class Presenter : public QObject
	{
		Q_OBJECT
	public:
		Presenter();
		~Presenter() = default;

		void 		setContextMembers(QQmlContext *t_context);

		Q_PROPERTY(bool isConnected READ isConnected NOTIFY sigConnected)
		bool 		isConnected() {
			return m_con.isConnected();
		}

		// API for QML
		Q_INVOKABLE void 	connectTo(const QString &t_ip, unsigned int t_port, bool t_checked,
										const QString &t_name, const QString &t_pass);
		Q_INVOKABLE void 	disconnectFrom();
		Q_INVOKABLE QString getAppVersion();

		Q_INVOKABLE void 	toolDumpModel(const QString &t_dir, const QString &t_ip, unsigned int t_port,
										bool t_checked, const QString &t_name, const QString &t_pass);

	public slots:
		void		slotCmdProgress(int t_proc, QString t_msg) {
			emit sigCmdProgress(t_proc, t_msg);
		}
		void		slotCmdFinished(bool t_done) {
			emit sigCmdFinished(t_done);
		}
		void 		slotConnected(bool t_done) {
			emit sigConnected(t_done);
		}

	signals:
		void		sigCmdProgress(int t_perc, QString t_msg);
		void		sigCmdFinished(bool t_done);
		void 		sigConnected(bool t_done);

	protected:
		IED_Connection	m_con; // Complex component of IED's stub

		// Backends for QML
		ComBackend		m_comBackend;
		LD_Backend		m_ldBackend;
		FS_Backend		m_fsBackend;
	};
}