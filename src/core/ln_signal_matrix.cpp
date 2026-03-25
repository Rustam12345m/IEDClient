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
            bool hasStVal = false;
            bool hasMag = false;
            bool hasValWTr = false;

            for (size_t i = 0; i < doItem->getItemCount(); i++) {
                QString name = doItem->getItem(i)->getName();
                if (name == "stVal")  hasStVal = true;
                if (name == "mag")    hasMag = true;
                if (name == "valWTr") hasValWTr = true;
            }

            // APC: has mag (analogue magnitude structure)
            if (hasMag) return "APC";
            // BSC: has valWTr (value with transient)
            if (hasValWTr) return "BSC";

            if (!hasStVal) return " - ";

            // Distinguish SPC/DPC/INC by checking stVal type:
            // SPC.stVal is Boolean, DPC.stVal is Dbpos (INT, 0-3), INC.stVal is INT32
            // Check Oper.ctlVal children to distinguish DPC from INC:
            // DPC Oper.ctlVal is a leaf (Dbpos), INC Oper.ctlVal is also a leaf (INT32)
            // Use stVal value heuristic: Boolean → SPC, otherwise check for 'q' sibling
            // that indicates it's a status attribute
            auto stVal = doItem->findSubItem("stVal");
            if (stVal) {
                QString val = stVal->getValue();
                if (val == "True" || val == "False") return "SPC";
            }

            // DPC vs INC: DPC has Cancel sub-object typically,
            // but both can have it. Use naming convention as fallback.
            // INC stVal range is unbounded, DPC stVal is 0-3.
            // Best effort: check if Oper.ctlVal has Dbpos-range value
            auto ctlVal = doItem->findSubItem("ctlVal");
            if (ctlVal && !ctlVal->getValue().isEmpty()) {
                bool ok = false;
                int v = ctlVal->getValue().toInt(&ok);
                if (ok && v >= 0 && v <= 3) return "DPC";
            }

            return "INC";
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
            // SPC/DPC/INC use stVal; APC uses mag.f (FLOAT32) or mag.i (INT32)
            row.m_value = item->findSubItem("stVal");
            if (!row.m_value) {
                row.m_value = item->findSubItem("mag", "f");
                if (!row.m_value) row.m_value = item->findSubItem("mag", "i");
                if (!row.m_value) row.m_value = item->findSubItem("mag");
            }
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