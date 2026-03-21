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

#include "dataset.hpp"

namespace Core
{
    /**
     * @brief Representation of a Report Control Block (BRCB or URCB)
     */
    class ReportBlock : public ModelItem
    {
    public:
        using ptr = QSharedPointer< ReportBlock >;

        ReportBlock(ModelItem *parent, const QString &name, const QString &lnRef, bool isBuffered)
            : ModelItem(parent, name), m_lnRef{lnRef}, m_isBuffered{isBuffered}
        {}

        const QString & lnRef()     const { return m_lnRef; }
        bool     isBuffered()       const { return m_isBuffered; }
        bool     rptEna()           const { return m_rptEna; }
        bool     resv()             const { return m_resv; }
        const QString & rptId()     const { return m_rptId; }
        const QString & dsRef()     const { return m_dsRef; }
        int      trgOps()           const { return m_trgOps; }
        uint32_t confRev()          const { return m_confRev; }
        uint32_t bufTm()            const { return m_bufTm; }
        uint32_t intgPd()           const { return m_intgPd; }
        const QString & owner()     const { return m_owner; }

        void setRptEna(bool v)             { m_rptEna  = v; }
        void setResv(bool v)               { m_resv    = v; }
        void setRptId(const QString &v)    { m_rptId   = v; }
        void setDsRef(const QString &v)    { m_dsRef   = v; }
        void setTrgOps(int v)              { m_trgOps  = v; }
        void setConfRev(uint32_t v)        { m_confRev = v; }
        void setBufTm(uint32_t v)          { m_bufTm   = v; }
        void setIntgPd(uint32_t v)         { m_intgPd  = v; }
        void setOwner(const QString &v)    { m_owner   = v; }

    private:
        QString  m_lnRef;
        bool     m_isBuffered = false;
        bool     m_rptEna     = false;
        bool     m_resv       = false;
        QString  m_rptId;
        QString  m_dsRef;
        int      m_trgOps  = 0;
        uint32_t m_confRev = 0;
        uint32_t m_bufTm   = 0;
        uint32_t m_intgPd  = 0;
        QString  m_owner;
    };
}
