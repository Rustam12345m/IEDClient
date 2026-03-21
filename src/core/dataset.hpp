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

#include <QObject>

#include "data_object.hpp"
#include "model_values_updater.hpp"

namespace Core
{
    class DataSetItem : public ModelItem
    {
    public:
        using ptr = QSharedPointer< DataSetItem >;

        DataSetItem(ModelItem *parent, const QString &ref, const QString &fc)
            : ModelItem(parent, ref), m_fc{fc}
        {}

        const QString    ref() const { return m_name; }
        const QString    fc() const { return m_fc; }
        ModelItem::ptr   item() { return m_valItem; }

        QString          getValue() const override {
            if (m_valItem) {
                return m_valItem->getValue();
            }
            return ModelItem::getValue();
        }

    private:
        QString         m_fc;
        ModelItem::ptr  m_valItem;

    friend class DataModelBuilder;
    };

    /**
     * @class DataSet
     * @brief Representation for a DataSet of IED
     */
    class DataSet : public QObject, public ModelItem
    {
        Q_OBJECT
    public:
        using ptr = QSharedPointer< DataSet >;

        DataSet(ModelItem *parent, const QString &name, const QString &lnRef, bool del)
            : QObject(nullptr), ModelItem(parent, name),
            m_lnReference{lnRef}, m_isDeletable{del}
        {}

        const QString ref() const { return m_lnReference; }

    signals:
        void sigItemUpdated(QList<ModelItem::ptr> items);

    private:
        QString m_lnReference;
        bool    m_isDeletable = false;
    };
}
