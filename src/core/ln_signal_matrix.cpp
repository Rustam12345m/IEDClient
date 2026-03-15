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

namespace Core
{
    LN_SignalMatrix::ptr LN_SignalMatrixBuilder::create(QSharedPointer< LogicalNode > t_ln,
                                                         const QList<QString> &t_fcFilter)
    {
        auto matrix = LN_SignalMatrix::ptr::create();

        auto dataObjList = t_ln->getItemList();
        for (auto item : dataObjList) {
            SignalMatrixRow rowPrototype;
            rowPrototype.m_dataObject = item;
            recursiveFillMatrix(matrix, t_ln, item, rowPrototype, t_fcFilter);
        }
        return matrix;
    }

    void LN_SignalMatrixBuilder::recursiveFillMatrix(LN_SignalMatrix::ptr t_matrix,
                                                     ModelItem::ptr t_root,
                                                     ModelItem::ptr t_item,
                                                     SignalMatrixRow t_rowPrototype,
                                                     const QList<QString> &t_fcFilter)
    {
        // Find Q, TS, Desc elements
        for (size_t i=0;i<t_item->getItemCount();i++) {
            ModelItem::ptr child = t_item->getItem(i);
            if (child->getName() == "q") {
                t_rowPrototype.m_quality = child;
                continue;
            }
            if (child->getName() == "t") {
                t_rowPrototype.m_timestamp = child;
                continue;
            }
            if (child->getName() == "d") {
                t_rowPrototype.m_desc = child;
                continue;
            }
        }

        const QList<QString> ATTR = { "q", "t", "d" };

        // Create signals
        for (size_t i=0;i<t_item->getItemCount();i++) {
            ModelItem::ptr child = t_item->getItem(i);

            auto da = child.dynamicCast<Core::DataAttribute>();
            if (da) {
                if (!t_fcFilter.contains(da->fcStr())) {
                    continue;
                }
                t_rowPrototype.m_fc = da->fcStr();
            }

            if (ATTR.contains(child->getName())) {
                continue;
            }

            if (child->getItemCount() == 0) {
                // Leaf
                t_rowPrototype.m_path = child->getReference(t_root.get());
                t_rowPrototype.m_value = child;
                t_matrix->m_signals.emplace_back(t_rowPrototype);
            } else {
                recursiveFillMatrix(t_matrix, t_root, child, t_rowPrototype, t_fcFilter);
            }
        }
    }
}