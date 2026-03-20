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
     * @brief Qt table model for GOOSE Control Blocks overview
     */
    class GOOSE_OverviewTable : public QAbstractTableModel
    {
        Q_OBJECT
        enum ColumnType {
            GOOSE_ENA_COLUMN = 0,
            GOOSE_ID_COLUMN,
            GOOSE_DS_COLUMN,
            GOOSE_CREV_COLUMN,
            GOOSE_MINTIME_COLUMN,
            GOOSE_MAXTIME_COLUMN,
            GOOSE_APPID_COLUMN,
            GOOSE_VLANID_COLUMN,
            GOOSE_VLANPRI_COLUMN,

            COLUMN_COUNT
        };

    public:
        GOOSE_OverviewTable(QObject *t_parent, Core::IED::ptr t_ied);

        Q_INVOKABLE void setSelectedGOOSE(int t_row);

        Q_INVOKABLE QString  selectedGoId() const;
        Q_INVOKABLE QString  selectedDatSet() const;
        Q_INVOKABLE bool     selectedGoEna() const;
        Q_INVOKABLE uint32_t selectedConfRev() const;
        Q_INVOKABLE uint32_t selectedMinTime() const;
        Q_INVOKABLE uint32_t selectedMaxTime() const;
        Q_INVOKABLE uint16_t selectedAppId() const;
        Q_INVOKABLE uint16_t selectedVlanId() const;
        Q_INVOKABLE uint8_t  selectedVlanPriority() const;

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
