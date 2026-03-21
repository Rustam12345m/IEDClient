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

#include <QString>
#include <QSharedPointer>

namespace Core
{
    class ModelItemValue
    {
    public:
        using ptr = QSharedPointer< ModelItemValue >;

        ModelItemValue() {}
        ModelItemValue(const QString &value) : m_value(value) {}

        QString     str() {
            return m_value;
        }

        bool operator==(const ModelItemValue &other) const {
            return (m_value == other.m_value);
        }

        static auto create(const QString &value) {
            return ModelItemValue::ptr::create(value);
        }

    private:
        QString     m_value;
    };

    /**
     * This class helps to fetch a full value for a ModelItem that contains all sub values
     */
    class ModelItem;
    class ModelItemFullValue
    {
    public:
        static QString get(QSharedPointer< ModelItem > item);
    };
}
