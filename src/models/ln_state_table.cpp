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
    QString     removeSomeParts(const QString &name)
    {
        if (name.endsWith(".stVal")) {
            return name.first(name.size() - 6);
        } else if (name.endsWith(".f")) {
            return name.first(name.size() - 2);
        }/* else if (name.endsWith(".mag.f")) {
            return name.first(name.size() - 6);
        }
        */
        return name;
    }
}

namespace App::Models
{
    LN_SignalTable::LN_SignalTable(QObject *parent, Core::IED::ptr ied, MatrixType type)
        : QAbstractTableModel(parent), m_ied(ied), m_type(type)
    {
    }

    Core::LN_SignalMatrix::ptr LN_SignalTable::getMatrix() const
    {
        if (!m_lnode) return nullptr;
        switch (m_type) {
        case MatrixType::Controls: return m_lnode->getControlsMatrix();
        case MatrixType::Settings: return m_lnode->getSettingsMatrix();
        default:                   return m_lnode->getSignalMatrix();
        }
    }

    void LN_SignalTable::setActiveIED(Core::IED::ptr ied)
    {
        beginResetModel();
        m_ied = ied;
        m_lnode.reset();
        endResetModel();
    }

    static QString ctlModelToString(const QString &val)
    {
        if (val == "0") return "Status";
        if (val == "1") return "Direct";
        if (val == "2") return "SBO";
        if (val == "3") return "Dir+Enh";
        if (val == "4") return "SBO+Enh";
        return val.isEmpty() ? " - " : val;
    }

    QVariant LN_SignalTable::headerData(int column, Qt::Orientation orientation, int role) const
    {
        if (orientation != Qt::Horizontal) {
            return QVariant();
        }

        if (m_type == MatrixType::Controls) {
            switch (column) {
            case CO_NAME:  return QVariant::fromValue(SortHeaderValue("Name", true));
            case CO_TYPE:  return QVariant::fromValue(SortHeaderValue("Type", true));
            case CO_VALUE: return QVariant::fromValue(SortHeaderValue("Value", true));
            case CO_MODEL: return QVariant::fromValue(SortHeaderValue("Model", true));
            case CO_SELD:  return QVariant::fromValue(SortHeaderValue("Seld", true));
            case CO_OPOK:  return QVariant::fromValue(SortHeaderValue("opOk", true));
            case CO_DESC:  return QVariant::fromValue(SortHeaderValue("Description", false));
            }
            return QVariant("");
        }

        if (m_type == MatrixType::Settings) {
            switch (column) {
            case SET_NAME:  return QVariant::fromValue(SortHeaderValue("Name", true));
            case SET_FC:    return QVariant::fromValue(SortHeaderValue("FC", true));
            case SET_VALUE: return QVariant::fromValue(SortHeaderValue("Value", true));
            case SET_DESC:  return QVariant::fromValue(SortHeaderValue("Description", false));
            }
            return QVariant("");
        }

        switch (column) {
        case DO_NAME_COLUMN:    return QVariant::fromValue(SortHeaderValue("Name", true));
        case DO_FC_COLUMN:      return QVariant::fromValue(SortHeaderValue("FC", true));
        case DO_VALUE_COLUMN:   return QVariant::fromValue(SortHeaderValue("Value", true));
        case DO_QUALITY_COLUMN: return QVariant::fromValue(SortHeaderValue("Quality", true));
        case DO_TS_COLUMN:      return QVariant::fromValue(SortHeaderValue("Timestamp", true));
        case DO_DESC_COLUMN:    return QVariant::fromValue(SortHeaderValue("Description", false));
        }
        return QVariant("");
    }

    QHash<int, QByteArray> LN_SignalTable::roleNames() const
    {
        return { { Qt::DisplayRole, "display" }, { Qt::UserRole + 1, "sort_value" } };
    }

    int LN_SignalTable::rowCount(const QModelIndex &parent) const
    {
        auto matrix = getMatrix();
        return matrix ? matrix->size() : 0;
    }

    int LN_SignalTable::columnCount(const QModelIndex &parent) const
    {
        if (m_type == MatrixType::Controls) return int(CO_COLUMN_COUNT);
        if (m_type == MatrixType::Settings) return int(SET_COLUMN_COUNT);
        return int(COLUMN_COUNT);
    }

    static inline QString orDash(const QString &v)
    {
        return v.isEmpty() ? QStringLiteral(" - ") : v;
    }

    QVariant LN_SignalTable::data(const QModelIndex &index, int role) const
    {
        int row = index.row(), col = index.column();

        auto matrix = getMatrix();
        if (!matrix) return QVariant(" ? ");

        // Controls table — different column layout
        if (m_type == MatrixType::Controls) {
            switch (col) {
            case CO_NAME:  return QVariant(matrix->name(row));
            case CO_TYPE:  return QVariant(orDash(matrix->ctlType(row)));
            case CO_VALUE: return QVariant(orDash(matrix->value(row)));
            case CO_MODEL: return QVariant(ctlModelToString(matrix->ctlModel(row)));
            case CO_SELD:  return QVariant(orDash(matrix->stSeld(row)));
            case CO_OPOK:  return QVariant(orDash(matrix->opOk(row)));
            case CO_DESC:  return QVariant(orDash(matrix->description(row)));
            }
            return QVariant(" ? ");
        }

        // Settings table — 4-column layout (no Quality/Timestamp)
        if (m_type == MatrixType::Settings) {
            if (role == ComRoles::ROLE_SORT_VALUE) {
                switch (col) {
                case SET_NAME:  return QVariant(matrix->name(row));
                case SET_FC:    return QVariant(matrix->fc(row));
                case SET_VALUE: return QVariant(orDash(matrix->value(row)));
                case SET_DESC:  return QVariant(matrix->description(row));
                }
            } else {
                switch (col) {
                case SET_NAME:  return QVariant(removeSomeParts(matrix->name(row)));
                case SET_FC:    return QVariant(matrix->fc(row));
                case SET_VALUE: return QVariant(orDash(matrix->value(row)));
                case SET_DESC:  return QVariant(matrix->description(row));
                }
            }
            return QVariant(" ? ");
        }

        // State table — 6-column layout
        if (role == ComRoles::ROLE_SORT_VALUE) {
            switch (col) {
            case DO_NAME_COLUMN:    return QVariant(matrix->name(row));
            case DO_FC_COLUMN:      return QVariant(matrix->fc(row));
            case DO_VALUE_COLUMN:   return QVariant(orDash(matrix->value(row)));
            case DO_QUALITY_COLUMN: return QVariant(matrix->quality(row));
            case DO_TS_COLUMN:      return QVariant(matrix->timestamp(row));
            case DO_DESC_COLUMN:    return QVariant(matrix->description(row));
            }
        } else {
            switch (col) {
            case DO_NAME_COLUMN:    return QVariant(removeSomeParts(matrix->name(row)));
            case DO_FC_COLUMN:      return QVariant(matrix->fc(row));
            case DO_VALUE_COLUMN:   return QVariant(orDash(matrix->value(row)));
            case DO_QUALITY_COLUMN: return QVariant(matrix->quality(row));
            case DO_TS_COLUMN:      return QVariant(matrix->timestamp(row));
            case DO_DESC_COLUMN:    return QVariant(matrix->description(row));
            }
        }
        return QVariant(" ? ");
    }

    void LN_SignalTable::slotDataUpdated(Core::ModelItem::ptrList nodes)
    {
        if (nodes->empty()) {
            return;
        }

        Core::ModelItem *doItem = nodes->front();

        auto matrixPtr = getMatrix();
        if (!matrixPtr) return;
        auto matrix = matrixPtr->getRows();
        int lastCol = (m_type == MatrixType::Controls) ? int(CO_COLUMN_COUNT - 1)
                    : (m_type == MatrixType::Settings) ? int(SET_VALUE)
                    : int(DO_TS_COLUMN);
        for (size_t i=0;i<matrix.size();i++) {
            if (matrix[i].base().get() == doItem) {
                emit dataChanged(index(i, 0), index(i, lastCol));
            }
        }
    }

    void LN_SignalTable::slotLNSelected(int ld, int ln)
    {
        Core::LogicalNode::ptr lnNode = m_ied->model().getLogicalNode(ld, ln);
        if (lnNode != m_lnode) {
            if (m_lnode) {
                disconnect(m_updConnection);
            }

            beginResetModel();
            m_lnode = lnNode;
            if (m_lnode) {
                m_updConnection = connect(m_lnode.get(), &Core::LogicalNode::sigDataObjectUpdated,
                                        this, &LN_SignalTable::slotDataUpdated);
            }
            endResetModel();
        }
    }
}