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
    QSharedPointer<ModelItem> ItemFactory::createModel(const QString &t_name)
    {
        return QSharedPointer<DataModel>::create(t_name);
    }

    QSharedPointer<ModelItem> ItemFactory::createLD(ModelItem *t_parent, const QString &t_name)
    {
        return QSharedPointer<LogicalDevice>::create(t_parent, t_name);
    }

    QSharedPointer<ModelItem> ItemFactory::createLN(ModelItem *t_parent, const QString &t_name)
    {
        return QSharedPointer<LogicalNode>::create(t_parent, t_name);
    }

    QSharedPointer<ModelItem> ItemFactory::createDO(ModelItem *t_parent, const QString &t_name)
    {
        return QSharedPointer<DataObject>::create(t_parent, t_name);
    }

    QSharedPointer<ModelItem> ItemFactory::createDA(ModelItem *t_parent, const QString &t_name,
                                                const QString &t_fc)
    {
        return QSharedPointer<DataAttribute>::create(t_parent, t_name, t_fc);
    }

    QSharedPointer<ModelItem> ItemFactory::createSA(ModelItem *t_parent, const QString &t_name)
    {
        return QSharedPointer<SubAttribute>::create(t_parent, t_name);
    }
}