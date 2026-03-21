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
#include "core/report_control_block.hpp"
#include "core/report_storage.hpp"

namespace Cmd
{
    class SetRCBValues_Cmd : public CmdInterface
    {
        Q_OBJECT
    public:
        SetRCBValues_Cmd(Core::ReportBlock::ptr rcb, bool enable,
                         int trgOps, uint32_t bufTm, uint32_t intgPd,
                         const QString &rptId, const QString &datSet,
                         Core::ReportStorage *storage = nullptr)
            : m_rcb{rcb}, m_enable{enable},
              m_trgOps{trgOps}, m_bufTm{bufTm}, m_intgPd{intgPd},
              m_rptId{rptId}, m_datSet{datSet}, m_storage{storage}
        {}
        ~SetRCBValues_Cmd() override = default;

        void execute(Cmd::Interface::IEC61850_API::ptr api) override;

        static auto create(Core::ReportBlock::ptr rcb, bool enable,
                           int trgOps, uint32_t bufTm, uint32_t intgPd,
                           const QString &rptId, const QString &datSet,
                           Core::ReportStorage *storage = nullptr) {
            return QSharedPointer<SetRCBValues_Cmd>::create(
                rcb, enable, trgOps, bufTm, intgPd, rptId, datSet, storage);
        }

    private:
        Core::ReportBlock::ptr m_rcb;
        bool     m_enable;
        int      m_trgOps;
        uint32_t m_bufTm;
        uint32_t m_intgPd;
        QString  m_rptId;
        QString  m_datSet;
        Core::ReportStorage *m_storage = nullptr;
    };
}
