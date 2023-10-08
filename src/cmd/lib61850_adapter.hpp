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

#include <memory>

#include "lib_interface.hpp"

// Forward declaration
struct sIedConnection;

namespace Core::Cmd
{
	/*
	 * This class is Adapter for libiec61850 API
	 * */
	class Lib61850 : public LibInterface
	{
	private:
		// libiec61850 stuff
		sIedConnection* m_libConn = nullptr;

	public:
		Lib61850() {}
		~Lib61850() override {}

		bool	isConnected() const override {
			return (m_libConn != nullptr);
		}
		void	printfVersion() const override;

		bool	connect(const QString &t_ip, unsigned int t_port, bool t_checked,
						const QString &t_name, const QString &t_pass) override;
		void	disconnect() override;

		int		getLD_List(Core::IED_Tree &t_tree) override;
		int		getDO_List(Core::ptrLN t_node) override;
		int		updateDO_List(Core::ptrLN t_node) override;

		int		getFS_List(Core::DirOn &t_dir) override;
		int 	removeFile(const QString &t_filename) override;

		void 	downloadFile(const QString &t_filename) override;
	};
	typedef QSharedPointer< Lib61850 >	ptrIED_Adapter;
}