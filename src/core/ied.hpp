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
    /*
     * Representation of all data for this IED
     * */
    class IED
    {
    public:
        IED(QObject *t_parent=nullptr) {
            m_model = QSharedPointer<Core::DataModel>::create("");
		    m_fsModel = QSharedPointer<Core::FS_Model>::create();
        }
        ~IED() = default;

        DataModel&   model() { return *m_model; }
        FS_Model&    fs() { return *m_fsModel; }

        auto getDataModel() { return m_model; }
        auto getFSModel() { return m_fsModel; }

        const DevServIdentity& identify() { return m_servIdent; }
        void setIdentify(const DevServIdentity &t_ident) {
            m_servIdent = t_ident;
        }

        void setModel(QSharedPointer<DataModel> t_model) {
            m_model = t_model;
        }
        void setFSModel(QSharedPointer<FS_Model> t_model) {
            m_fsModel = t_model;
        }

    private:
        QSharedPointer<DataModel>   m_model;
        QSharedPointer<FS_Model>    m_fsModel;
        DevServIdentity             m_servIdent;
    };
    typedef QSharedPointer<IED>     ptrIED;
}