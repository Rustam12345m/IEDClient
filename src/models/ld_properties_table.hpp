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

#include <QAbstractListModel>

#include "core/ied.hpp"

namespace App::Models
{
    /**
     * @brief ListModel with properties for the selected LD
     */
    class LD_PropertiesTable : public QAbstractListModel
    {
        Q_OBJECT
        enum Roles
        {
            SECTION_ROLE = Qt::UserRole + 1,
            NAME_ROLE,
            VALUE_ROLE
        };
        struct PropertyItem
        {
            QString     section;
            QString     name;
            QString     obj;

            PropertyItem(const QString &t_node, const QString &t_name, const QString &t_obj)
                : section{t_node}, name{t_name}, obj{t_obj} {}
        };

    public:
        LD_PropertiesTable(QObject *t_parent, Core::IED::ptr t_ied);

        void setActiveIED(Core::IED::ptr t_ied);

        QHash<int, QByteArray> roleNames() const override;
        int rowCount(const QModelIndex &t_index = QModelIndex()) const override;
        QVariant data(const QModelIndex &t_index, int t_role = Qt::DisplayRole) const override;

    private:
        QVariant dataLD(const QModelIndex &t_index, int t_role) const;
        QVariant dataIED(const QModelIndex &t_index, int t_role) const;

    public slots:
        void slotLDSelected(int t_ld);

    private:
        Core::IED::ptr      m_ied;
        QList<PropertyItem> m_ldProp;
        QList<PropertyItem> m_devProp;
        int                 m_currentLD = -1;
    };
}
