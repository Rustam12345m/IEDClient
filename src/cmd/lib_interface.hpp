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

#include <QString>

#include "core/server_identify.hpp"
#include "core/data_model_builder.hpp"

namespace Core::Cmd
{
	class LibInterface
	{
	public:
		virtual ~LibInterface() {}

		virtual bool	isConnected() const = 0;
		virtual QString	getLibVersion() const = 0;

		virtual bool	connect(const QString &t_ip, unsigned int t_port, bool t_checked,
								const QString &t_name, const QString &t_pass) = 0;
		virtual void	disconnect() = 0;

		virtual Core::DevServIdentity 	getServIdentity() = 0;
		virtual int 	fetchDataModel(Core::DataModelBuilder &t_builder) = 0;

		virtual int		updateLDs_Status(Core::ptrDataModel t_model) = 0;
		virtual int		updateLNs_Status(Core::ptrLD t_ld) = 0;
		virtual int		updateLN_Signals(Core::ptrLN t_ln) = 0;
		virtual int		updateDS_Signals(Core::ptrDataSet t_ds) = 0;

		virtual int		getFileList(Core::DirOn &t_dir) = 0;
		virtual void 	downloadFile(const QString &t_filename) = 0;
		virtual int 	removeFile(const QString &t_filename) = 0;
	};
}