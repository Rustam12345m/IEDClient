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

#include "core/data_model.hpp"
#include "core/fs_model.hpp"

namespace Core
{
    class DataModelBuilder
    {
    public:
        DataModelBuilder();
        ~DataModelBuilder();

        DataModelBuilder& createLD(const QString &name);
        DataModelBuilder& createLN(const QString &name);
        DataModelBuilder& createDO(const QString &name);
        DataModelBuilder& createDA(const QString &name, const QString &fc);
        DataModelBuilder& createSDA(ModelItem::ptr parent, const QString &name);

        DataModelBuilder& createDataSet(const QString &name, const QString &lnRef, bool del);
        DataModelBuilder& createDataSet_Elem(const QString &ref, const QString &fc);
        DataModelBuilder& createRCB(const QString &name, const QString &lnRef, bool isBuffered);
        DataModelBuilder& createGOCB(const QString &name, const QString &lnRef);
        DataModelBuilder& createSVCB(const QString &name, const QString &lnRef, bool isMulticast);
        DataModelBuilder& createSGCB(const QString &ldRef, uint8_t numOfSG, uint8_t actSG);

        auto lastLD() const { return m_lastLD; }
        auto lastLN() const { return m_lastLN; }
        auto lastDO() const { return m_lastDO; }
        auto lastDA() const { return m_lastDA; }
        auto lastSDA() const { return m_lastSDA; }

        auto lastDataSet() const { return m_lastDataSet; }
        auto lastRCB() const { return m_lastRCB; }
        auto lastGOCB() const { return m_lastGOCB; }
        auto lastSVCB() const { return m_lastSVCB; }
        auto lastSGCB() const { return m_lastSGCB; }

        DataModel::ptr    build(QThread *guiThread);

    private:
        DataModel::ptr          m_model;
        LogicalDevice::ptr      m_lastLD;
        LogicalNode::ptr        m_lastLN;
        DataObject::ptr         m_lastDO;
        DataAttribute::ptr      m_lastDA;
        SubAttribute::ptr       m_lastSDA;
        //
        DataSet::ptr            m_lastDataSet;
        ReportBlock::ptr        m_lastRCB;
        GooseControlBlock::ptr  m_lastGOCB;
        SV_ControlBlock::ptr    m_lastSVCB;
        SGCB::ptr               m_lastSGCB;
    };
}