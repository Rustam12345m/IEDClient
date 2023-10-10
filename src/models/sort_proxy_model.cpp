
#include "sort_proxy_model.hpp"
#include "models_stub.hpp"

#include <QDebug>

bool SortProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
	//qDebug() << "SortProxyModel: filterAcceptsRow row " << source_row;
	return true;
}

bool SortProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
	//qDebug() << "SortProxyModel: filterAcceptsColumn row " << source_column;
	return true;
}

bool SortProxyModel::lessThan(const QModelIndex &t_left, const QModelIndex &t_right) const
{
	//qDebug() << "SortProxyModel: lessThan L = " << t_left.row() << ", R = " << t_right.row();

	QAbstractItemModel *m = sourceModel();
	if (m != nullptr) {
		QVariant l = m->data(t_left, ComRoles::ROLE_SORT_VALUE),
				 r = m->data(t_right, ComRoles::ROLE_SORT_VALUE);

		if (l.typeId() == r.typeId()) {
			switch (l.typeId()) {
			case QVariant::LongLong: {
				return (l.toLongLong() < r.toLongLong());
			}
			case QVariant::Double: {
				return (l.toDouble() < r.toDouble());
			}
			case QVariant::String: {
				return (l.toString() < r.toString());
			}
			}
		}
	}
	return false;
}