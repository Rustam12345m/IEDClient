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

#include "goose_overview_table.hpp"

namespace App::Models
{
    GOOSE_OverviewTable::GOOSE_OverviewTable(QObject *t_parent, Core::IED::ptr t_ied)
        : QAbstractTableModel(t_parent), m_ied(t_ied)
    {
    }

    void GOOSE_OverviewTable::setSelectedGOOSE(int t_row)
    {
        m_selectedRow = t_row;
    }

    QString GOOSE_OverviewTable::selectedGoId() const
    {
        const auto list = m_ied->model().getGO_CBList();
        if (m_selectedRow < 0 || m_selectedRow >= list.count()) return {};
        const auto &gocb = list[m_selectedRow];
        return gocb->goId().isEmpty() ? gocb->getName() : gocb->goId();
    }

    QString GOOSE_OverviewTable::selectedDatSet() const
    {
        const auto list = m_ied->model().getGO_CBList();
        if (m_selectedRow < 0 || m_selectedRow >= list.count()) return {};
        return list[m_selectedRow]->datSet();
    }

    bool GOOSE_OverviewTable::selectedGoEna() const
    {
        const auto list = m_ied->model().getGO_CBList();
        if (m_selectedRow < 0 || m_selectedRow >= list.count()) return false;
        return list[m_selectedRow]->goEna();
    }

    uint32_t GOOSE_OverviewTable::selectedConfRev() const
    {
        const auto list = m_ied->model().getGO_CBList();
        if (m_selectedRow < 0 || m_selectedRow >= list.count()) return 0;
        return list[m_selectedRow]->confRev();
    }

    uint32_t GOOSE_OverviewTable::selectedMinTime() const
    {
        const auto list = m_ied->model().getGO_CBList();
        if (m_selectedRow < 0 || m_selectedRow >= list.count()) return 0;
        return list[m_selectedRow]->minTime();
    }

    uint32_t GOOSE_OverviewTable::selectedMaxTime() const
    {
        const auto list = m_ied->model().getGO_CBList();
        if (m_selectedRow < 0 || m_selectedRow >= list.count()) return 0;
        return list[m_selectedRow]->maxTime();
    }

    uint16_t GOOSE_OverviewTable::selectedAppId() const
    {
        const auto list = m_ied->model().getGO_CBList();
        if (m_selectedRow < 0 || m_selectedRow >= list.count()) return 0;
        return list[m_selectedRow]->appId();
    }

    uint16_t GOOSE_OverviewTable::selectedVlanId() const
    {
        const auto list = m_ied->model().getGO_CBList();
        if (m_selectedRow < 0 || m_selectedRow >= list.count()) return 0;
        return list[m_selectedRow]->vlanId();
    }

    uint8_t GOOSE_OverviewTable::selectedVlanPriority() const
    {
        const auto list = m_ied->model().getGO_CBList();
        if (m_selectedRow < 0 || m_selectedRow >= list.count()) return 0;
        return list[m_selectedRow]->vlanPriority();
    }

    void GOOSE_OverviewTable::setActiveIED(Core::IED::ptr t_ied)
    {
        beginResetModel();
        m_ied = t_ied;
        endResetModel();
    }

    QVariant GOOSE_OverviewTable::headerData(int t_column, Qt::Orientation t_orientation, int t_role) const
    {
        if (t_orientation != Qt::Horizontal) return QVariant();

        switch (t_column) {
        case GOOSE_ENA_COLUMN:    return QVariant(QString("Enabled"));
        case GOOSE_ID_COLUMN:     return QVariant(QString("GOOSE ID"));
        case GOOSE_DS_COLUMN:     return QVariant(QString("DataSet"));
        case GOOSE_CREV_COLUMN:   return QVariant(QString("ConfRev"));
        case GOOSE_MINTIME_COLUMN:return QVariant(QString("Min Time (ms)"));
        case GOOSE_MAXTIME_COLUMN:return QVariant(QString("Max Time (ms)"));
        case GOOSE_APPID_COLUMN:  return QVariant(QString("AppId"));
        case GOOSE_VLANID_COLUMN: return QVariant(QString("VLAN Id"));
        case GOOSE_VLANPRI_COLUMN:return QVariant(QString("VLAN Priority"));
        }
        return QVariant();
    }

    QHash<int, QByteArray> GOOSE_OverviewTable::roleNames() const
    {
        return { { Qt::DisplayRole, "display" } };
    }

    int GOOSE_OverviewTable::rowCount(const QModelIndex &t_parent) const
    {
        if (!m_ied) return 0;
        return m_ied->model().getGO_CBList().count();
    }

    int GOOSE_OverviewTable::columnCount(const QModelIndex &t_parent) const
    {
        return COLUMN_COUNT;
    }

    QVariant GOOSE_OverviewTable::data(const QModelIndex &t_index, int t_role) const
    {
        if (t_role != Qt::DisplayRole) return QVariant();

        int row = t_index.row(), column = t_index.column();
        const auto list = m_ied->model().getGO_CBList();
        if (row < 0 || row >= list.count()) return QVariant();

        const auto &gocb = list[row];

        switch (column) {
        case GOOSE_ENA_COLUMN:    return QVariant(gocb->goEna() ? "Yes" : "No");
        case GOOSE_ID_COLUMN:     return QVariant(gocb->goId().isEmpty() ? gocb->getName() : gocb->goId());
        case GOOSE_DS_COLUMN:     return QVariant(gocb->datSet());
        case GOOSE_CREV_COLUMN:   return QVariant(QString::number(gocb->confRev()));
        case GOOSE_MINTIME_COLUMN:return QVariant(QString::number(gocb->minTime()));
        case GOOSE_MAXTIME_COLUMN:return QVariant(QString::number(gocb->maxTime()));
        case GOOSE_APPID_COLUMN:  return QVariant(QString("0x%1").arg(gocb->appId(), 4, 16, QChar('0')).toUpper());
        case GOOSE_VLANID_COLUMN: return QVariant(QString::number(gocb->vlanId()));
        case GOOSE_VLANPRI_COLUMN:return QVariant(QString::number(gocb->vlanPriority()));
        }
        return QVariant();
    }

    void GOOSE_OverviewTable::slotDataUpdated(bool t_done)
    {
        beginResetModel();
        endResetModel();
    }
}
