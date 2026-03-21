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

#include "sv_overview_table.hpp"

namespace App::Models
{
    SV_OverviewTable::SV_OverviewTable(QObject *parent, Core::IED::ptr ied)
        : QAbstractTableModel(parent), m_ied(ied)
    {
    }

    void SV_OverviewTable::setSelectedSV(int row)
    {
        m_selectedRow = row;
    }

    QString SV_OverviewTable::selectedSvId() const
    {
        const auto list = m_ied->model().getSV_CBList();
        if (m_selectedRow < 0 || m_selectedRow >= list.count()) return {};
        const auto &svcb = list[m_selectedRow];
        return svcb->svId().isEmpty() ? svcb->getName() : svcb->svId();
    }

    QString SV_OverviewTable::selectedDatSet() const
    {
        const auto list = m_ied->model().getSV_CBList();
        if (m_selectedRow < 0 || m_selectedRow >= list.count()) return {};
        return list[m_selectedRow]->datSet();
    }

    bool SV_OverviewTable::selectedSvEna() const
    {
        const auto list = m_ied->model().getSV_CBList();
        if (m_selectedRow < 0 || m_selectedRow >= list.count()) return false;
        return list[m_selectedRow]->svEna();
    }

    bool SV_OverviewTable::selectedIsMulticast() const
    {
        const auto list = m_ied->model().getSV_CBList();
        if (m_selectedRow < 0 || m_selectedRow >= list.count()) return false;
        return list[m_selectedRow]->isMulticast();
    }

    uint32_t SV_OverviewTable::selectedConfRev() const
    {
        const auto list = m_ied->model().getSV_CBList();
        if (m_selectedRow < 0 || m_selectedRow >= list.count()) return 0;
        return list[m_selectedRow]->confRev();
    }

    uint32_t SV_OverviewTable::selectedSmpRate() const
    {
        const auto list = m_ied->model().getSV_CBList();
        if (m_selectedRow < 0 || m_selectedRow >= list.count()) return 0;
        return list[m_selectedRow]->smpRate();
    }

    int SV_OverviewTable::selectedNoASDU() const
    {
        const auto list = m_ied->model().getSV_CBList();
        if (m_selectedRow < 0 || m_selectedRow >= list.count()) return 0;
        return list[m_selectedRow]->noASDU();
    }

    void SV_OverviewTable::setActiveIED(Core::IED::ptr ied)
    {
        beginResetModel();
        m_ied = ied;
        endResetModel();
    }

    QVariant SV_OverviewTable::headerData(int column, Qt::Orientation orientation, int role) const
    {
        if (orientation != Qt::Horizontal) return QVariant();

        switch (column) {
        case SV_ENA_COLUMN:    return QVariant(QString("Enabled"));
        case SV_TYPE_COLUMN:   return QVariant(QString("Type"));
        case SV_ID_COLUMN:     return QVariant(QString("SV ID"));
        case SV_DS_COLUMN:     return QVariant(QString("DataSet"));
        case SV_CREV_COLUMN:   return QVariant(QString("ConfRev"));
        case SV_SMPRATE_COLUMN:return QVariant(QString("Smp Rate"));
        case SV_NOASDU_COLUMN: return QVariant(QString("NoASDU"));
        }
        return QVariant();
    }

    QHash<int, QByteArray> SV_OverviewTable::roleNames() const
    {
        return { { Qt::DisplayRole, "display" } };
    }

    int SV_OverviewTable::rowCount(const QModelIndex &parent) const
    {
        if (!m_ied) return 0;
        return m_ied->model().getSV_CBList().count();
    }

    int SV_OverviewTable::columnCount(const QModelIndex &parent) const
    {
        return COLUMN_COUNT;
    }

    QVariant SV_OverviewTable::data(const QModelIndex &index, int role) const
    {
        if (role != Qt::DisplayRole) return QVariant();

        int row = index.row(), column = index.column();
        const auto list = m_ied->model().getSV_CBList();
        if (row < 0 || row >= list.count()) return QVariant();

        const auto &svcb = list[row];

        switch (column) {
        case SV_ENA_COLUMN:    return QVariant(svcb->svEna() ? "Yes" : "No");
        case SV_TYPE_COLUMN:   return QVariant(svcb->isMulticast() ? "MSVCB" : "USVCB");
        case SV_ID_COLUMN:     return QVariant(svcb->svId().isEmpty() ? svcb->getName() : svcb->svId());
        case SV_DS_COLUMN:     return QVariant(svcb->datSet());
        case SV_CREV_COLUMN:   return QVariant(QString::number(svcb->confRev()));
        case SV_SMPRATE_COLUMN:return QVariant(QString::number(svcb->smpRate()));
        case SV_NOASDU_COLUMN: return QVariant(QString::number(svcb->noASDU()));
        }
        return QVariant();
    }

    void SV_OverviewTable::slotDataUpdated(bool done)
    {
        beginResetModel();
        endResetModel();
    }
}
