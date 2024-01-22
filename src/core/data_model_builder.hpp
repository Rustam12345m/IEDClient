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

#include "core/data_model.hpp"
#include "core/fs_model.hpp"

namespace Core
{
	class DataModelBuilder
	{
	public:
		DataModelBuilder();
		~DataModelBuilder();

		DataModelBuilder& 	createLD(const QString &t_name);
		DataModelBuilder& 	createLN(const QString &t_name);
		DataModelBuilder& 	createDO(const QString &t_name);
		DataModelBuilder& 	createDA(const QString &t_name, const QString &t_fc, int t_fcNum);
		DataModelBuilder& 	createSDA(QSharedPointer<Item> t_parent, const QString &t_name);

		DataModelBuilder& 	createDataSet(const QString &t_name, const QString &t_lnRef, bool t_del);
		DataModelBuilder& 	createDataSet_Elem(const QString &t_ref, const QString &t_fc);
		DataModelBuilder& 	createRCB(const QString &t_name);
		DataModelBuilder& 	createGOCB(const QString &t_name);
		DataModelBuilder& 	createSVCB(const QString &t_name);

		auto 	lastLD() const { return m_lastLD; }
		auto 	lastLN() const { return m_lastLN; }
		auto 	lastDO() const { return m_lastDO; }
		auto 	lastDA() const { return m_lastDA; }
		auto 	lastSDA() const { return m_lastSDA; }

		auto 	lastDataSet() const { return m_lastDataSet; }
		auto 	lastRCB() const { return m_lastRCB; }
		auto 	lastGOCB() const { return m_lastGOCB; }
		auto 	lastSVCB() const { return m_lastSVCB; }

		ptrDataModel	build(QThread *t_guiThread);

	private:
		ptrDataModel	m_model;
		ptrLD 			m_lastLD;
		ptrLN 			m_lastLN;
		ptrDO 			m_lastDO;
		ptrDA 			m_lastDA;
		ptrSDA 			m_lastSDA;
		//
		ptrDataSet		m_lastDataSet;
		ptrRCB			m_lastRCB;
		ptrGOCB			m_lastGOCB;
		ptrSVCB			m_lastSVCB;
	};
}