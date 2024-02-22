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
	 * 
	 * */
	class UpdateLNode_Cmd : public CmdInterface
	{
		Q_OBJECT
	public:
		UpdateLNode_Cmd(Core::ptrIED t_ied, Core::ptrLN t_lnode)
			: m_ied{t_ied}, m_lnode{t_lnode}
		{
		}

		void 	execute(Cmd::Interface::ptrIEC61850_API t_api) override;

		static auto create(Core::ptrIED t_ied, Core::ptrLN t_lnode) {
			return QSharedPointer<UpdateLNode_Cmd>::create(t_ied, t_lnode);
		}

	signals:
		void 	sigNewValues(Core::ptrValuesUpdater t_vals);

	private:
		Core::ptrIED    m_ied;
		Core::ptrLN     m_lnode;
	};
}