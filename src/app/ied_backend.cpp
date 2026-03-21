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

#include "ied_backend.hpp"
#include "cmd/set_rcb_values_cmd.hpp"
#include "core/dataset.hpp"

#include <QCoreApplication>

namespace App
{
    IED_Backend::IED_Backend(IEDConContainer &con, EventStorage &ev)
        : BackendInterface(con, ev)
    {
        m_ldsModel = new Models::LD_OverviewGrid(this, m_con.m_ied);
        m_ldPropModel = new Models::LD_PropertiesTable(this, m_con.m_ied);
        m_lnsModel = new Models::LN_OverviewTable(this, m_con.m_ied);
        m_lnAllModel = new Models::LN_AllOverviewTable(this, m_con.m_ied);
        m_lnStateModel    = new Models::LN_SignalTable(this, m_con.m_ied, Models::LN_SignalTable::MatrixType::State);
        m_lnControlsModel = new Models::LN_SignalTable(this, m_con.m_ied, Models::LN_SignalTable::MatrixType::Controls);
        m_lnSettingsModel = new Models::LN_SignalTable(this, m_con.m_ied, Models::LN_SignalTable::MatrixType::Settings);
        m_lnTreeModel = new Models::LN_CommonTree(this, m_con.m_ied);
        m_dsComModel    = new Models::DS_OverviewTable(this, m_con.m_ied);
        m_dsSigModel    = new Models::DS_SignalsTable(this, m_con.m_ied);
        m_brcbComModel  = new Models::RCB_OverviewTable(this, m_con.m_ied, true);
        m_urcbComModel  = new Models::RCB_OverviewTable(this, m_con.m_ied, false);
        m_gooseComModel = new Models::GOOSE_OverviewTable(this, m_con.m_ied);
        m_svComModel    = new Models::SV_OverviewTable(this, m_con.m_ied);
        m_reportsModel  = new Models::ReportsTable(this, m_con.m_ied);
        m_iedTreeModel  = new Models::IED_ModelTree(this, m_con.m_ied);
        m_iedTreeFilter = new Models::TreeFilterProxy(this);
        m_iedTreeFilter->setSourceModel(m_iedTreeModel);

        m_sortDOModel = new Models::SortProxyModel(this);
        m_sortDOModel->setSourceModel(m_lnStateModel);

        m_sortControlsModel = new Models::SortProxyModel(this);
        m_sortControlsModel->setSourceModel(m_lnControlsModel);

        m_sortSettingsModel = new Models::SortProxyModel(this);
        m_sortSettingsModel->setSourceModel(m_lnSettingsModel);

        // Selection process LD -> LN -> DO
        connect(m_ldsModel, &Models::LD_OverviewGrid::sigLDSelected, m_ldPropModel, &Models::LD_PropertiesTable::slotLDSelected);        
        connect(m_ldsModel, &Models::LD_OverviewGrid::sigLDSelected, m_lnsModel, &Models::LN_OverviewTable::slotLDSelected);
        connect(m_ldsModel, &Models::LD_OverviewGrid::sigLDSelected, m_lnAllModel, &Models::LN_AllOverviewTable::slotLDSelected);
        connect(m_lnsModel, &Models::LN_OverviewTable::sigLNSelected, m_lnStateModel,    &Models::LN_SignalTable::slotLNSelected);
        connect(m_lnsModel, &Models::LN_OverviewTable::sigLNSelected, m_lnControlsModel, &Models::LN_SignalTable::slotLNSelected);
        connect(m_lnsModel, &Models::LN_OverviewTable::sigLNSelected, m_lnSettingsModel, &Models::LN_SignalTable::slotLNSelected);
        connect(m_lnsModel, &Models::LN_OverviewTable::sigLNSelected, m_lnTreeModel,     &Models::LN_CommonTree::slotLNSelected);

        connect(m_lnAllModel, &Models::LN_AllOverviewTable::sigLNSelected, m_lnStateModel,    &Models::LN_SignalTable::slotLNSelected);
        connect(m_lnAllModel, &Models::LN_AllOverviewTable::sigLNSelected, m_lnControlsModel, &Models::LN_SignalTable::slotLNSelected);
        connect(m_lnAllModel, &Models::LN_AllOverviewTable::sigLNSelected, m_lnSettingsModel, &Models::LN_SignalTable::slotLNSelected);
        connect(m_lnAllModel, &Models::LN_AllOverviewTable::sigLNSelected, m_lnTreeModel,     &Models::LN_CommonTree::slotLNSelected);

        connect(m_dsComModel, &Models::DS_OverviewTable::sigDSSelected, m_dsSigModel, &Models::DS_SignalsTable::slotDataSetSelected);
        connect(m_brcbComModel, &Models::RCB_OverviewTable::sigRCBSelected, m_reportsModel, &Models::ReportsTable::slotRCBSelected);
        connect(m_urcbComModel, &Models::RCB_OverviewTable::sigRCBSelected, m_reportsModel, &Models::ReportsTable::slotRCBSelected);
    }

    void IED_Backend::updateLDs_Status()
    {
        // qDebug() << "IED_Backend: Update LDs";

        auto cmd = Cmd::UpdateLDs_StatusCmd::create(m_con.m_ied);
        connect(cmd.get(), &Cmd::UpdateLDs_StatusCmd::sigModelValues,
                this, &IED_Backend::slotUpdateItems, Qt::QueuedConnection);

        putCmdToQueue(cmd);
    }

    void IED_Backend::updateLNs_Status()
    {
        // qDebug() << "IED_Backend: Update LNs";

        auto cmd = Cmd::UpdateLNs_StatusCmd::create(m_con.m_ied, m_lnsModel->getLogicalDevice());
        connect(cmd.get(), &Cmd::UpdateLNs_StatusCmd::sigModelValues,
                this, &IED_Backend::slotUpdateItems, Qt::QueuedConnection);

        putCmdToQueue(cmd);
    }

    void IED_Backend::updateRCBs_Status()
    {
        // qDebug() << "IED_Backend: Update RCBs";

        auto cmd = Cmd::UpdateRCBs_Cmd::create(m_con.m_ied);

        connect(cmd.get(), &Cmd::CmdInterface::sigCmdEvent, this,
                [this](Cmd::CmdEvent ev) {
                    if (ev.m_type == Cmd::FINISH_EVENT) {
                        m_brcbComModel->slotDataUpdated(ev.m_result);
                        m_urcbComModel->slotDataUpdated(ev.m_result);
                        emit sigRCBUpdated();
                    }
                }, Qt::QueuedConnection);

        putCmdToQueue(cmd);
    }

    void IED_Backend::updateGOOSE_Status()
    {
        // qDebug() << "IED_Backend: Update GOOSE CBs";

        auto cmd = Cmd::UpdateGooseCBs_Cmd::create(m_con.m_ied);

        connect(cmd.get(), &Cmd::CmdInterface::sigCmdEvent, this,
                [this](Cmd::CmdEvent ev) {
                    if (ev.m_type == Cmd::FINISH_EVENT) {
                        m_gooseComModel->slotDataUpdated(ev.m_result);
                        emit sigGOOSE_SVUpdated();
                    }
                }, Qt::QueuedConnection);

        putCmdToQueue(cmd);
    }

    void IED_Backend::updateSV_Status()
    {
        // qDebug() << "IED_Backend: Update SV CBs";

        auto cmd = Cmd::UpdateSVCBs_Cmd::create(m_con.m_ied);

        connect(cmd.get(), &Cmd::CmdInterface::sigCmdEvent, this,
                [this](Cmd::CmdEvent ev) {
                    if (ev.m_type == Cmd::FINISH_EVENT) {
                        m_svComModel->slotDataUpdated(ev.m_result);
                        emit sigGOOSE_SVUpdated();
                    }
                }, Qt::QueuedConnection);

        putCmdToQueue(cmd);
    }

    void IED_Backend::setGOOSEEnable(int index, bool enable)
    {
        const auto &gocbList = m_con.m_ied->model().getGO_CBList();
        if (index < 0 || index >= gocbList.size()) {
            // qDebug() << "IED_Backend: Invalid GOOSE index" << index;
            return;
        }

        auto gocb = gocbList[index];
        auto cmd = Cmd::SetGooseEnable_Cmd::create(gocb, enable);

        connect(cmd.get(), &Cmd::CmdInterface::sigCmdEvent, this,
                [this](Cmd::CmdEvent ev) {
                    if (ev.m_type == Cmd::FINISH_EVENT) {
                        m_gooseComModel->slotDataUpdated(ev.m_result);
                        emit sigGOOSE_SVUpdated();
                    }
                }, Qt::QueuedConnection);

        putCmdToQueue(cmd);
    }

    void IED_Backend::setSVEnable(int index, bool enable)
    {
        const auto &svcbList = m_con.m_ied->model().getSV_CBList();
        if (index < 0 || index >= svcbList.size()) {
            // qDebug() << "IED_Backend: Invalid SV index" << index;
            return;
        }

        auto svcb = svcbList[index];
        auto cmd = Cmd::SetSVEnable_Cmd::create(svcb, enable);

        connect(cmd.get(), &Cmd::CmdInterface::sigCmdEvent, this,
                [this](Cmd::CmdEvent ev) {
                    if (ev.m_type == Cmd::FINISH_EVENT) {
                        m_svComModel->slotDataUpdated(ev.m_result);
                        emit sigGOOSE_SVUpdated();
                    }
                }, Qt::QueuedConnection);

        putCmdToQueue(cmd);
    }

    void IED_Backend::dumpMmsModel(const QString &ip, int port)
    {
        Cmd::IEDCredentials cred(ip, port, false, "", "");
        auto cmd = Cmd::MmsDumpCmd::create(cred);
        putCmdToQueue(cmd);
    }

    QStringList IED_Backend::getDataSetRefs() const
    {
        QStringList refs;
        for (const auto &ds : m_con.m_ied->model().getDataSetList()) {
            refs.append(ds->ref() + "$" + ds->getName());
        }
        refs.sort();
        return refs;
    }

    void IED_Backend::selectDataSetByRef(const QString &ref)
    {
        const auto &dsList = m_con.m_ied->model().getDataSetList();
        for (int i = 0; i < dsList.size(); i++) {
            QString dsRef = dsList[i]->ref() + "$" + dsList[i]->getName();
            if (dsRef == ref || ref.endsWith(dsList[i]->getName())) {
                m_dsComModel->setSelectedDS(i);
                return;
            }
        }
    }

    void IED_Backend::updateLN_TreeValues()
    {
        // qDebug() << "IED_Backend: Update LN command";

        auto lnode = m_lnStateModel->getCurrectLN();
        if (lnode == nullptr) {
            // qDebug() << "IED_Backend: LNode not found!";
            return;
        }

        auto cmd = Cmd::UpdateLNode_Cmd::create(m_con.m_ied, lnode);
        connect(cmd.get(), &Cmd::UpdateLNode_Cmd::sigModelValues,
                this, &IED_Backend::slotUpdateItems, Qt::QueuedConnection);

        putCmdToQueue(cmd);
    }

    void IED_Backend::updateDS_Values()
    {
        // qDebug() << "IED_Backend: Update DataSet";

        Core::DataSet::ptr ds = m_dsSigModel->getDataSet();
        auto cmd = Cmd::UpdateDataSet_Cmd::create(m_con.m_ied, ds);
        connect(cmd.get(), &Cmd::UpdateDataSet_Cmd::sigModelValues, this,
                [this](Core::ModelStateUpdater::ptr vals) {
                    if (vals) {
                        auto updated = vals->update();
                        m_dsSigModel->slotDataUpdated(updated);
                    }
                }, Qt::QueuedConnection);

        putCmdToQueue(cmd);
    }

    void IED_Backend::setRCBEnable(bool buffered, int index,
                                       bool enable, int trgOps,
                                       int bufTm, int intgPd,
                                       const QString &rptId, const QString &datSet)
    {
        auto *model = buffered ? m_brcbComModel : m_urcbComModel;
        model->setSelectedRCB(index);
        auto rcb = model->getSelectedReportBlock();
        if (!rcb) {
            // qDebug() << "IED_Backend: No RCB selected";
            return;
        }

        Core::ReportStorage *storage = nullptr;
        if (enable) {
            QString prefix = rcb->isBuffered() ? "BR" : "RP";
            QString rcbRef = QString("%1.%2.%3").arg(rcb->lnRef(), prefix, rcb->getName());
            storage = m_con.m_ied->model().getOrCreateReportStorage(rcbRef);

            storage->setDataSetRef(datSet);

            // Pre-populate member names from the DataSet
            for (auto &ds : m_con.m_ied->model().getDataSetList()) {
                // datSet may be full ref like "LD0/LLN0$ds1" — match against
                // both the short name and the composed full reference
                QString fullRef = ds->ref() + "$" + ds->getName();
                if (ds->getName() == datSet || fullRef == datSet
                    || datSet.endsWith(ds->getName())) {
                    QStringList names;
                    for (size_t i = 0; i < ds->getItemCount(); i++) {
                        auto dsItem = ds->getItem<Core::DataSetItem>(i);
                        names.append(dsItem ? dsItem->getName() : QString::number(i));
                    }
                    storage->setMemberNames(names);
                    break;
                }
            }
        }

        auto cmd = Cmd::SetRCBValues_Cmd::create(rcb, enable, trgOps,
                                                  static_cast<uint32_t>(bufTm),
                                                  static_cast<uint32_t>(intgPd),
                                                  rptId, datSet, storage);

        connect(cmd.get(), &Cmd::CmdInterface::sigCmdEvent, this,
                [this, model](Cmd::CmdEvent ev) {
                    if (ev.m_type == Cmd::FINISH_EVENT) {
                        model->slotDataUpdated(ev.m_result);
                        emit sigRCBUpdated();
                    }
                }, Qt::QueuedConnection);

        putCmdToQueue(cmd);
    }

    // ── Control operations ──────────────────────────────────────────

    QString IED_Backend::getControlObjectRef(int proxyRow)
    {
        QModelIndex proxyIdx = m_sortControlsModel->index(proxyRow, 0);
        QModelIndex sourceIdx = m_sortControlsModel->mapToSource(proxyIdx);
        int sourceRow = sourceIdx.row();

        auto matrix = m_lnControlsModel->getCurrectLN()
                          ? m_lnControlsModel->getCurrectLN()->getControlsMatrix()
                          : nullptr;

        if (!matrix || sourceRow < 0 || sourceRow >= matrix->size()) {
            return {};
        }

        auto base = matrix->getRows()[sourceRow].base();
        return base ? base->getReference() : QString();
    }

    void IED_Backend::queryControlInfo(const QString &objRef)
    {
        auto cmd = Cmd::GetControlInfo_Cmd::create(objRef);
        connect(cmd.get(), &Cmd::GetControlInfo_Cmd::sigControlInfo,
                this, &IED_Backend::sigControlInfo, Qt::QueuedConnection);
        putCmdToQueue(cmd);
    }

    void IED_Backend::controlOperate(const QString &objRef, int ctlModel,
                                      int valType, const QVariant &value)
    {
        auto cmd = Cmd::ControlOperate_Cmd::create(
            objRef, Cmd::ControlOperate_Cmd::Action::Operate,
            static_cast<Cmd::Interface::CtlModel>(ctlModel),
            static_cast<Cmd::Interface::CtlValType>(valType),
            value);

        connect(cmd.get(), &Cmd::ControlOperate_Cmd::sigControlResult,
                this, &IED_Backend::sigControlResult, Qt::QueuedConnection);
        putCmdToQueue(cmd);
    }

    void IED_Backend::controlSelect(const QString &objRef, int ctlModel,
                                     int valType, const QVariant &value)
    {
        auto cmd = Cmd::ControlOperate_Cmd::create(
            objRef, Cmd::ControlOperate_Cmd::Action::Select,
            static_cast<Cmd::Interface::CtlModel>(ctlModel),
            static_cast<Cmd::Interface::CtlValType>(valType),
            value);

        connect(cmd.get(), &Cmd::ControlOperate_Cmd::sigControlResult,
                this, &IED_Backend::sigControlResult, Qt::QueuedConnection);
        putCmdToQueue(cmd);
    }

    void IED_Backend::controlCancel(const QString &objRef)
    {
        auto cmd = Cmd::ControlOperate_Cmd::create(
            objRef, Cmd::ControlOperate_Cmd::Action::Cancel,
            Cmd::Interface::CtlModel::StatusOnly,
            Cmd::Interface::CtlValType::Unknown,
            QVariant());

        connect(cmd.get(), &Cmd::ControlOperate_Cmd::sigControlResult,
                this, &IED_Backend::sigControlResult, Qt::QueuedConnection);
        putCmdToQueue(cmd);
    }

    // ── Status bar ────────────────────────────────────────────────

    QString IED_Backend::ldsPageStatus()
    {
        return "IED: " + m_con.m_ied->model().getName();
    }

    QString IED_Backend::lnsPageStatus()
    {
        auto ld = m_lnsModel->getLogicalDevice();
        if (ld) {
            return "LD: " + ld->getName();
        }
        return " - ";
    }

    QString IED_Backend::dsPageStatus()
    {
        return "All DataSets";
    }

    QString IED_Backend::rcbPageStatus()
    {
        return "All Report Control Blocks";
    }

    void IED_Backend::slotConnected(bool done)
    {
        m_ldsModel->setActiveIED(m_con.m_ied);
        m_ldPropModel->setActiveIED(m_con.m_ied);
        m_lnsModel->setActiveIED(m_con.m_ied);
        m_lnAllModel->setActiveIED(m_con.m_ied);
        m_lnStateModel->setActiveIED(m_con.m_ied);
        m_lnControlsModel->setActiveIED(m_con.m_ied);
        m_lnSettingsModel->setActiveIED(m_con.m_ied);
        m_lnTreeModel->setActiveIED(m_con.m_ied);
        m_dsComModel->setActiveIED(m_con.m_ied);
        m_dsSigModel->setActiveIED(m_con.m_ied);
        m_brcbComModel->setActiveIED(m_con.m_ied);
        m_urcbComModel->setActiveIED(m_con.m_ied);
        m_gooseComModel->setActiveIED(m_con.m_ied);
        m_svComModel->setActiveIED(m_con.m_ied);
        m_reportsModel->setActiveIED(m_con.m_ied);
        m_iedTreeModel->setActiveIED(m_con.m_ied);
    }

    void IED_Backend::slotUpdateItems(Core::ModelStateUpdater::ptr vals)
    {
        if (vals) {
            vals->update();
        }
    }
}