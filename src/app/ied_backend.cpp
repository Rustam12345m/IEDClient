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
#include "core/data_attribute.hpp"
#include "core/data_object.hpp"

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
        m_watchlistModel = new Models::WatchlistModel(this);

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

        // CommandTermination from network thread → main thread
        connect(m_con.m_api.get(), &Cmd::Interface::IEC61850_API::sigCommandTermination,
                this, &IED_Backend::sigCommandTermination, Qt::QueuedConnection);
    }

    void IED_Backend::updateLDs_Status()
    {
        auto cmd = Cmd::UpdateLDs_StatusCmd::create(m_con.m_ied);
        connect(cmd.get(), &Cmd::UpdateLDs_StatusCmd::sigModelValues,
                this, &IED_Backend::slotUpdateItems, Qt::QueuedConnection);

        putCmdToQueue(cmd);
    }

    void IED_Backend::updateLNs_Status()
    {
        auto cmd = Cmd::UpdateLNs_StatusCmd::create(m_con.m_ied, m_lnsModel->getLogicalDevice());
        connect(cmd.get(), &Cmd::UpdateLNs_StatusCmd::sigModelValues,
                this, &IED_Backend::slotUpdateItems, Qt::QueuedConnection);

        putCmdToQueue(cmd);
    }

    void IED_Backend::updateRCBs_Status()
    {
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
        auto lnode = m_lnStateModel->getCurrectLN();
        if (lnode == nullptr) {
            return;
        }

        auto cmd = Cmd::UpdateLNode_Cmd::create(m_con.m_ied, lnode);
        connect(cmd.get(), &Cmd::UpdateLNode_Cmd::sigModelValues,
                this, &IED_Backend::slotUpdateItems, Qt::QueuedConnection);

        putCmdToQueue(cmd);
    }

    void IED_Backend::updateDS_Values()
    {
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
                [this, buffered](Cmd::CmdEvent ev) {
                    if (ev.m_type == Cmd::FINISH_EVENT) {
                        auto *m = buffered ? m_brcbComModel : m_urcbComModel;
                        m->slotDataUpdated(ev.m_result);
                        emit sigRCBUpdated();
                    }
                }, Qt::QueuedConnection);

        putCmdToQueue(cmd);
    }

    // ── SGCB operations ─────────────────────────────────────────────

    static Core::SGCB::ptr findSGCB(Core::IED::ptr ied, const QString &ldRef)
    {
        for (const auto &sgcb : ied->model().getSGCBList()) {
            if (sgcb->ldRef() == ldRef) return sgcb;
        }
        return nullptr;
    }

    void IED_Backend::setActiveSG(const QString &ldRef, int sg)
    {
        QString ref = ldRef + "/LLN0.SGCB.ActSG";
        auto cmd = Cmd::WriteValue_Cmd::create(ref, "SP", QString::number(sg));
        connect(cmd.get(), &Cmd::WriteValue_Cmd::sigWriteResult,
                this, [this, ldRef](QString, bool ok, QString msg) {
                    if (ok) {
                        auto sgcb = findSGCB(m_con.m_ied, ldRef);
                        if (sgcb) {
                            auto refresh = Cmd::RefreshSGCB_Cmd::create(sgcb);
                            connect(refresh.get(), &Cmd::RefreshSGCB_Cmd::sigSGCBRefreshed,
                                    this, &IED_Backend::sigSGCBUpdated, Qt::QueuedConnection);
                            putCmdToQueue(refresh);
                        }
                    }
                    emit sigWriteResult(ldRef, ok, msg);
                }, Qt::QueuedConnection);
        putCmdToQueue(cmd);
    }

    void IED_Backend::selectEditSG(const QString &ldRef, int sg)
    {
        QString ref = ldRef + "/LLN0.SGCB.EditSG";
        auto cmd = Cmd::WriteValue_Cmd::create(ref, "SP", QString::number(sg));
        connect(cmd.get(), &Cmd::WriteValue_Cmd::sigWriteResult,
                this, [this, ldRef](QString, bool ok, QString msg) {
                    if (ok) {
                        auto sgcb = findSGCB(m_con.m_ied, ldRef);
                        if (sgcb) {
                            auto refresh = Cmd::RefreshSGCB_Cmd::create(sgcb);
                            connect(refresh.get(), &Cmd::RefreshSGCB_Cmd::sigSGCBRefreshed,
                                    this, &IED_Backend::sigSGCBUpdated, Qt::QueuedConnection);
                            putCmdToQueue(refresh);
                        }
                    }
                    emit sigWriteResult(ldRef, ok, msg);
                }, Qt::QueuedConnection);
        putCmdToQueue(cmd);
    }

    void IED_Backend::confirmEditSG(const QString &ldRef)
    {
        QString ref = ldRef + "/LLN0.SGCB.CnfEdit";
        auto cmd = Cmd::WriteValue_Cmd::create(ref, "SP", "True");
        connect(cmd.get(), &Cmd::WriteValue_Cmd::sigWriteResult,
                this, [this, ldRef](QString, bool ok, QString msg) {
                    if (ok) {
                        auto sgcb = findSGCB(m_con.m_ied, ldRef);
                        if (sgcb) {
                            auto refresh = Cmd::RefreshSGCB_Cmd::create(sgcb);
                            connect(refresh.get(), &Cmd::RefreshSGCB_Cmd::sigSGCBRefreshed,
                                    this, &IED_Backend::sigSGCBUpdated, Qt::QueuedConnection);
                            putCmdToQueue(refresh);
                        }
                    }
                    emit sigWriteResult(ldRef, ok, msg);
                }, Qt::QueuedConnection);
        putCmdToQueue(cmd);
    }

    void IED_Backend::cancelEditSG(const QString &ldRef)
    {
        QString ref = ldRef + "/LLN0.SGCB.EditSG";
        auto cmd = Cmd::WriteValue_Cmd::create(ref, "SP", "0");
        connect(cmd.get(), &Cmd::WriteValue_Cmd::sigWriteResult,
                this, [this, ldRef](QString, bool ok, QString msg) {
                    if (ok) {
                        auto sgcb = findSGCB(m_con.m_ied, ldRef);
                        if (sgcb) {
                            auto refresh = Cmd::RefreshSGCB_Cmd::create(sgcb);
                            connect(refresh.get(), &Cmd::RefreshSGCB_Cmd::sigSGCBRefreshed,
                                    this, &IED_Backend::sigSGCBUpdated, Qt::QueuedConnection);
                            putCmdToQueue(refresh);
                        }
                    }
                    emit sigWriteResult(ldRef, ok, msg);
                }, Qt::QueuedConnection);
        putCmdToQueue(cmd);
    }

    QVariantMap IED_Backend::getSGCBInfo(const QString &ldRef)
    {
        QVariantMap info;
        auto sgcb = findSGCB(m_con.m_ied, ldRef);
        if (!sgcb) return info;

        info["numOfSG"] = sgcb->numOfSG();
        info["actSG"]   = sgcb->actSG();
        info["editSG"]  = sgcb->editSG();
        info["cnfEdit"] = sgcb->cnfEdit();
        info["lActTm"]  = (qulonglong)sgcb->lActTm();
        info["resvTms"] = sgcb->resvTms();
        return info;
    }

    bool IED_Backend::hasSGCB(const QString &ldRef)
    {
        return findSGCB(m_con.m_ied, ldRef) != nullptr;
    }

    QString IED_Backend::getCurrentSettingsLDRef()
    {
        auto ln = m_lnSettingsModel->getCurrectLN();
        if (!ln || !ln->getParent()) return {};
        return ln->getParent()->getName();
    }

    // ── Write operations ─────────────────────────────────────────────

    void IED_Backend::writeValue(const QString &ref, const QString &fc, const QString &value)
    {
        auto cmd = Cmd::WriteValue_Cmd::create(ref, fc, value);
        connect(cmd.get(), &Cmd::WriteValue_Cmd::sigWriteResult,
                this, [this](QString r, bool ok, QString msg) {
                    emit sigWriteResult(r, ok, msg);
                    if (ok) updateLN_TreeValues();
                }, Qt::QueuedConnection);
        putCmdToQueue(cmd);
    }

    QString IED_Backend::getSettingsItemRef(int proxyRow)
    {
        QModelIndex proxyIdx = m_sortSettingsModel->index(proxyRow, 0);
        QModelIndex sourceIdx = m_sortSettingsModel->mapToSource(proxyIdx);
        int sourceRow = sourceIdx.row();

        auto matrix = m_lnSettingsModel->getCurrectLN()
                          ? m_lnSettingsModel->getCurrectLN()->getSettingsMatrix()
                          : nullptr;

        if (!matrix || sourceRow < 0 || sourceRow >= matrix->size()) {
            return {};
        }

        auto &rows = matrix->getRows();
        auto item = rows[sourceRow].valueItem();
        return item ? item->getReference() : QString();
    }

    QString IED_Backend::getSettingsItemFC(int proxyRow)
    {
        QModelIndex proxyIdx = m_sortSettingsModel->index(proxyRow, 0);
        QModelIndex sourceIdx = m_sortSettingsModel->mapToSource(proxyIdx);
        int sourceRow = sourceIdx.row();

        auto matrix = m_lnSettingsModel->getCurrectLN()
                          ? m_lnSettingsModel->getCurrectLN()->getSettingsMatrix()
                          : nullptr;

        if (!matrix || sourceRow < 0 || sourceRow >= matrix->size()) {
            return {};
        }

        return matrix->fc(sourceRow);
    }

    QString IED_Backend::getSettingsItemValue(int proxyRow)
    {
        QModelIndex proxyIdx = m_sortSettingsModel->index(proxyRow, 0);
        QModelIndex sourceIdx = m_sortSettingsModel->mapToSource(proxyIdx);
        int sourceRow = sourceIdx.row();

        auto matrix = m_lnSettingsModel->getCurrectLN()
                          ? m_lnSettingsModel->getCurrectLN()->getSettingsMatrix()
                          : nullptr;

        if (!matrix || sourceRow < 0 || sourceRow >= matrix->size()) {
            return {};
        }

        return matrix->value(sourceRow);
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
                this, [this](QString r, bool ok, QString msg) {
                    emit sigControlResult(r, ok, msg);
                    if (ok) updateLN_TreeValues();
                }, Qt::QueuedConnection);
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
                this, [this](QString r, bool ok, QString msg) {
                    emit sigControlResult(r, ok, msg);
                    if (ok) updateLN_TreeValues();
                }, Qt::QueuedConnection);
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
                this, [this](QString r, bool ok, QString msg) {
                    emit sigControlResult(r, ok, msg);
                    if (ok) updateLN_TreeValues();
                }, Qt::QueuedConnection);
        putCmdToQueue(cmd);
    }

    void IED_Backend::setTestMode(bool test)
    {
        m_con.m_api->control().setTestMode(test);
    }

    void IED_Backend::setInterlockCheck(bool check)
    {
        m_con.m_api->control().setInterlockCheck(check);
    }

    void IED_Backend::setSynchroCheck(bool check)
    {
        m_con.m_api->control().setSynchroCheck(check);
    }

    void IED_Backend::setOrigin(const QString &orIdent, int orCat)
    {
        m_con.m_api->control().setOrigin(orIdent, orCat);
    }

    // ── Watchlist ─────────────────────────────────────────────────

    void IED_Backend::addToWatchlist(const QString &ref, const QString &fc, const QString &value)
    {
        m_watchlistModel->addItem(ref, fc, value);
    }

    void IED_Backend::removeFromWatchlist(int row)
    {
        m_watchlistModel->removeItem(row);
    }

    void IED_Backend::clearWatchlist()
    {
        m_watchlistModel->clear();
    }

    void IED_Backend::loadWatchlist(const WatchlistRefs &items)
    {
        QList<Models::WatchlistModel::WatchItem> wItems;
        for (const auto &[ref, fc] : items) {
            wItems.append({ref, fc, " - "});
        }
        m_watchlistModel->setItems(wItems);
    }

    static bool isDataObjectOrBelow(Core::ModelItem *item)
    {
        for (auto *p = item; p; p = p->getParent()) {
            if (dynamic_cast<Core::DataObject*>(p))
                return true;
        }
        return false;
    }

    static QString findFC(Core::ModelItem *item)
    {
        for (auto *p = item; p; p = p->getParent()) {
            auto *da = dynamic_cast<Core::DataAttribute*>(p);
            if (da) return da->fcStr();
        }
        return {};
    }

    void IED_Backend::addTreeItemToWatchlist(const QModelIndex &proxyIndex)
    {
        auto srcIdx = m_iedTreeFilter->mapToSource(proxyIndex);
        if (!srcIdx.isValid()) return;

        auto *item = static_cast<Core::ModelItem*>(srcIdx.internalPointer());
        if (!item || !isDataObjectOrBelow(item)) return;

        if (item->getItemCount() == 0) {
            m_watchlistModel->addItem(item->getReference(), findFC(item), item->getValue());
        } else {
            std::function<void(Core::ModelItem*)> addLeaves = [&](Core::ModelItem *node) {
                if (node->getItemCount() == 0) {
                    m_watchlistModel->addItem(node->getReference(), findFC(node), node->getValue());
                } else {
                    for (auto &child : node->getItemList()) {
                        addLeaves(child.get());
                    }
                }
            };
            addLeaves(item);
        }
    }

    void IED_Backend::updateWatchlistValues()
    {
        auto items = m_watchlistModel->getItems();
        if (items.isEmpty()) return;

        QStringList refs, fcs;
        for (const auto &item : items) {
            refs.append(item.ref);
            fcs.append(item.fc);
        }

        auto cmd = Cmd::UpdateWatchlist_Cmd::create(refs, fcs);
        connect(cmd.get(), &Cmd::UpdateWatchlist_Cmd::sigValuesRead,
                this, [this](QVariantList results) {
                    m_watchlistModel->updateValues(results);
                }, Qt::QueuedConnection);

        putCmdToQueue(cmd);
    }

    // ── Status bar ────────────────────────────────────────────────

    QString IED_Backend::ldsPageStatus()
    {
        if (!m_con.m_ied) return QString();
        return "IED: " + m_con.m_ied->model().getName();
    }

    QString IED_Backend::lnsPageStatus()
    {
        return m_lnAllModel->getSelectedReference();
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