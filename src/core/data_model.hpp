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
        DataModel(const QString &t_name) : ModelItem(nullptr, t_name)
        {
            m_delimetr = ""; // There isn't a delimetr because it is a top node
        }
        ~DataModel() = default;

        ptrModelItem getItemByReference(const QString &t_ref);
        ptrLD     getLogicalDevice(int t_ld);
        ptrLN     getLogicalNode(int t_ld, int t_ln);

        const QList<ptrDataSet> getDataSetList() {
            return m_ds;
        }
        const QList<ptrRCB>     getReportCBList() {
            return m_rcb;
        }
        const QList<ptrGOCB>    getGO_CBList() {
            return m_gocb;
        }
        const QList<ptrSVCB>    getSV_CBList() {
            return m_svcb;
        }

        void    pushDataSet(ptrDataSet t_ds);
        void    pushReportCB(ptrRCB t_cb);
        void    pushGooseCB(ptrGOCB t_cb);
        void    pushSV_CB(ptrSVCB t_cb);

        void    print();

    private:
        ptrModelItem recFindModelItem(QStringList &t_names, int t_inx, ptrModelItem t_item);
        void          resolveIEDName(); // TODO: Move to DataModelBuilder

    private:
        QList<ptrDataSet>    m_ds;
        QList<ptrRCB>        m_rcb;
        QList<ptrGOCB>       m_gocb;
        QList<ptrSVCB>       m_svcb;
    
    friend class DataModelBuilder;
    };
    typedef QSharedPointer<DataModel>    ptrDataModel;
}