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

#include "data_model.hpp"
#include "fs_model.hpp"

namespace Core
{
	class IED_Object : public QObject
	{
		Q_OBJECT
	public:
		IED_Object(QObject *t_parent=nullptr);
		~IED_Object() = default;

		Core::DataModel&	model() { return *m_model; }
		Core::FS_Model&		fs() { return *m_fsModel; }

		void setModel(QSharedPointer<DataModel> t_model) {
			m_model = t_model;
		}
		void setFSModel(QSharedPointer<FS_Model> t_model) {
			m_fsModel = t_model;
		}

	signals:

	private:
		QSharedPointer<Core::DataModel>		m_model;
		QSharedPointer<Core::FS_Model>		m_fsModel;
	};
	typedef QSharedPointer<Core::IED_Object>	ptrIED_Object;
}