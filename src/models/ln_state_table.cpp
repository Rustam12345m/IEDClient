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

#include "ln_state_table.hpp"

namespace
{
    // remove: .stVal and .f
    QString     removeSomeParts(const QString &t_name)
    {
        if (t_name.endsWith(".stVal")) {
            return t_name.first(t_name.size() - 6);
        } else if (t_name.endsWith(".f")) {
            return t_name.first(t_name.size() - 2);
        }/* else if (t_name.endsWith(".mag.f")) {
            return t_name.first(t_name.size() - 6);
        }
        */
        return t_name;
    }
}

namespace App::Models
{
    LN_SignalTable::LN_SignalTable(QObject *t_parent, Core::ptrIED t_ied)
        : QAbstractTableModel(t_parent), m_ied(t_ied)
    {
    }

    void LN_SignalTable::setActiveIED(Core::ptrIED t_ied)
    {
        beginResetModel();
        m_ied = t_ied;
        m_lnode.reset();
        endResetModel();
    }

    QVariant LN_SignalTable::headerData(int t_column, Qt::Orientation t_orientation, int t_role) const
    {
        if (t_orientation != Qt::Horizontal) {
            return QVariant();
        }

        switch (t_column) {
        case DO_NAME_COLUMN: {
            return QVariant::fromValue(SortHeaderValue("Name", true));
        }
        case DO_FC_COLUMN: {
            return QVariant::fromValue(SortHeaderValue("FC", true));
        }
        case DO_VALUE_COLUMN: {
            return QVariant::fromValue(SortHeaderValue("Value", true));
        }
        case DO_QUALITY_COLUMN: {
            return QVariant::fromValue(SortHeaderValue("Quality", true));
        }
        case DO_TS_COLUMN: {
            return QVariant::fromValue(SortHeaderValue("Timestamp", true));
        }
        case DO_DESC_COLUMN: {
            return QVariant::fromValue(SortHeaderValue("Description", false));
        }
        }
        return QVariant("");
    }

    QHash<int, QByteArray> LN_SignalTable::roleNames() const
    {
        return { { Qt::DisplayRole, "display" }, { Qt::UserRole + 1, "sort_value" } };
    }

    int LN_SignalTable::rowCount(const QModelIndex &t_parent) const
    {
        if (m_lnode) {
            return m_lnode->getSignalMatrix()->size();
        }
        return 0;
    }

    int LN_SignalTable::columnCount(const QModelIndex &t_parent) const
    {
        return 6;
    }

    QVariant LN_SignalTable::data(const QModelIndex &t_index, int t_role) const
    {
        //qDebug() << "LN_SignalTable: " << QString("index = %1 %2, role = %3").arg(t_index.row()).arg(t_index.column()).arg(t_role);
        int row = t_index.row(), column = t_index.column();

        if (m_lnode) {
            auto doTable = m_lnode->getSignalMatrix();
            if (t_role == ComRoles::ROLE_SORT_VALUE) {
                // for sorting process
                switch (column) {
                case DO_NAME_COLUMN: {
                    return QVariant(doTable->name(row));
                }
                case DO_FC_COLUMN: {
                    return QVariant(doTable->fc(row));
                }
                case DO_VALUE_COLUMN: {
                    return QVariant(doTable->value(row));
                }
                case DO_QUALITY_COLUMN: {
                    return QVariant(doTable->quality(row));
                }
                case DO_TS_COLUMN: {
                    return QVariant(doTable->timestamp(row));
                }
                case DO_DESC_COLUMN: {
                    return QVariant(doTable->description(row));
                }
                }
            } else {
                // for user interface
                switch (column) {
                case DO_NAME_COLUMN: {
                    return QVariant(removeSomeParts(doTable->name(row)));
                }
                case DO_FC_COLUMN: {
                    return QVariant(doTable->fc(row));
                }
                case DO_VALUE_COLUMN: {
                    return QVariant(doTable->value(row));
                }
                case DO_QUALITY_COLUMN: {
                    return QVariant(doTable->quality(row));
                }
                case DO_TS_COLUMN: {
                    return QVariant(doTable->timestamp(row));
                }
                case DO_DESC_COLUMN: {
                    return QVariant(doTable->description(row));
                }
                }
            }
        }
        return QVariant(" ? ");
    }

    void LN_SignalTable::slotDataUpdated(QSharedPointer<QList<Core::ModelItem*>> t_nodes)
    {
        if (t_nodes->empty()) {
            return;
        }

        Core::ModelItem *doItem = t_nodes->front();
        qDebug() << "LN_SignalTable: slotDataUpdated, do =" << doItem->getName();

        auto matrix = m_lnode->getSignalMatrix()->getRows();
        for (size_t i=0;i<matrix.size();i++) {
            if (matrix[i].base().get() == doItem) {
                emit dataChanged(index(i, DO_VALUE_COLUMN), index(i, DO_TS_COLUMN));
            }
        }
        // emit dataChanged(index(0, DO_NAME_COLUMN), index(rowCount() - 1, DO_DESC_COLUMN));
    }

    void LN_SignalTable::slotLNSelected(int t_ld, int t_ln)
    {
        // qDebug() << "LN_SignalTable: ld = " << t_ld << " ln = " << t_ln;
        Core::ptrLN ln = m_ied->model().getLogicalNode(t_ld, t_ln);
        if (ln != m_lnode) {
            if (m_lnode) {
                disconnect(m_updConnection);
            }

            beginResetModel();
            m_lnode = ln;
            if (m_lnode) {
                m_updConnection = connect(m_lnode.get(), &Core::LogicalNode::sigDataObjectUpdated,
                                        this, &LN_SignalTable::slotDataUpdated);
            }
            endResetModel();
        }
    }
}