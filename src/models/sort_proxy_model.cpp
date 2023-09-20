
#include "sort_proxy_model.hpp"

#include <QDebug>

bool SimpleProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
	//qDebug() << "SimpleProxyModel: filterAcceptsRow row " << source_row;
	return true;
}

bool SimpleProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
	//qDebug() << "SimpleProxyModel: filterAcceptsColumn row " << source_column;
	return true;
}

bool SimpleProxyModel::lessThan(const QModelIndex &t_left, const QModelIndex &t_right) const
{
	//qDebug() << "SimpleProxyModel: lessThan L = " << t_left.row() << ", R = " << t_right.row();

	QAbstractItemModel *m = sourceModel();
	if (m != nullptr) {
		QVariant l = m->data(t_left);
		QVariant r = m->data(t_right);

		/*
		qDebug() << "SimpleProxyModel: lessThan "
				<< QString("L = (%1 %2 %3)").arg(t_left.row()).arg(t_left.column()).arg(l.toString())
				<< QString(", R = (%1 %2 %3)").arg(t_right.row()).arg(t_right.column()).arg(r.toString());
		*/

		if (l.toString() < r.toString()) {
			return true;
		}
	}
	return false;
}
