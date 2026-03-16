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

#include "rcb_overview_table.hpp"

namespace App::Models
{
    static QString formatTrgOps(int t_trgOps)
    {
        QStringList parts;
        if (t_trgOps & 2)  parts << "dchg";
        if (t_trgOps & 4)  parts << "qchg";
        if (t_trgOps & 8)  parts << "dupd";
        if (t_trgOps & 16) parts << "intg";
        if (t_trgOps & 32) parts << "gi";
        return parts.isEmpty() ? "none" : parts.join("|");
    }

    RCB_OverviewTable::RCB_OverviewTable(QObject *t_parent, Core::IED::ptr t_ied, bool t_buffered)
        : QAbstractTableModel(t_parent), m_ied(t_ied), m_buffered(t_buffered)
    {
    }

    QList<Core::ReportBlock::ptr> RCB_OverviewTable::filteredList() const
    {
        QList<Core::ReportBlock::ptr> result;
        for (const auto &rcb : m_ied->model().getReportCBList()) {
            if (rcb->isBuffered() == m_buffered)
                result.append(rcb);
        }
        return result;
    }

    Core::ReportBlock::ptr RCB_OverviewTable::selectedRCB() const
    {
        const auto list = filteredList();
        if (m_currentRCB >= 0 && m_currentRCB < list.count())
            return list[m_currentRCB];
        return nullptr;
    }

    void RCB_OverviewTable::setSelectedRCB(int t_inx)
    {
        m_currentRCB = t_inx;
        emit sigRCBSelected(m_currentRCB);
    }

    QString RCB_OverviewTable::selectedRptId() const
    {
        auto rcb = selectedRCB();
        return rcb ? (rcb->rptId().isEmpty() ? rcb->getName() : rcb->rptId()) : QString();
    }

    QString RCB_OverviewTable::selectedDsRef() const
    {
        auto rcb = selectedRCB();
        return rcb ? rcb->dsRef() : QString();
    }

    int RCB_OverviewTable::selectedTrgOps() const
    {
        auto rcb = selectedRCB();
        return rcb ? rcb->trgOps() : 0;
    }

    bool RCB_OverviewTable::selectedRptEna() const
    {
        auto rcb = selectedRCB();
        return rcb ? rcb->rptEna() : false;
    }

    bool RCB_OverviewTable::selectedResv() const
    {
        auto rcb = selectedRCB();
        return rcb ? rcb->resv() : false;
    }

    uint RCB_OverviewTable::selectedConfRev() const
    {
        auto rcb = selectedRCB();
        return rcb ? rcb->confRev() : 0;
    }

    uint RCB_OverviewTable::selectedBufTm() const
    {
        auto rcb = selectedRCB();
        return rcb ? rcb->bufTm() : 0;
    }

    uint RCB_OverviewTable::selectedIntgPd() const
    {
        auto rcb = selectedRCB();
        return rcb ? rcb->intgPd() : 0;
    }

    void RCB_OverviewTable::setActiveIED(Core::IED::ptr t_ied)
    {
        beginResetModel();
        m_ied = t_ied;
        endResetModel();
    }

    QVariant RCB_OverviewTable::headerData(int t_column, Qt::Orientation t_orientation, int t_role) const
    {
        if (t_orientation != Qt::Horizontal) {
            return QVariant();
        }

        switch (t_column) {
        case RCB_ENA_COLUMN: {
            return QVariant(QString("Enabled"));
        }
        case RCB_RESV_COLUMN: {
            return QVariant(QString("Reserved"));
        }
        case RCB_ID_COLUMN: {
            return QVariant(QString("Report ID"));
        }
        case RCB_OWNER_COLUMN: {
            return QVariant(QString("Owner"));
        }
        case RCB_DS_COLUMN: {
            return QVariant(QString("DataSet"));
        }
        case RCB_TRIG_COLUMN: {
            return QVariant(QString("Triggers"));
        }
        case RCB_CREV_COLUMN: {
            return QVariant(QString("ConfRev"));
        }
        case RCB_BUFF_COLUMN: {
            return QVariant(QString("Buffer (ms)"));
        }
        case RCB_INTEGRITY_COLUMN: {
            return QVariant(QString("Integrity (ms)"));
        }
        }
        return QVariant("");
    }

    QHash<int, QByteArray> RCB_OverviewTable::roleNames() const
    {
        return { { Qt::DisplayRole, "display" } };
    }

    int RCB_OverviewTable::rowCount(const QModelIndex &t_parent) const
    {
        if (!m_ied) return 0;
        return filteredList().count();
    }

    int RCB_OverviewTable::columnCount(const QModelIndex &t_parent) const
    {
        return COLUMN_COUNT;
    }

    QVariant RCB_OverviewTable::data(const QModelIndex &t_index, int t_role) const
    {
        if (t_role != Qt::DisplayRole) return QVariant();

        int row = t_index.row(), column = t_index.column();
        const auto list = filteredList();
        if (row < 0 || row >= list.count()) return QVariant();

        const auto &rcb = list[row];

        switch (column) {
        case RCB_ENA_COLUMN:
            return QVariant(rcb->rptEna() ? "Yes" : "No");
        case RCB_RESV_COLUMN:
            return QVariant(rcb->resv() ? "Yes" : "No");
        case RCB_ID_COLUMN:
            return QVariant(rcb->rptId().isEmpty() ? rcb->getName() : rcb->rptId());
        case RCB_OWNER_COLUMN:
            return QVariant(rcb->owner());
        case RCB_DS_COLUMN:
            return QVariant(rcb->dsRef());
        case RCB_TRIG_COLUMN:
            return QVariant(formatTrgOps(rcb->trgOps()));
        case RCB_CREV_COLUMN:
            return QVariant(QString::number(rcb->confRev()));
        case RCB_BUFF_COLUMN:
            return QVariant(QString::number(rcb->bufTm()));
        case RCB_INTEGRITY_COLUMN:
            return QVariant(QString::number(rcb->intgPd()));
        }
        return QVariant();
    }

    void RCB_OverviewTable::slotDataUpdated(bool t_done)
    {
        beginResetModel();
        endResetModel();
    }
}
