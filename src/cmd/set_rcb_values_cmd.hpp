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

namespace Cmd
{
    class SetRCBValues_Cmd : public CmdInterface
    {
        Q_OBJECT
    public:
        SetRCBValues_Cmd(Core::ReportBlock::ptr t_rcb, bool t_enable,
                         int t_trgOps, uint32_t t_bufTm, uint32_t t_intgPd)
            : m_rcb{t_rcb}, m_enable{t_enable},
              m_trgOps{t_trgOps}, m_bufTm{t_bufTm}, m_intgPd{t_intgPd}
        {}
        ~SetRCBValues_Cmd() override = default;

        void execute(Cmd::Interface::IEC61850_API::ptr t_api) override;

        static auto create(Core::ReportBlock::ptr t_rcb, bool t_enable,
                           int t_trgOps, uint32_t t_bufTm, uint32_t t_intgPd) {
            return QSharedPointer<SetRCBValues_Cmd>::create(t_rcb, t_enable, t_trgOps, t_bufTm, t_intgPd);
        }

    private:
        Core::ReportBlock::ptr m_rcb;
        bool     m_enable;
        int      m_trgOps;
        uint32_t m_bufTm;
        uint32_t m_intgPd;
    };
}
