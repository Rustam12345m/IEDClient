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
        void     printTree(QString prefix, ModelItem::ptr item)
        {
            auto &nodeList = item->getItemList();
            for (auto node : nodeList) {
                // qDebug().noquote() << prefix << node->getName();

                printTree(prefix + "  ", node);
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

    LogicalDevice::ptr DataModel::getLogicalDevice(int ld)
    {
        return getItem<Core::LogicalDevice>(ld);
    }

    LogicalNode::ptr DataModel::getLogicalNode(int ld, int ln)
    {
        auto ldPtr = getItem<Core::LogicalDevice>(ld);
        if (ldPtr) {
            return ldPtr->getItem<Core::LogicalNode>(ln);
        }
        return nullptr;
    }

    ModelItem::ptr DataModel::getItemByReference(const QString &ref)
    {
        // MMS_REF: "HugeModelIEDMain/GGIO1.Mod[CO]"
        // MMS_REF: "HugeModelIEDMain/GGIO1.Mod.stVal[ST]"
        int inx = ref.indexOf('/'), nextInx = -1;
        if (inx == -1) {
            return nullptr;
        }
        QString ldName = ref.mid(m_name.size(), inx - m_name.size());
        inx++;

        QStringList names;
        while ((nextInx = ref.indexOf('.', inx)) != -1) {
            names.push_back(ref.mid(inx, nextInx - inx));
            inx = nextInx + 1;
        }
        if (inx < ref.size()) {
            names.push_back(ref.mid(inx, ref.size() - inx));
        }

        // qDebug() << "GetItemByRef: ref = " << ref << ", ldName = " << ldName << ", nameList = " << names;
        return recFindModelItem(names, 0, findSubItem(ldName));
    }

    void DataModel::print()
    {
        // qDebug() << "IED: " << m_name;

        auto &ldList = m_items;
        for (auto ld : ldList) {
            // qDebug() << "  LD: " << ld->getName();
            printTree("    ", ld);
        }
    }
    
    void DataModel::pushDataSet(DataSet::ptr ds)
    {
        m_ds.push_back(ds);
    }

    void DataModel::pushReportCB(ReportBlock::ptr cb)
    {
        m_rcb.push_back(cb);
    }

    void DataModel::pushGooseCB(GooseControlBlock::ptr cb)
    {
        m_gocb.push_back(cb);
    }

    void DataModel::pushSV_CB(SV_ControlBlock::ptr cb)
    {
        m_svcb.push_back(cb);
    }

    void DataModel::pushSGCB(SGCB::ptr cb)
    {
        m_sgcbs.push_back(cb);
    }

    ReportStorage* DataModel::getOrCreateReportStorage(const QString &rcbRef)
    {
        auto it = m_reportStorages.find(rcbRef);
        if (it != m_reportStorages.end()) {
            return it.value();
        }
        auto *storage = new ReportStorage(nullptr);
        m_reportStorages.insert(rcbRef, storage);
        return storage;
    }

    ModelItem::ptr DataModel::recFindModelItem(QStringList &names, int inx, ModelItem::ptr item)
    {
        if (item == nullptr) {
            return nullptr;
        }
        if (inx == (names.size() - 1)) {
            return item->findSubItem(names[inx]);
        }

        // Try all children with matching name (handles multiple FCs like phsA[DC] and phsA[MX])
        for (auto &child : item->getItemList()) {
            if (child->getName() == names[inx]) {
                auto result = recFindModelItem(names, inx + 1, child);
                if (result) {
                    return result;
                }
            }
        }
        return nullptr;
    }
}