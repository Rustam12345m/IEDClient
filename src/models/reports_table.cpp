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

#include "reports_table.hpp"
#include "rcb_overview_table.hpp"

#include <QDateTime>

namespace App::Models
{
    ReportsTable::ReportsTable(QObject *t_parent, Core::IED::ptr t_ied)
        : QAbstractTableModel(t_parent), m_ied(t_ied)
    {
    }

    void ReportsTable::setActiveIED(Core::IED::ptr t_ied)
    {
        beginResetModel();
        m_ied = t_ied;
        switchStorage(nullptr);
        endResetModel();
    }

    QVariant ReportsTable::headerData(int t_section, Qt::Orientation t_orientation, int t_role) const
    {
        if (t_role != Qt::DisplayRole) {
            return QVariant();
        }

        switch (t_orientation) {
        case Qt::Horizontal: {
            const char* labels[] = { "#", "Timestamp", "Reason", "Values" };
            return QVariant(labels[t_section % COLUMN_COUNT]);
        }
        case Qt::Vertical:
            break;
        }
        return QVariant();
    }

    QHash<int, QByteArray> ReportsTable::roleNames() const
    {
        return { { Qt::DisplayRole, "display" } };
    }

    int ReportsTable::rowCount(const QModelIndex &t_parent) const
    {
        Q_UNUSED(t_parent);
        return m_storage ? m_storage->count() : 0;
    }

    int ReportsTable::columnCount(const QModelIndex &t_parent) const
    {
        Q_UNUSED(t_parent);
        return COLUMN_COUNT;
    }

    QVariant ReportsTable::data(const QModelIndex &t_index, int t_role) const
    {
        if (t_role != Qt::DisplayRole || !m_storage) {
            return QVariant();
        }

        int row = t_index.row();
        // Show newest reports first
        int storageIndex = m_storage->count() - 1 - row;
        auto report = m_storage->getReport(storageIndex);
        if (!report) {
            return QVariant();
        }

        switch (t_index.column()) {
        case SEQ_COLUMN:
            return report->seqNum;
        case TIMESTAMP_COLUMN:
            if (report->timestamp > 0) {
                return QDateTime::fromMSecsSinceEpoch(report->timestamp).toString("hh:mm:ss.zzz");
            }
            return "-";
        case REASON_COLUMN:
            return reasonToString(report->reasonCode);
        case VALUES_COLUMN: {
            QString val = report->entryValues.join("; ");
            if (val.size() > 128) {
                val.truncate(128);
                val += "...";
            }
            return val;
        }
        }
        return QVariant();
    }

    QVariantMap ReportsTable::getReportDetail(int t_row) const
    {
        QVariantMap result;
        if (!m_storage) {
            return result;
        }

        int storageIndex = m_storage->count() - 1 - t_row;
        auto report = m_storage->getReport(storageIndex);
        if (!report) {
            return result;
        }

        result["seqNum"] = report->seqNum;
        result["rcbRef"] = report->rcbRef;
        result["dataSetRef"] = report->dataSetRef;
        result["reason"] = reasonToString(report->reasonCode);

        if (report->timestamp > 0) {
            result["timestamp"] = QDateTime::fromMSecsSinceEpoch(report->timestamp)
                                      .toString("yyyy-MM-dd hh:mm:ss.zzz");
        } else {
            result["timestamp"] = "-";
        }

        QVariantList entries;
        for (int i = 0; i < report->entryNames.size(); i++) {
            QVariantMap entry;
            entry["name"] = report->entryNames[i];
            entry["value"] = i < report->entryValues.size() ? report->entryValues[i] : QString();
            entry["reason"] = i < report->entryReasons.size() ? reasonToString(report->entryReasons[i]) : QString();
            entries.append(entry);
        }
        result["entries"] = entries;

        return result;
    }

    void ReportsTable::slotDataUpdated()
    {
        beginResetModel();
        endResetModel();
    }

    void ReportsTable::slotRCBSelected(int t_inx)
    {
        beginResetModel();

        Core::ReportStorage *newStorage = nullptr;

        if (m_ied && t_inx >= 0) {
            // Determine which overview table sent the signal
            auto *overviewTable = qobject_cast<RCB_OverviewTable*>(sender());
            if (overviewTable) {
                auto rcb = overviewTable->getSelectedReportBlock();
                if (rcb) {
                    QString prefix = rcb->isBuffered() ? "BR" : "RP";
                    QString rcbRef = QString("%1.%2.%3").arg(rcb->lnRef(), prefix, rcb->getName());
                    newStorage = m_ied->model().getOrCreateReportStorage(rcbRef);
                }
            }
        }

        switchStorage(newStorage);
        endResetModel();
    }

    void ReportsTable::slotReportReceived()
    {
        if (!m_storage) {
            return;
        }
        int newRow = 0; // Newest at top
        beginInsertRows(QModelIndex(), newRow, newRow);
        endInsertRows();
    }

    void ReportsTable::switchStorage(Core::ReportStorage *t_storage)
    {
        if (m_storageConn) {
            disconnect(m_storageConn);
        }
        m_storage = t_storage;
        if (m_storage) {
            m_storageConn = connect(m_storage, &Core::ReportStorage::sigReportReceived,
                                    this, &ReportsTable::slotReportReceived,
                                    Qt::QueuedConnection);
        }
    }

    QString ReportsTable::reasonToString(int t_reason)
    {
        QStringList parts;
        if (t_reason & 1)  parts.append("DataChange");
        if (t_reason & 2)  parts.append("QualityChange");
        if (t_reason & 4)  parts.append("DataUpdate");
        if (t_reason & 8)  parts.append("Integrity");
        if (t_reason & 16) parts.append("GI");
        if (parts.isEmpty()) {
            return "Unknown";
        }
        return parts.join("|");
    }
}
