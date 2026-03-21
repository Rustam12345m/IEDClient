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

#include "tree_filter_proxy.hpp"

namespace App::Models
{
    TreeFilterProxy::TreeFilterProxy(QObject *parent)
        : QSortFilterProxyModel(parent)
    {
        setRecursiveFilteringEnabled(true);
    }

    void TreeFilterProxy::setFilter(const QString &text)
    {
        m_filter = text.trimmed();
        m_filterParts = m_filter.split('.', Qt::SkipEmptyParts);
        invalidateFilter();
    }

    bool TreeFilterProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
    {
        if (m_filter.isEmpty()) {
            return true;
        }

        QModelIndex idx = sourceModel()->index(sourceRow, 0, sourceParent);

        // Build the full path from root to this node
        QString path = buildPath(idx);

        if (matchesPath(path)) {
            return true;
        }

        return hasMatchingDescendant(idx);
    }

    QString TreeFilterProxy::buildPath(const QModelIndex &index) const
    {
        if (!index.isValid()) return {};

        QStringList parts;
        QModelIndex cur = index;
        while (cur.isValid()) {
            QString name = sourceModel()->data(cur, Qt::DisplayRole).toString();
            parts.prepend(name);
            cur = cur.parent();
        }
        return parts.join(".");
    }

    bool TreeFilterProxy::matchesPath(const QString &path) const
    {
        if (m_filterParts.isEmpty()) return false;

        // All filter parts must appear in the path in order
        int pos = 0;
        for (const auto &part : m_filterParts) {
            int found = path.indexOf(part, pos, Qt::CaseInsensitive);
            if (found < 0) return false;
            pos = found + part.length();
        }
        return true;
    }

    bool TreeFilterProxy::hasMatchingDescendant(const QModelIndex &parent) const
    {
        int rows = sourceModel()->rowCount(parent);
        for (int i = 0; i < rows; i++) {
            QModelIndex child = sourceModel()->index(i, 0, parent);
            QString path = buildPath(child);
            if (matchesPath(path) || hasMatchingDescendant(child)) {
                return true;
            }
        }
        return false;
    }
}
