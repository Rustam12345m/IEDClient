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
#include <QSharedPointer>
#include <QString>

namespace Core
{
    class SGCB : public QObject
    {
        Q_OBJECT
    public:
        using ptr = QSharedPointer<SGCB>;

        SGCB(const QString &ldRef, uint8_t numOfSG, uint8_t actSG)
            : QObject(nullptr), m_ldRef{ldRef}, m_numOfSG{numOfSG}, m_actSG{actSG}
        {}

        QString  ldRef()    const { return m_ldRef; }
        QString  sgcbRef()  const { return m_ldRef + "/LLN0.SGCB"; }
        uint8_t  numOfSG()  const { return m_numOfSG; }
        uint8_t  actSG()    const { return m_actSG; }
        uint8_t  editSG()   const { return m_editSG; }
        bool     cnfEdit()  const { return m_cnfEdit; }
        uint64_t lActTm()   const { return m_lActTm; }
        uint16_t resvTms()  const { return m_resvTms; }

        void setActSG(uint8_t v)    { m_actSG   = v; }
        void setEditSG(uint8_t v)   { m_editSG  = v; }
        void setCnfEdit(bool v)     { m_cnfEdit = v; }
        void setLActTm(uint64_t v)  { m_lActTm  = v; }
        void setResvTms(uint16_t v) { m_resvTms = v; }

    private:
        QString  m_ldRef;
        uint8_t  m_numOfSG;
        uint8_t  m_actSG;
        uint8_t  m_editSG  = 0;
        bool     m_cnfEdit = false;
        uint64_t m_lActTm  = 0;
        uint16_t m_resvTms = 0;
    };
}
