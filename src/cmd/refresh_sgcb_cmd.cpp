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

#include "refresh_sgcb_cmd.hpp"

namespace Cmd
{
    void RefreshSGCB_Cmd::execute(Cmd::Interface::IEC61850_API::ptr api)
    {
        if (!m_sgcb) return;

        QString sgcbRef = m_sgcb->sgcbRef();

        // Read SGCB structure via the state API readValuesByRef
        QStringList refs = {sgcbRef + ".NumOfSG", sgcbRef + ".ActSG",
                            sgcbRef + ".EditSG",  sgcbRef + ".CnfEdit",
                            sgcbRef + ".LActTm",  sgcbRef + ".ResvTms"};
        QStringList fcs  = {"SP", "SP", "SP", "SP", "SP", "SP"};

        QVariantList results = api->state().readValuesByRef(refs, fcs);

        if (results.size() >= 6) {
            auto val = [&](int i) { return results[i].toMap()["value"].toString(); };
            m_sgcb->setActSG(val(1).toUInt());
            m_sgcb->setEditSG(val(2).toUInt());
            m_sgcb->setCnfEdit(val(3).compare("True", Qt::CaseInsensitive) == 0);
            m_sgcb->setResvTms(val(5).toUInt());
        }

        emit sigSGCBRefreshed(m_sgcb->ldRef());
        emit sigCmdEvent(CmdEvent::FinishEvent(sgcbRef, "SGCB refreshed", true));
    }
}
