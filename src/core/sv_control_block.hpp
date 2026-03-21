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
     * @brief Representation of a Sampled Values Control Block (MSVCB or USVCB)
     */
    class SV_ControlBlock : public QObject, public ModelItem
    {
        Q_OBJECT
    public:
        using ptr = QSharedPointer< SV_ControlBlock >;

        SV_ControlBlock(ModelItem *parent, const QString &name, const QString &lnRef, bool isMulticast)
            : QObject(nullptr), ModelItem(parent, name), m_lnRef{lnRef}, m_isMulticast{isMulticast}
        {}

        const QString & lnRef()     const { return m_lnRef; }
        bool     isMulticast()      const { return m_isMulticast; }
        bool     svEna()            const { return m_svEna; }
        const QString & svId()      const { return m_svId; }
        const QString & datSet()    const { return m_datSet; }
        uint32_t confRev()          const { return m_confRev; }
        uint32_t smpRate()          const { return m_smpRate; }
        int      noASDU()           const { return m_noASDU; }

        void setSvEna(bool v)              { m_svEna    = v; }
        void setSvId(const QString &v)     { m_svId     = v; }
        void setDatSet(const QString &v)   { m_datSet   = v; }
        void setConfRev(uint32_t v)        { m_confRev  = v; }
        void setSmpRate(uint32_t v)        { m_smpRate  = v; }
        void setNoASDU(int v)              { m_noASDU   = v; }

    signals:
        void sigUpdated();

    private:
        QString  m_lnRef;
        bool     m_isMulticast = false;
        bool     m_svEna       = false;
        QString  m_svId;
        QString  m_datSet;
        uint32_t m_confRev     = 0;
        uint32_t m_smpRate     = 0;
        int      m_noASDU      = 0;
    };
}
