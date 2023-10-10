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

#include "backend_base.hpp"

#include "models/sort_proxy_model.hpp"
#include "models/ld_listmodel.hpp"
#include "models/ln_tablemodel.hpp"
#include "models/do_tablemodel.hpp"

namespace App
{
	/*
	* Presenter for LD, LN, DO table in QML
	* */
	class LD_Backend : public BackendBase
	{
		Q_OBJECT
	public:
		LD_Backend(ConnectionObject &t_con);
		~LD_Backend() = default;

		Q_PROPERTY(LD_ListModel* 		ldModel 		READ getLD_Model		CONSTANT)
		Q_PROPERTY(LN_TableModel* 		lnModel 		READ getLN_Model 		CONSTANT)
		Q_PROPERTY(QAbstractItemModel* 	doModel 		READ getSortDO_Model 	CONSTANT)
		//Q_PROPERTY(DO_TableModel* 	doModel 		READ getDO_Model 		CONSTANT)
		LD_ListModel*		getLD_Model() const { return m_ldModel; }
		LN_TableModel*		getLN_Model() const { return m_lnModel; }
		DO_TableModel*		getDO_Model() const { return m_doModel; }
		QAbstractItemModel* getSortDO_Model() const { return m_sortDOModel; }

		// Active LD & LN in models
		Q_PROPERTY(int 	currentLD 	READ getCurrentLD 	WRITE setCurrentLD 	NOTIFY sigCurrentLD)
		int 	getCurrentLD() const { return m_currentLD; }
		void 	setCurrentLD(int t_inx) {
			m_currentLD = t_inx;
			//m_lnModel.setCurrentLD(t_inx);
			m_doModel->setCurrentLD(t_inx);
		}

		Q_PROPERTY(int 	currentLN 	READ getCurrentLN 	WRITE setCurrentLN 	NOTIFY sigCurrentLN)
		int 	getCurrentLN() const { return m_currentLN; }
		void 	setCurrentLN(int t_inx) {
			m_currentLN = t_inx;
			m_doModel->setCurrentLN(t_inx);
		}


		// LD & LN
		Q_INVOKABLE QString getLN_TextStatus();
		Q_INVOKABLE void updateLNodeData(int t_ldIndex, int t_lnIndex);

		void 	slotNewIED() override;

	signals:
		void 	sigCurrentLD(int t_inx);
		void 	sigCurrentLN(int t_inx);

	protected:
		// Models for Tables in GUI
		LD_ListModel*		m_ldModel = nullptr;
		LN_TableModel*		m_lnModel = nullptr;
		DO_TableModel*		m_doModel = nullptr;
		SortProxyModel* 	m_sortDOModel = nullptr;

		// Active selected by User
		int 				m_currentLD = -1;
		int 				m_currentLN = -1;
	};
}