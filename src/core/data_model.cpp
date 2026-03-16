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

#include "data_model.hpp"

#include <QDebug>

namespace Core
{
    namespace
    {
        void     printTree(QString t_prefix, ModelItem::ptr t_item)
        {
            auto &nodeList = t_item->getItemList();
            for (auto node : nodeList) {
                qDebug().noquote() << t_prefix << node->getName();

                printTree(t_prefix + "  ", node);
            }
        }
    }

    void DataModel::resolveIEDName()
    {
        if (m_items.isEmpty()) {
            m_name = "Undefined";
        } else if (m_items.size() == 1) {
            // We have only one LD
            m_name = m_items[0]->getName().first(m_items[0]->getName().size() / 2);
        } else {
            // We have more than one LD
            QList<QString> ldNames;
            size_t minSize = 0;
            for (auto ld : m_items) {
                QString name = ld->getName();
                if (name.size() > minSize) {
                    minSize = name.size();
                }
                ldNames.push_back(name);
            }

            for (size_t i=0;i<minSize;i++) {
                QChar letter = ldNames[0].at(i);

                for (size_t j=1;j<ldNames.size();j++) {
                    if (ldNames[j].at(i) != letter) {
                        m_name = ldNames[0].first(i);
                        i = minSize;
                        break;
                    }
                }
            }
        }

        // Change LDs' names
        for (size_t i=0;i<m_items.size();i++) {
            LogicalDevice::ptr ld = getLogicalDevice(i);
            QString name = ld->getName();
            ld->m_name = name.right(name.size() - m_name.size());
        }
    }

    LogicalDevice::ptr DataModel::getLogicalDevice(int t_ld)
    {
        return getItem<Core::LogicalDevice>(t_ld);
    }

    LogicalNode::ptr DataModel::getLogicalNode(int t_ld, int t_ln)
    {
        auto ld = getItem<Core::LogicalDevice>(t_ld);
        if (ld) {
            return ld->getItem<Core::LogicalNode>(t_ln);
        }
        return nullptr;
    }

    ModelItem::ptr DataModel::getItemByReference(const QString &t_ref)
    {
        // MMS_REF: "HugeModelIEDMain/GGIO1.Mod[CO]"
        // MMS_REF: "HugeModelIEDMain/GGIO1.Mod.stVal[ST]"
        int inx = t_ref.indexOf('/'), nextInx = -1;
        if (inx == -1) {
            return nullptr;
        }
        QString ldName = t_ref.mid(m_name.size(), inx - m_name.size());
        inx++;

        QStringList names;
        while ((nextInx = t_ref.indexOf('.', inx)) != -1) {
            names.push_back(t_ref.mid(inx, nextInx - inx));
            inx = nextInx + 1;
        }
        if (inx < t_ref.size()) {
            names.push_back(t_ref.mid(inx, t_ref.size() - inx));
        }

        // qDebug() << "GetItemByRef: ref = " << t_ref << ", ldName = " << ldName << ", nameList = " << names;
        return recFindModelItem(names, 0, findSubItem(ldName));
    }

    void DataModel::print()
    {
        qDebug() << "IED: " << m_name;

        auto &ldList = m_items;
        for (auto ld : ldList) {
            qDebug() << "  LD: " << ld->getName();
            printTree("    ", ld);
        }
    }
    
    void DataModel::pushDataSet(DataSet::ptr t_ds)
    {
        m_ds.push_back(t_ds);
    }

    void DataModel::pushReportCB(ReportBlock::ptr t_cb)
    {
        m_rcb.push_back(t_cb);
    }

    void DataModel::pushGooseCB(GooseControlBlock::ptr t_cb)
    {
        m_gocb.push_back(t_cb);
    }

    void DataModel::pushSV_CB(SV_ControlBlock::ptr t_cb)
    {
        m_svcb.push_back(t_cb);
    }

    ReportStorage* DataModel::getOrCreateReportStorage(const QString &t_rcbRef)
    {
        auto it = m_reportStorages.find(t_rcbRef);
        if (it != m_reportStorages.end()) {
            return it.value();
        }
        auto *storage = new ReportStorage(nullptr);
        m_reportStorages.insert(t_rcbRef, storage);
        return storage;
    }

    ModelItem::ptr DataModel::recFindModelItem(QStringList &t_names, int t_inx, ModelItem::ptr t_item)
    {
        if (t_item == nullptr) {
            return nullptr;
        }
        if (t_inx == (t_names.size() - 1)) {
            // End
            return t_item->findSubItem(t_names[t_inx]);
        }
        return recFindModelItem(t_names, t_inx + 1, t_item->findSubItem(t_names[t_inx]));
    }
}