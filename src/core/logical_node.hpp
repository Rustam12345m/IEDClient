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

#include "dataset.hpp"
#include "report_control_block.hpp"
#include "goose_control_block.hpp"
#include "sv_control_block.hpp"

#include "ln_signal_matrix.hpp"

namespace Core
{
    /*
     * Representation of Logical Node
     * As a QObject this Item should live in GUI's thread
     * */
    class LogicalNode : public QObject, public ModelItem
    {
        Q_OBJECT
    public:
        using ptr = QSharedPointer< LogicalNode >;

        LogicalNode(ModelItem *t_parent, const QString &t_name)
            : QObject(nullptr), ModelItem(t_parent, t_name)
        {
            m_delimetr = "/"; // Between LDName and LNName
        }

        DataObject::ptr    getModItem() const { return m_mod; }
        DataObject::ptr    getBehItem() const { return m_beh; }
        DataObject::ptr    getHealthItem() const { return m_health; }

        void     addSubItem(ModelItem::ptr t_child) override {
            if (t_child->getName() == "Mod") {
                m_mod = t_child.dynamicCast<DataObject>();
            } else if (t_child->getName() == "Beh") {
                m_beh = t_child.dynamicCast<DataObject>();
            } else if (t_child->getName() == "Health") {
                m_health = t_child.dynamicCast<DataObject>();
            }
            ModelItem::addSubItem(t_child);
        }
        void     addSubItem(DataSet::ptr t_ds) {
            m_dataSets.push_back(t_ds);
        }
        void     addSubItem(ReportBlock::ptr t_rcb) {
            m_rcbs.push_back(t_rcb);
        }
        void     addSubItem(GooseControlBlock::ptr t_gocb) {
            m_gooses.push_back(t_gocb);
        }

        auto&    getSignalMatrix() const {
            return m_sigMatrix;
        }
        auto&    getDataSets() const {
            return m_dataSets;
        }
        auto&    getReportBlocks() const {
            return m_rcbs;
        }
        auto&    getGooseCB() const {
            return m_gooses;
        }

    signals:
        void     sigDataObjectUpdated(ModelItem::ptrList t_nodes);

    protected:
         void    notifyFromChild(ModelItem::ptrList t_nodes) override;

    protected:
        DataObject::ptr         m_mod;
        DataObject::ptr         m_beh;
        DataObject::ptr         m_health;
        LN_SignalMatrix::ptr    m_sigMatrix;
        QList< DataSet::ptr >           m_dataSets;
        QList< ReportBlock::ptr >       m_rcbs;
        QList< GooseControlBlock::ptr > m_gooses;

    friend class DataModelBuilder;
    };
}