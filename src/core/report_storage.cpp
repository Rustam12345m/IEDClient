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

#include "report_storage.hpp"

namespace Core
{
    ReportStorage::ReportStorage(QObject *parent)
        : QObject(parent)
    {
    }

    void ReportStorage::setMemberNames(const QStringList &names)
    {
        QMutexLocker locker(&m_lock);
        m_memberNames = names;
    }

    QString ReportStorage::memberName(int index) const
    {
        QMutexLocker locker(&m_lock);
        if (index >= 0 && index < m_memberNames.size()) {
            return m_memberNames[index];
        }
        return QString::number(index);
    }

    void ReportStorage::setDataSetRef(const QString &ref)
    {
        QMutexLocker locker(&m_lock);
        m_dataSetRef = ref;
    }

    QString ReportStorage::dataSetRef() const
    {
        QMutexLocker locker(&m_lock);
        return m_dataSetRef;
    }

    void ReportStorage::addReport(ReceivedReport::ptr report)
    {
        {
            QMutexLocker locker(&m_lock);
            int pos = (m_head + m_count) % Capacity;
            m_buffer[pos] = report;

            if (m_count < Capacity) {
                ++m_count;
            } else {
                m_head = (m_head + 1) % Capacity;
            }
        }

        emit sigReportReceived();
    }

    ReceivedReport::ptr ReportStorage::getReport(int index) const
    {
        QMutexLocker locker(&m_lock);
        if (index < 0 || index >= m_count) {
            return nullptr;
        }
        int pos = (m_head + index) % Capacity;
        return m_buffer[pos];
    }

    int ReportStorage::count() const
    {
        QMutexLocker locker(&m_lock);
        return m_count;
    }

    void ReportStorage::clear()
    {
        QMutexLocker locker(&m_lock);
        m_buffer.fill(nullptr);
        m_head = 0;
        m_count = 0;
    }
}
