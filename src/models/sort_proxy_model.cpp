
#include "sort_proxy_model.hpp"
#include "models_stub.hpp"

#include <QDebug>

namespace App::Models
{
    bool SortProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
    {
        return true;
    }

    bool SortProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
    {
        return true;
    }

    bool SortProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
    {
        QAbstractItemModel *m = sourceModel();
        if (m != nullptr) {
            QVariant l = m->data(source_left, ComRoles::ROLE_SORT_VALUE),
                    r = m->data(source_right, ComRoles::ROLE_SORT_VALUE);

            if (l.typeId() == r.typeId()) {
                switch (l.typeId()) {
                case QMetaType::LongLong: {
                    return (l.toLongLong() < r.toLongLong());
                }
                case QMetaType::Double: {
                    return (l.toDouble() < r.toDouble());
                }
                case QMetaType::QString: {
                    return (l.toString() < r.toString());
                }
                }
            }
        }
        return false;
    }
}