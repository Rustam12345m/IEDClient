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

#include <QSet>

#include "cmd/interface/ied_control_api.hpp"

struct sControlObjectClient;

namespace Libiec61850
{
    class ApiAdapter;

    class IED_ControlAPI_Impl : public Cmd::Interface::IED_ControlAPI
    {
    public:
        IED_ControlAPI_Impl(ApiAdapter &api) : m_api(api) {}
        ~IED_ControlAPI_Impl() override;

        bool setRCBValues(const QString &rcbRef, bool enable,
                          int trgOps, uint32_t bufTm, uint32_t intgPd,
                          const QString &rptId, const QString &datSet) override;

        bool refreshRCBValues(Core::ReportBlock::ptr rcb) override;

        // GOOSE Control Block
        bool refreshGOOSEValues(Core::GooseControlBlock::ptr gocb) override;
        QString setGOOSEEnable(const QString &gocbRef, bool enable) override;

        // SV Control Block
        bool refreshSVValues(Core::SV_ControlBlock::ptr svcb) override;
        QString setSVEnable(const QString &svcbRef, bool enable) override;

        bool installReportHandler(const QString &rcbRef,
                                  const QString &rptId,
                                  Core::ReportStorage *storage) override;
        void uninstallReportHandler(const QString &rcbRef) override;
        void uninstallAllHandlers();

        // Direct Control / SBO
        Cmd::Interface::ControlInfo getControlInfo(const QString &objRef) override;
        bool controlOperate(const QString &objRef, Cmd::Interface::CtlModel model,
                            Cmd::Interface::CtlValType valType, const QVariant &value) override;
        bool controlSelect(const QString &objRef, Cmd::Interface::CtlModel model,
                           Cmd::Interface::CtlValType valType, const QVariant &value) override;
        bool controlCancel(const QString &objRef) override;

        // Control flags
        void setTestMode(bool test) override;
        void setInterlockCheck(bool check) override;
        void setSynchroCheck(bool check) override;

    private:
        static void staticReportCallback(void *param, void *report);
        static void commandTerminationHandler(void *param, sControlObjectClient *client);

        sControlObjectClient* getOrCreateClient(const QString &objRef);
        void destroyActiveClient();
        void applyControlFlags(sControlObjectClient *client);

        ApiAdapter& m_api;
        QSet<QString> m_activeHandlers;

        // Persistent control client for SBO sequence + CommandTermination
        sControlObjectClient* m_activeClient = nullptr;
        QString m_activeClientRef;

        // Control flags
        bool m_testMode = false;
        bool m_interlockCheck = false;
        bool m_synchroCheck = false;
    };
};
