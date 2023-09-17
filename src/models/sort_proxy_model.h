
#pragma once

#include <QSortFilterProxyModel>

class SimpleProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	SimpleProxyModel(QObject *t_parent) : QSortFilterProxyModel(t_parent) {
	}
	~SimpleProxyModel() {
	}

	bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const override;
	
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};
