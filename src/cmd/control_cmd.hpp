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

#include "cmd_interface.hpp"
#include "interface/control_types.hpp"

namespace Cmd
{
    /**
     * @brief Queries control model and value type for a controllable object
     */
    class GetControlInfo_Cmd : public CmdInterface
    {
        Q_OBJECT
    public:
        GetControlInfo_Cmd(const QString &objRef)
            : m_objRef{objRef}
        {}
        ~GetControlInfo_Cmd() override = default;

        void execute(Cmd::Interface::IEC61850_API::ptr api) override;

        static auto create(const QString &objRef) {
            return QSharedPointer<GetControlInfo_Cmd>::create(objRef);
        }

    signals:
        void sigControlInfo(QString objRef, int ctlModel, int ctlValType);

    private:
        QString m_objRef;
    };

    /**
     * @brief Executes Operate / Select / Cancel on a controllable object
     */
    class ControlOperate_Cmd : public CmdInterface
    {
        Q_OBJECT
    public:
        enum class Action { Operate, Select, Cancel };

        ControlOperate_Cmd(const QString &objRef, Action action,
                           Cmd::Interface::CtlModel model,
                           Cmd::Interface::CtlValType valType,
                           const QVariant &value)
            : m_objRef{objRef}, m_action{action},
              m_model{model}, m_valType{valType}, m_value{value}
        {}
        ~ControlOperate_Cmd() override = default;

        void execute(Cmd::Interface::IEC61850_API::ptr api) override;

        static auto create(const QString &objRef, Action action,
                           Cmd::Interface::CtlModel model,
                           Cmd::Interface::CtlValType valType,
                           const QVariant &value) {
            return QSharedPointer<ControlOperate_Cmd>::create(
                objRef, action, model, valType, value);
        }

    signals:
        void sigControlResult(QString objRef, bool success, QString message);

    private:
        QString m_objRef;
        Action  m_action;
        Cmd::Interface::CtlModel   m_model;
        Cmd::Interface::CtlValType m_valType;
        QVariant m_value;
    };
}
