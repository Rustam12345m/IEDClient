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
#include <QMutex>

#include <array>

#include "received_report.hpp"

namespace Core
{
    class ReportStorage : public QObject
    {
        Q_OBJECT
    public:
        static constexpr int Capacity = 100;

        explicit ReportStorage(QObject *t_parent = nullptr);

        void setMemberNames(const QStringList &t_names);
        QString memberName(int t_index) const;

        void setDataSetRef(const QString &t_ref);
        QString dataSetRef() const;

        void addReport(ReceivedReport::ptr t_report);
        ReceivedReport::ptr getReport(int t_index) const;
        int  count() const;
        void clear();

    signals:
        void sigReportReceived();

    private:
        mutable QMutex                                  m_lock;
        std::array<ReceivedReport::ptr, Capacity>       m_buffer{};
        int                                             m_head = 0;
        int                                             m_count = 0;
        QStringList                                     m_memberNames;
        QString                                         m_dataSetRef;
    };
}
