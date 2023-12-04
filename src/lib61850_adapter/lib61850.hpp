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

#include <memory>

#include "cmd/lib_interface.hpp"

// Forward declaration
struct sIedConnection;

namespace Core::Lib
{
	/*
	 * This class is Adapter for libiec61850 API
	 * */
	class Lib61850_Adapter : public QObject, public Cmd::LibInterface
	{
		Q_OBJECT
	public:
		Lib61850_Adapter() {}
		~Lib61850_Adapter() override {}

		bool	isConnected() const override;
		void	printfVersion() const override;

		bool	connect(const QString &t_ip, unsigned int t_port, bool t_checked,
						const QString &t_name, const QString &t_pass) override;
		void	disconnect() override;

		Core::DevServIdentity 	getServIdentity() override;
		int 	fetchDataModel(Core::DataModelBuilder &t_builder) override;

	private:
		int 	fetchLN_DO(Core::DataModelBuilder &t_builder);
		int 	fetchLN_DS(Core::DataModelBuilder &t_builder);
		int 	fetchLN_RCB(Core::DataModelBuilder &t_builder);
		int 	fetchLN_GOCB(Core::DataModelBuilder &t_builder);
		int 	fetchLN_SVCB(Core::DataModelBuilder &t_builder);

	public:
		int		updateLDs_Status(Core::ptrDataModel t_model) override;
		int		updateLNs_Status(Core::ptrLD t_ld) override;
		int		updateLN_Signals(Core::ptrLN t_ln) override;
		int		updateDS_Signals(Core::ptrDataSet t_ds) override;

		int		getFS_FileList(Core::DirOn &t_dir) override;
		void 	downloadFile(const QString &t_filename) override;
		int 	removeFile(const QString &t_filename) override;

		void 	callbackOnCloseEvent();

	signals:
		void 	sigFoundNode(const QString t_path);
		void 	sigConClosed();

	private:
		// libiec61850 stuff
		sIedConnection* m_libConn = nullptr;
	};
}