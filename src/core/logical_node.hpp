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

#include "data_object.hpp"

#include "dataset.hpp"
#include "report_control_block.hpp"
#include "goose_control_block.hpp"
#include "sv_control_block.hpp"

#include "ln_signal_matrix.hpp"

namespace Core
{
	/*
	 * Representation of Logical Node
	 * As a QObject this Item should live in GUI's thread
	 * */
	class LogicalNode : public QObject, public ModelItem
	{
		Q_OBJECT
	public:
		LogicalNode(ModelItem *t_parent, const QString &t_name)
            : QObject(nullptr), ModelItem(t_parent, t_name)
		{
			m_delimetr = "/"; // Between LDName and LNName
		}

		ptrDO	getModItem() const { return m_mod; }
		ptrDO	getBehItem() const { return m_beh; }
		ptrDO	getHealthItem() const { return m_health; }

		void	addSubItem(QSharedPointer< ModelItem > t_child) override {
			if (t_child->getName() == "Mod") {
				m_mod = t_child.dynamicCast<DataObject>();
			} else if (t_child->getName() == "Beh") {
				m_beh = t_child.dynamicCast<DataObject>();
			} else if (t_child->getName() == "Health") {
				m_health = t_child.dynamicCast<DataObject>();
			}
			ModelItem::addSubItem(t_child);
		}
		void 	addSubItem(QSharedPointer< DataSet > t_ds) {
			m_dataSets.push_back(t_ds);
		}
		void 	addSubItem(QSharedPointer< ReportBlock > t_rcb) {
			m_rcbs.push_back(t_rcb);
		}
		void 	addSubItem(QSharedPointer< GooseControlBlock > t_gocb) {
			m_gooses.push_back(t_gocb);
		}

		auto& 	getSignalMatrix() const {
			return m_sigMatrix;
		}
		auto& 	getDataSets() const {
			return m_dataSets;
		}
		auto& 	getReportBlocks() const {
			return m_rcbs;
		}
		auto& 	getGooseCB() const {
			return m_gooses;
		}

	signals:
		void 	sigDataObjectUpdated(QSharedPointer<QList<ModelItem*>> t_nodes);

	protected:
	 	void 	notifyFromChild(QSharedPointer<QList<ModelItem*>> t_nodes) override;

	protected:
		QSharedPointer<DataObject>		m_mod;
		QSharedPointer<DataObject>		m_beh;
		QSharedPointer<DataObject>		m_health;
		QList< ptrDataSet >				m_dataSets;
		QList< ptrRCB >					m_rcbs;
		QList< ptrGOCB >				m_gooses;
		QSharedPointer<LN_SignalMatrix>	m_sigMatrix;

	friend class DataModelBuilder;
	};

	typedef QSharedPointer< LogicalNode >	ptrLN;
}