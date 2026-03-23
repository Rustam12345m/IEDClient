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

#include <QString>
#include <cstdint>

#include "control_types.hpp"
#include "core/report_control_block.hpp"
#include "core/report_storage.hpp"
#include "core/goose_control_block.hpp"
#include "core/sv_control_block.hpp"

namespace Cmd::Interface
{
    class IED_ControlAPI
    {
    public:
        virtual ~IED_ControlAPI() {}

        virtual bool setRCBValues(const QString &rcbRef, bool enable,
                                  int trgOps, uint32_t bufTm, uint32_t intgPd,
                                  const QString &rptId, const QString &datSet) = 0;

        virtual bool refreshRCBValues(Core::ReportBlock::ptr rcb) = 0;

        // GOOSE Control Block
        virtual bool refreshGOOSEValues(Core::GooseControlBlock::ptr gocb) = 0;
        virtual QString setGOOSEEnable(const QString &gocbRef, bool enable) = 0;

        // SV Control Block
        virtual bool refreshSVValues(Core::SV_ControlBlock::ptr svcb) = 0;
        virtual QString setSVEnable(const QString &svcbRef, bool enable) = 0;

        virtual bool installReportHandler(const QString &rcbRef,
                                          const QString &rptId,
                                          Core::ReportStorage *storage) = 0;
        virtual void uninstallReportHandler(const QString &rcbRef) = 0;

        // Direct Control / SBO
        virtual ControlInfo getControlInfo(const QString &objRef) = 0;
        virtual bool controlOperate(const QString &objRef, CtlModel model,
                                    CtlValType valType, const QVariant &value) = 0;
        virtual bool controlSelect(const QString &objRef, CtlModel model,
                                   CtlValType valType, const QVariant &value) = 0;
        virtual bool controlCancel(const QString &objRef) = 0;

        // Control flags
        virtual void setTestMode(bool test) = 0;
        virtual void setInterlockCheck(bool check) = 0;
        virtual void setSynchroCheck(bool check) = 0;
    };
}