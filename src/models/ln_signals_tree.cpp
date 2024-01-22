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

#include "ln_signals_tree.hpp"

namespace App::Models
{
	LN_SignalTree::LN_SignalTree(QObject *t_parent, QSharedPointer<Core::IED_Object> t_ied)
		: QAbstractItemModel(t_parent), m_ied(t_ied)
	{
	}

	void LN_SignalTree::setNewIED(QSharedPointer<Core::IED_Object> t_ied)
	{
		beginResetModel();
		m_ied = t_ied;
		m_lnode.reset();
		endResetModel();
	}

	QVariant LN_SignalTree::headerData(int t_sect, Qt::Orientation t_orient, int t_role) const
	{
		switch (t_sect) {
		case NAME_COLUMN: {
			return QVariant("Name");
		}
		case FC_COLUMN: {
			return QVariant("FC");
		}
		case VALUE_COLUMN: {
			return QVariant("Value");
		}
		}
		return QVariant("");
	}

	int LN_SignalTree::rowCount(const QModelIndex &t_parent) const
	{
		if (t_parent.column() > 0) {
			return 0;
		}

		Core::Item *item = nullptr;
		if (!t_parent.isValid()) {
			item = m_lnode.get();
		} else {
			item = static_cast<Core::Item*>(t_parent.internalPointer());
		}

		if (item != nullptr) {
			return item->getItemCount();
		}
		return 0;
	}

	int LN_SignalTree::columnCount(const QModelIndex &t_parent) const
	{
		return COLUMN_COUNT;
	}

	QModelIndex LN_SignalTree::index(int t_row, int t_column, const QModelIndex &t_parent) const
	{
		if (!hasIndex(t_row, t_column, t_parent)) {
			return QModelIndex();
		}

		Core::Item *item = nullptr;
		if (!t_parent.isValid()) {
			item = m_lnode.get();
		} else {
			item = static_cast<Core::Item*>(t_parent.internalPointer());
		}

		if (item != nullptr) {
			auto node = item->getItem(t_row);
			if (node) {
				return createIndex(t_row, t_column, node.get());
			}
		}
		return QModelIndex();
	}

	QModelIndex LN_SignalTree::parent(const QModelIndex &t_index) const
	{
		if (!t_index.isValid()) {
			return QModelIndex();
		}

		Core::Item *parent = nullptr;
		Core::Item *item = static_cast<Core::Item*>(t_index.internalPointer());
		if (item != nullptr) {
			parent = item->getParent();
		}

		if (parent == m_lnode.get()) {
			return QModelIndex();
		}
		return createIndex(parent->getItemCount(), 0, parent);
	}

	QVariant LN_SignalTree::data(const QModelIndex &t_index, int t_role) const
	{
		if (!t_index.isValid() || (t_role != Qt::DisplayRole)) {
			return QVariant();
		}

		Core::Item *item = static_cast<Core::Item*>(t_index.internalPointer());
		if (item != nullptr) {
			switch (t_index.column()) {
			case NAME_COLUMN: {
				return QVariant(item->getName());
			}
			case FC_COLUMN: {
				auto *da = dynamic_cast<Core::DataAttribute*>(item);
				if (da) {
					return QVariant(da->fc());
				}
				return QVariant("");
			}
			case VALUE_COLUMN: {
				return QVariant(item->getValue());
			}
			}
		}
		return QVariant(" ? ");
	}

	void LN_SignalTree::slotDataUpdated(QSharedPointer<QList<Core::Item*>> t_nodes)
	{
		//qDebug() << "LN_SignalTree: slotDataUpdated";
		// emit dataChanged(index(0, 0), index(rowCount() - 1, VALUE_COLUMN));
	}

	void LN_SignalTree::slotLNSelected(int t_ld, int t_ln)
	{
		// qDebug() << "LN_SignalTree: ld = " << t_ld << " ln = " << t_ln;

		Core::ptrLN ln = m_ied->model().getLogicalNode(t_ld, t_ln);
		if (ln != m_lnode) {
			if (m_lnode) {
				disconnect(m_updConnection);
			}

			beginResetModel();
			m_lnode = ln;
			if (m_lnode) {
				m_updConnection = connect(m_lnode.get(), &Core::LogicalNode::sigDataObjectUpdated,
										this, &LN_SignalTree::slotDataUpdated);
			}
			endResetModel();
		}
	}
}