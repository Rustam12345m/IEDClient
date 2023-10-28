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

		void 				setContext(QQmlContext *t_context);

		Q_INVOKABLE QString getAppVersion();

		Q_INVOKABLE void 	connectTo(const QString &t_ip, unsigned int t_port, bool t_checked,
										const QString &t_name, const QString &t_pass);
		Q_INVOKABLE void 	disconnectFrom();

	public slots:
		void		slotConProcess(int t_proc, QString t_msg) {
			emit sigConProgress(t_proc, t_msg);
		}
		void 		slotConnected(bool t_done) {
			emit sigConnected(t_done);
		}

	signals:
		void		sigConProgress(int t_perc, QString t_msg);
		void 		sigConnected(bool t_done);

	protected:
		ConnectionObject	m_con; // Complex component of IED's stub

		// many presenters - backends
		ComBackend			m_comBackend;
		FS_Backend			m_fsBackend;
		LD_Backend			m_ldBackend;
	};
}