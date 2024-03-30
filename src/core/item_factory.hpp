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

#include <QSharedPointer>

namespace Core
{
    class ModelItem; // forward

    /*
     * Factory for all classes inherit from Item
     * */
    class ItemFactory
    {
    public:
        // Object Tree <- base node for IED Data Object model
        static QSharedPointer<ModelItem> createModel(const QString &t_name);

        // Logical Device
        static QSharedPointer<ModelItem> createLD(ModelItem *t_parent, const QString &t_name);

        // Logical Node
        static QSharedPointer<ModelItem> createLN(ModelItem *t_parent, const QString &t_name);

        // Data Object
        static QSharedPointer<ModelItem> createDO(ModelItem *t_parent, const QString &t_name);

        // Data Attribute
        static QSharedPointer<ModelItem> createDA(ModelItem *t_parent, const QString &t_name,
                                                  const QString &t_fc);

        // Sub Attribute
        static QSharedPointer<ModelItem> createSA(ModelItem *t_parent, const QString &t_name);
    };
}