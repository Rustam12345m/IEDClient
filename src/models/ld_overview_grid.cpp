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

#include "ld_overview_grid.hpp"
#include "iec61850_enums.hpp"

namespace App::Models
{
    namespace
    {
        inline int getInt(Core::ptrModelItem t_item)
        {
            return t_item ? t_item->getValue().toInt() : -1;
        }
    }

    LD_OverviewGrid::LD_OverviewGrid(QObject *t_parent, Core::ptrIED t_ied)
        : QAbstractListModel(t_parent), m_ied(t_ied)
    {
    }

    void LD_OverviewGrid::setActiveIED(Core::ptrIED t_ied)
    {
        beginResetModel();

        connectToUpdates(m_ied->model(), false);
        m_ied = t_ied;
        connectToUpdates(m_ied->model(), true);

        endResetModel();
    }

    QHash<int, QByteArray> LD_OverviewGrid::roleNames() const
    {
        return { { LD_ROLE_VALUES, "ld_object" } };
    }

    int LD_OverviewGrid::rowCount(const QModelIndex &t_index) const
    {
        return m_ied->model().getItemCount();
    }

    QVariant LD_OverviewGrid::data(const QModelIndex &t_index, int t_role) const
    {
        auto ld = m_ied->model().getLogicalDevice(t_index.row());
        if (ld) {
            int mod = -1, beh = -1, health = -1, sim = -1, blk = -1;
            auto ln0 = ld->lln0();
            if (ln0) {
                mod = getInt(ln0->getModItem());
                beh = getInt(ln0->getBehItem());
                health = getInt(ln0->getHealthItem());
                sim = -1;
                blk = -1;
            }

            QVariantMap ldObj;
            ldObj["name"] = ld->getName();
            ldObj["mod"] = IEC_EnumUserInfo::mod(mod);
            ldObj["beh"] = IEC_EnumUserInfo::beh(beh);
            ldObj["health"] = IEC_EnumUserInfo::health(health);
            ldObj["sim"] = IEC_EnumUserInfo::sim(sim);
            ldObj["blk"] = IEC_EnumUserInfo::blk(blk);
            return ldObj;
        }
        return QVariant(" - ");
    }

    void LD_OverviewGrid::setSelectedLD(int t_ld)
    {
        //qDebug() << "LD_OverviewGrid: Selected LD = " << t_ld;
        emit sigLDSelected(t_ld);
    }

    void LD_OverviewGrid::connectToUpdates(Core::DataModel &t_model, bool t_con)
    {
        for (size_t i=0;i<t_model.getItemCount();i++) {
            auto ld = t_model.getItem< Core::LogicalDevice >(i);

            if (ld->lln0()) {
                if (t_con) {
                    connect(ld->lln0().get(), &Core::LogicalNode::sigDataObjectUpdated,
                            this, &LD_OverviewGrid::slotDataUpdated);
                } else {
                    disconnect(ld->lln0().get(), &Core::LogicalNode::sigDataObjectUpdated,
                            this, &LD_OverviewGrid::slotDataUpdated);
                }
            }
        }
    }

    void LD_OverviewGrid::slotDataUpdated(Core::ptrModelItemList t_nodes)
    {
        // qDebug() << "LD_OverviewGrid: Data updated";

        beginResetModel();
        endResetModel();
    }
}