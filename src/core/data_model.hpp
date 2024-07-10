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

#include "logical_device.hpp"

namespace Core
{
    /*
     * Representation of a model available by MMS of an IED
     * */
    class DataModel : public ModelItem
    {
    public:
        using ptr = QSharedPointer< DataModel >;

        DataModel(const QString &t_name) : ModelItem(nullptr, t_name)
        {
            m_delimetr = ""; // There isn't a delimetr because it is a top node
        }
        ~DataModel() = default;

        ModelItem::ptr      getItemByReference(const QString &t_ref);
        LogicalDevice::ptr  getLogicalDevice(int t_ld);
        LogicalNode::ptr    getLogicalNode(int t_ld, int t_ln);

        const auto getDataSetList() { return m_ds; }
        const auto getReportCBList() { return m_rcb; }
        const auto getGO_CBList() { return m_gocb; }
        const auto getSV_CBList() { return m_svcb; }

        void pushDataSet(DataSet::ptr t_ds);
        void pushReportCB(ReportBlock::ptr t_cb);
        void pushGooseCB(GooseControlBlock::ptr t_cb);
        void pushSV_CB(SV_ControlBlock::ptr t_cb);

        void print();

    private:
        ModelItem::ptr recFindModelItem(QStringList &t_names, int t_inx, ModelItem::ptr t_item);
        void resolveIEDName(); // TODO: Move to DataModelBuilder

    private:
        QList< DataSet::ptr >             m_ds;
        QList< ReportBlock::ptr >         m_rcb;
        QList< GooseControlBlock::ptr >   m_gocb;
        QList< SV_ControlBlock::ptr >     m_svcb;
    
    friend class DataModelBuilder;
    };
}