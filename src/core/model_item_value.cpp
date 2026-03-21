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

#include "model_item_value.hpp"
#include "model_item.hpp"

namespace Core
{
    namespace
    {
        void recurs_fillValue(ModelItem::ptr root, ModelItem::ptr item, QString &val)
        {
            if (item->getItemCount() == 0) {
                // Leaft = End
                val += QString("%1 = \"%2\"; ").arg(item->getReference(root.get())).arg(item->getValue());
            }

            auto subItemList = item->getItemList();
            for (auto s : subItemList) {
                recurs_fillValue(root, s, val);
            }
        }
    }

    QString ModelItemFullValue::get(QSharedPointer< ModelItem > item)
    {
        if (item->getItemCount() == 0) {
            return item->getValue();
        }

        auto subItemList = item->getItemList();
        QString fullValue;
        recurs_fillValue(item, item, fullValue);
        return fullValue;
    }
}