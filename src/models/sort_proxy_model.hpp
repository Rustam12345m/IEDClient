
#pragma once

#include <QSortFilterProxyModel>

namespace App::Models
{
    class SortProxyModel : public QSortFilterProxyModel
    {
        Q_OBJECT
    public:
        SortProxyModel(QObject *parent) : QSortFilterProxyModel(parent) {}
        ~SortProxyModel() = default;

        bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
        bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const override;
        bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
    };
}