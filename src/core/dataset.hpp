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
        DataSetItem(ModelItem *t_parent, const QString &t_ref, const QString &t_fc)
            : ModelItem(t_parent, t_ref), m_fc{t_fc}
        {}

        const QString    ref() const { return m_name; }
        const QString    fc() const { return m_fc; }
        ptrModelItem     item() { return m_valItem; }

        QString          getValue() const override {
            if (m_valItem) {
                return m_valItem->getValue();
            }
            return ModelItem::getValue();
        }

    private:
        QString         m_fc;
        ptrModelItem    m_valItem;

    friend class DataModelBuilder;
    };

    /*
     * Representation of a DataSet of IED
     * */
    class DataSet : public QObject, public ModelItem
    {
        Q_OBJECT
    public:
        DataSet(ModelItem *t_parent, const QString &t_name, const QString &t_lnRef, bool t_del)
            : QObject(nullptr), ModelItem(t_parent, t_name),
            m_lnReference{t_lnRef}, m_isDeletable{t_del}
        {}

        const QString ref() const { return m_lnReference; }

    signals:
        void     sigItemUpdated(QList<ptrModelItem> t_items);

    private:
        QString m_lnReference;
        bool    m_isDeletable = false;
    };
    typedef QSharedPointer< DataSet >     ptrDataSet;
}