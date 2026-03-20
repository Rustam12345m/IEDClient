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

#include "models_stub.hpp"
#include "core/ied.hpp"

namespace App::Models
{
    /**
     * @brief Qt table model for Sampled Values Control Blocks overview
     */
    class SV_OverviewTable : public QAbstractTableModel
    {
        Q_OBJECT
        enum ColumnType {
            SV_ENA_COLUMN = 0,
            SV_TYPE_COLUMN,
            SV_ID_COLUMN,
            SV_DS_COLUMN,
            SV_CREV_COLUMN,
            SV_SMPRATE_COLUMN,
            SV_NOASDU_COLUMN,

            COLUMN_COUNT
        };

    public:
        SV_OverviewTable(QObject *t_parent, Core::IED::ptr t_ied);

        Q_INVOKABLE void setSelectedSV(int t_row);

        Q_INVOKABLE QString  selectedSvId() const;
        Q_INVOKABLE QString  selectedDatSet() const;
        Q_INVOKABLE bool     selectedSvEna() const;
        Q_INVOKABLE bool     selectedIsMulticast() const;
        Q_INVOKABLE uint32_t selectedConfRev() const;
        Q_INVOKABLE uint32_t selectedSmpRate() const;
        Q_INVOKABLE int      selectedNoASDU() const;

        void        setActiveIED(Core::IED::ptr t_ied);

        QVariant    headerData(int t_section, Qt::Orientation t_orientation,
                               int t_role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;

        int         rowCount(const QModelIndex &t_parent = QModelIndex()) const override;
        int         columnCount(const QModelIndex &t_parent = QModelIndex()) const override;
        QVariant    data(const QModelIndex &t_index, int t_role = Qt::DisplayRole) const override;

    public slots:
        void    slotDataUpdated(bool t_done);

    private:
        Core::IED::ptr m_ied;
        int m_selectedRow = -1;
    };
}
