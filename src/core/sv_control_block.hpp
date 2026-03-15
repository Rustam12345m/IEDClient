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

        SV_ControlBlock(ModelItem *t_parent, const QString &t_name, const QString &t_lnRef, bool t_isMulticast)
            : QObject(nullptr), ModelItem(t_parent, t_name), m_lnRef{t_lnRef}, m_isMulticast{t_isMulticast}
        {}

        const QString & lnRef()     const { return m_lnRef; }
        bool     isMulticast()      const { return m_isMulticast; }
        bool     svEna()            const { return m_svEna; }
        const QString & svId()      const { return m_svId; }
        const QString & datSet()    const { return m_datSet; }
        uint32_t confRev()          const { return m_confRev; }
        uint32_t smpRate()          const { return m_smpRate; }
        int      noASDU()           const { return m_noASDU; }

        void setSvEna(bool t_v)              { m_svEna    = t_v; }
        void setSvId(const QString &t_v)     { m_svId     = t_v; }
        void setDatSet(const QString &t_v)   { m_datSet   = t_v; }
        void setConfRev(uint32_t t_v)        { m_confRev  = t_v; }
        void setSmpRate(uint32_t t_v)        { m_smpRate  = t_v; }
        void setNoASDU(int t_v)              { m_noASDU   = t_v; }

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
