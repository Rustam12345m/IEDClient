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

#include "cmd_interface.hpp"
#include "core/ied.hpp"

namespace Cmd
{
	/*
	 * This CMD is needed to update status signals of LN0 for all LDs
	 * */
	class UpdateLDs_StatusCmd : public CmdInterface
	{
		Q_OBJECT
	public:
		UpdateLDs_StatusCmd(Core::ptrIED t_ied) : m_ied(t_ied)
		{
		}
		~UpdateLDs_StatusCmd() override {}

		void	execute(Cmd::Interface::ptrIEC61850_API t_api) override;

		static auto create(Core::ptrIED t_ied) {
			return QSharedPointer<UpdateLDs_StatusCmd>::create(t_ied);
		}
    
    signals:
		void 	sigModelValues(Core::ptrModelStateUpd t_vals);

	private slots:
		void 	slotMsgProgress(const QString &t_msg);

	private:
		Core::ptrIED	m_ied;
	};
}