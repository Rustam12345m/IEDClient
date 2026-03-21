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

#include <QObject>

#include "server_identify.hpp"
#include "data_model.hpp"
#include "fs_model.hpp"

namespace Core
{
    /**
     * @brief Representation of all data for this IED
     */
    class IED
    {
    public:
        using ptr = QSharedPointer< IED >;

        IED(QObject *parent=nullptr) {
            m_model = DataModel::ptr::create("");
            m_fsModel = FS_Model::ptr::create();
        }
        ~IED() = default;

        DataModel& model() { return *m_model; }
        FS_Model&  fs() { return *m_fsModel; }

        auto getDataModel() { return m_model; }
        auto getFSModel() { return m_fsModel; }

        const DevServIdentity& identify() { return m_servIdent; }
        void setIdentify(const DevServIdentity &ident) {
            m_servIdent = ident;
        }

        void setModel(DataModel::ptr model) {
            m_model = model;
        }
        void setFSModel(FS_Model::ptr fsModel) {
            m_fsModel = fsModel;
        }

    private:
        DataModel::ptr  m_model;
        FS_Model::ptr   m_fsModel;
        DevServIdentity m_servIdent;
    };
}
