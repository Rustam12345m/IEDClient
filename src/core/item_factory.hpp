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

    /**
     * @brief Factory for all classes inherit from Item
     */
    class ItemFactory
    {
    public:
        // Object Tree <- base node for IED Data Object model
        static QSharedPointer< ModelItem > createModel(const QString &name);

        // Logical Device
        static QSharedPointer< ModelItem > createLD(ModelItem *parent, const QString &name);

        // Logical Node
        static QSharedPointer< ModelItem > createLN(ModelItem *parent, const QString &name);

        // Data Object
        static QSharedPointer< ModelItem > createDO(ModelItem *parent, const QString &name);

        // Data Attribute
        static QSharedPointer< ModelItem > createDA(ModelItem *parent, const QString &name,
                                                    const QString &fc);

        // Sub Attribute
        static QSharedPointer< ModelItem > createSA(ModelItem *parent, const QString &name);
    };
}
