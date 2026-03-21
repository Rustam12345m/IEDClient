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

#include <QSortFilterProxyModel>

namespace App::Models
{
    /**
     * @brief Recursive filter proxy for tree models.
     * Shows a node if its full path matches all filter parts (split by ".").
     * Parent nodes are kept visible to preserve tree structure.
     * Example: filter "mag.f" matches "Control.LLN0.Pos.Oper.ctlVal.mag.f"
     */
    class TreeFilterProxy : public QSortFilterProxyModel
    {
        Q_OBJECT
    public:
        TreeFilterProxy(QObject *parent = nullptr);

        Q_INVOKABLE void setFilter(const QString &text);

    protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

    private:
        QString buildPath(const QModelIndex &index) const;
        bool matchesPath(const QString &path) const;
        bool hasMatchingDescendant(const QModelIndex &parent) const;

        QString m_filter;
        QStringList m_filterParts;
    };
}
