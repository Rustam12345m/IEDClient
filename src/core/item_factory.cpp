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
    ModelItem::ptr ItemFactory::createModel(const QString &t_name)
    {
        return DataModel::ptr::create(t_name);
    }

    ModelItem::ptr ItemFactory::createLD(ModelItem *t_parent, const QString &t_name)
    {
        return LogicalDevice::ptr::create(t_parent, t_name);
    }

    ModelItem::ptr ItemFactory::createLN(ModelItem *t_parent, const QString &t_name)
    {
        return LogicalNode::ptr::create(t_parent, t_name);
    }

    ModelItem::ptr ItemFactory::createDO(ModelItem *t_parent, const QString &t_name)
    {
        return DataObject::ptr::create(t_parent, t_name);
    }

    ModelItem::ptr ItemFactory::createDA(ModelItem *t_parent, const QString &t_name,
                                                const QString &t_fc)
    {
        return DataAttribute::ptr::create(t_parent, t_name, t_fc);
    }

    ModelItem::ptr ItemFactory::createSA(ModelItem *t_parent, const QString &t_name)
    {
        return SubAttribute::ptr::create(t_parent, t_name);
    }
}