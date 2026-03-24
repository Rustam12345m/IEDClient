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

#include "ln_signal_matrix.hpp"
#include "logical_node.hpp"
#include "data_object.hpp"

namespace Core
{
    LN_SignalMatrix::ptr LN_SignalMatrixBuilder::create(QSharedPointer< LogicalNode > ln,
                                                         const QList<QString> &fcFilter)
    {
        auto matrix = LN_SignalMatrix::ptr::create();

        auto dataObjList = ln->getItemList();
        for (auto item : dataObjList) {
            SignalMatrixRow rowPrototype;
            rowPrototype.m_dataObject = item;
            recursiveFillMatrix(matrix, ln, item, rowPrototype, fcFilter);
        }
        return matrix;
    }

    void LN_SignalMatrixBuilder::recursiveFillMatrix(LN_SignalMatrix::ptr table,
                                                     ModelItem::ptr root,
                                                     ModelItem::ptr item,
                                                     SignalMatrixRow rowPrototype,
                                                     const QList<QString> &fcFilter)
    {
        // Find Q, TS, Desc elements
        for (size_t i=0;i<item->getItemCount();i++) {
            ModelItem::ptr child = item->getItem(i);
            if (child->getName() == "q") {
                rowPrototype.m_quality = child;
                continue;
            }
            if (child->getName() == "t") {
                rowPrototype.m_timestamp = child;
                continue;
            }
            if (child->getName() == "d") {
                rowPrototype.m_desc = child;
                continue;
            }
        }

        const QList<QString> ATTR = { "q", "t", "d" };

        // Create signals
        for (size_t i=0;i<item->getItemCount();i++) {
            ModelItem::ptr child = item->getItem(i);

            auto da = child.dynamicCast<Core::DataAttribute>();
            if (da) {
                if (!fcFilter.contains(da->fcStr())) {
                    continue;
                }
                rowPrototype.m_fc = da->fcStr();
            }

            if (ATTR.contains(child->getName())) {
                continue;
            }

            if (child->getItemCount() == 0) {
                // Leaf
                rowPrototype.m_path = child->getReference(root.get());
                rowPrototype.m_value = child;
                table->m_signals.emplace_back(rowPrototype);
            } else {
                recursiveFillMatrix(table, root, child, rowPrototype, fcFilter);
            }
        }
    }

    namespace
    {
        bool hasCOChildren(ModelItem::ptr item)
        {
            for (size_t i = 0; i < item->getItemCount(); i++) {
                auto da = item->getItem(i).dynamicCast<DataAttribute>();
                if (da && da->fcStr() == "CO") return true;
            }
            return false;
        }

        QString inferCDCType(ModelItem::ptr doItem)
        {
            // Check child names to infer CDC type
            for (size_t i = 0; i < doItem->getItemCount(); i++) {
                QString name = doItem->getItem(i)->getName();
                if (name == "stVal") {
                    auto da = doItem->getItem(i).dynamicCast<DataAttribute>();
                    if (!da) continue;
                    // Boolean → SPC, Dbpos (enum/integer with 4 states) → DPC, INT32 → INC
                    QString val = da->getValue();
                    if (val == "True" || val == "False") return "SPC";
                    // Check if it looks like Dbpos (intermediate-state/off/on/bad-state)
                    bool ok = false;
                    int intVal = val.toInt(&ok);
                    if (ok && intVal >= 0 && intVal <= 3) return "DPC";
                    if (ok) return "INC";
                    return "SPC";
                }
                if (name == "mag") return "APC";
                if (name == "valWTr") return "BSC";
            }
            return " - ";
        }
    }

    LN_SignalMatrix::ptr LN_SignalMatrixBuilder::createControlsMatrix(
        QSharedPointer<LogicalNode> ln)
    {
        auto matrix = LN_SignalMatrix::ptr::create();

        for (auto &item : ln->getItemList()) {
            if (!hasCOChildren(item)) continue;

            SignalMatrixRow row;
            row.m_dataObject = item;
            row.m_path = item->getName();
            row.m_fc = "CO";

            // Resolve all attributes once during build (cached as pointers)
            row.m_value    = item->findSubItem("stVal");
            if (!row.m_value) row.m_value = item->findSubItem("mag");
            row.m_desc     = item->findSubItem("d");
            row.m_ctlModel = item->findSubItem("ctlModel");
            row.m_stSeld   = item->findSubItem("stSeld");
            row.m_opOk     = item->findSubItem("opOk");
            row.m_ctlType  = inferCDCType(item);

            matrix->m_signals.emplace_back(row);
        }
        return matrix;
    }
}