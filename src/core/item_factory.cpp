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

#include "item_factory.hpp"

#include "data_model.hpp"
#include "logical_device.hpp"
#include "logical_node.hpp"
#include "data_object.hpp"
#include "dataset.hpp"
#include "report_control_block.hpp"
#include "goose_control_block.hpp"
#include "sv_control_block.hpp"

namespace Core
{
    ModelItem::ptr ItemFactory::createModel(const QString &name)
    {
        return DataModel::ptr::create(name);
    }

    ModelItem::ptr ItemFactory::createLD(ModelItem *parent, const QString &name)
    {
        return LogicalDevice::ptr::create(parent, name);
    }

    ModelItem::ptr ItemFactory::createLN(ModelItem *parent, const QString &name)
    {
        return LogicalNode::ptr::create(parent, name);
    }

    ModelItem::ptr ItemFactory::createDO(ModelItem *parent, const QString &name)
    {
        return DataObject::ptr::create(parent, name);
    }

    ModelItem::ptr ItemFactory::createDA(ModelItem *parent, const QString &name,
                                                const QString &fc)
    {
        return DataAttribute::ptr::create(parent, name, fc);
    }

    ModelItem::ptr ItemFactory::createSA(ModelItem *parent, const QString &name)
    {
        return SubAttribute::ptr::create(parent, name);
    }
}