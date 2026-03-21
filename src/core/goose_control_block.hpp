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

#include <cstdint>

#include <QObject>

#include "dataset.hpp"

namespace Core
{
    /**
     * @brief Representation of a GOOSE Control Block (GoCB)
     */
    class GooseControlBlock : public QObject, public ModelItem
    {
        Q_OBJECT
    public:
        using ptr = QSharedPointer< GooseControlBlock >;

        GooseControlBlock(ModelItem *parent, const QString &name, const QString &lnRef)
            : QObject(nullptr), ModelItem(parent, name), m_lnRef{lnRef}
        {}

        const QString & lnRef()     const { return m_lnRef; }
        bool     goEna()            const { return m_goEna; }
        const QString & goId()      const { return m_goId; }
        const QString & datSet()    const { return m_datSet; }
        uint32_t confRev()          const { return m_confRev; }
        uint32_t minTime()          const { return m_minTime; }
        uint32_t maxTime()          const { return m_maxTime; }
        uint16_t appId()            const { return m_appId; }
        uint16_t vlanId()           const { return m_vlanId; }
        uint8_t  vlanPriority()     const { return m_vlanPriority; }

        void setGoEna(bool v)              { m_goEna       = v; }
        void setGoId(const QString &v)     { m_goId        = v; }
        void setDatSet(const QString &v)   { m_datSet      = v; }
        void setConfRev(uint32_t v)        { m_confRev     = v; }
        void setMinTime(uint32_t v)        { m_minTime     = v; }
        void setMaxTime(uint32_t v)        { m_maxTime     = v; }
        void setAppId(uint16_t v)          { m_appId       = v; }
        void setVlanId(uint16_t v)         { m_vlanId      = v; }
        void setVlanPriority(uint8_t v)    { m_vlanPriority = v; }

    signals:
        void sigUpdated();

    private:
        QString  m_lnRef;
        bool     m_goEna         = false;
        QString  m_goId;
        QString  m_datSet;
        uint32_t m_confRev       = 0;
        uint32_t m_minTime       = 0;
        uint32_t m_maxTime       = 0;
        uint16_t m_appId         = 0;
        uint16_t m_vlanId        = 0;
        uint8_t  m_vlanPriority  = 0;
    };
}
