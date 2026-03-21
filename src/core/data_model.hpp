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
#include "report_storage.hpp"

namespace Core
{
    /**
     * @brief Representation of a model available by MMS of an IED
     */
    class DataModel : public ModelItem
    {
    public:
        using ptr = QSharedPointer< DataModel >;

        DataModel(const QString &name) : ModelItem(nullptr, name)
        {
            m_delimetr = ""; // There isn't a delimetr because it is a top node
        }
        ~DataModel() = default;

        ModelItem::ptr      getItemByReference(const QString &ref);
        LogicalDevice::ptr  getLogicalDevice(int ld);
        LogicalNode::ptr    getLogicalNode(int ld, int ln);

        const auto getDataSetList() { return m_ds; }
        const auto getReportCBList() { return m_rcb; }
        const auto getGO_CBList() { return m_gocb; }
        const auto getSV_CBList() { return m_svcb; }

        void pushDataSet(DataSet::ptr ds);
        void pushReportCB(ReportBlock::ptr cb);
        void pushGooseCB(GooseControlBlock::ptr cb);
        void pushSV_CB(SV_ControlBlock::ptr cb);

        ReportStorage* getOrCreateReportStorage(const QString &rcbRef);

        void print();

    private:
        ModelItem::ptr recFindModelItem(QStringList &names, int inx, ModelItem::ptr item);
        void resolveIEDName(); // TODO: Move to DataModelBuilder

    private:
        QList< DataSet::ptr >             m_ds;
        QList< ReportBlock::ptr >         m_rcb;
        QList< GooseControlBlock::ptr >   m_gocb;
        QList< SV_ControlBlock::ptr >     m_svcb;
        QMap<QString, ReportStorage*>     m_reportStorages;
    
    friend class DataModelBuilder;
    };
}
