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

#include "data_model_builder.hpp"

namespace Core
{
    DataModelBuilder::DataModelBuilder()
    {
        m_model = QSharedPointer<DataModel>::create("");
    }

    DataModelBuilder::~DataModelBuilder()
    {
    }

    DataModelBuilder& DataModelBuilder::createLD(const QString &t_name)
    {
        m_lastLD = LogicalDevice::ptr::create(m_model.get(), t_name);
        m_model->addSubItem(m_lastLD);
        return *this;
    }

    DataModelBuilder& DataModelBuilder::createLN(const QString &t_name)
    {
        m_lastLN = LogicalNode::ptr::create(m_lastLD.get(), t_name);
        m_lastLD->addSubItem(m_lastLN);
        return *this;
    }

    DataModelBuilder& DataModelBuilder::createDO(const QString &t_name)
    {
        m_lastDO = DataObject::ptr::create(m_lastLN.get(), t_name);
        m_lastLN->addSubItem(m_lastDO);
        return *this;
    }

    DataModelBuilder& DataModelBuilder::createDA(const QString &t_name, const QString &t_fc)
    {
        m_lastDA = DataAttribute::ptr::create(m_lastDO.get(), t_name, t_fc);
        m_lastDO->addSubItem(m_lastDA);
        return *this;
    }

    DataModelBuilder& DataModelBuilder::createSDA(ModelItem::ptr t_parent, const QString &t_name)
    {
        m_lastSDA = SubAttribute::ptr::create(t_parent.get(), t_name);
        t_parent->addSubItem(m_lastSDA);
        return *this;
    }

    DataModelBuilder& DataModelBuilder::createDataSet(const QString &t_name, const QString &t_lnRef, bool t_del)
    {
        m_lastDataSet = DataSet::ptr::create(lastLN().get(), t_name, t_lnRef, t_del);
        m_model->pushDataSet(m_lastDataSet);
        // m_lastLN->
        return *this;
    }

    DataModelBuilder& DataModelBuilder::createDataSet_Elem(const QString &t_ref, const QString &t_fc)
    {
        auto dsEnt = DataSetItem::ptr::create(lastDataSet().get(), t_ref, t_fc);
        lastDataSet()->addSubItem(dsEnt);
        return *this;
    }

    DataModelBuilder& DataModelBuilder::createRCB(const QString &t_name, const QString &t_lnRef, bool t_isBuffered)
    {
        m_lastRCB = ReportBlock::ptr::create(lastLN().get(), t_name, t_lnRef, t_isBuffered);
        m_model->pushReportCB(m_lastRCB);
        return *this;
    }

    DataModelBuilder& DataModelBuilder::createGOCB(const QString &t_name, const QString &t_lnRef)
    {
        m_lastGOCB = GooseControlBlock::ptr::create(lastLN().get(), t_name, t_lnRef);
        m_model->pushGooseCB(m_lastGOCB);
        return *this;
    }

    DataModelBuilder& DataModelBuilder::createSVCB(const QString &t_name, const QString &t_lnRef, bool t_isMulticast)
    {
        m_lastSVCB = SV_ControlBlock::ptr::create(lastLN().get(), t_name, t_lnRef, t_isMulticast);
        m_model->pushSV_CB(m_lastSVCB);
        return *this;
    }

    QSharedPointer<DataModel> DataModelBuilder::build(QThread *t_guiThread)
    {
        m_model->resolveIEDName();

        for (size_t i=0;i<m_model->getItemCount();i++) {
            auto ld = m_model->getLogicalDevice(i);

            for (size_t j=0;j<ld->getItemCount();j++) {
                auto ln = ld->getItem<LogicalNode>(j);
                ln->moveToThread(t_guiThread); // GUI thread

                ln->m_sigMatrix = LN_SignalMatrixBuilder::create(ln);
            }
        }

        // Resolve dsRefItem in the model
        for (auto ds : m_model->getDataSetList()) {
            for (size_t i=0;i<ds->getItemCount();i++) {
                auto dsItem = ds->getItem<DataSetItem>(i);
                if (dsItem) {
                    dsItem->m_valItem = m_model->getItemByReference(dsItem->ref());
                }
            }
        }
        return m_model;
    }
}