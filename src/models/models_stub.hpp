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

#include <QAbstractTableModel>

namespace App::Models
{
    class SortHeaderValue
    {
        // hack
        Q_GADGET
    public:
        SortHeaderValue() {}
        SortHeaderValue(const QString &text, bool sort)
            : m_text(text), m_sortable(sort) {}

        QString m_text;
        bool    m_sortable = false;

        Q_PROPERTY(int         sortable     MEMBER     m_sortable)
        Q_PROPERTY(QString     text         MEMBER     m_text)
    };

    enum ComRoles
    {
        ROLE_SORT_VALUE = Qt::UserRole + 1
    };
}
Q_DECLARE_METATYPE(App::Models::SortHeaderValue)