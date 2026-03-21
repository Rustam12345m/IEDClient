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

#include "mms_dump_cmd.hpp"

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDateTime>
#include <QStandardPaths>

extern "C"
{
#include <mms_client_connection.h>
#include <mms_value.h>
}

namespace
{
    void writeValue(QTextStream &out, MmsValue *val, int depth)
    {
        if (!val) { out << "(null)"; return; }

        char buf[512];

        switch (MmsValue_getType(val)) {
        case MMS_STRUCTURE:
        case MMS_ARRAY: {
            int n = MmsValue_getArraySize(val);
            out << "{";
            for (int i = 0; i < n; i++) {
                if (i > 0) out << ", ";
                if (n > 4 && i == 0) out << "\n" << QString(depth + 2, ' ');
                else if (n > 4) out << QString(depth + 2, ' ');
                writeValue(out, MmsValue_getElement(val, i), depth + 2);
                if (n > 4) out << "\n";
            }
            if (n <= 4) out << "}";
            else out << QString(depth, ' ') << "}";
            break;
        }
        case MMS_BOOLEAN:
            out << (MmsValue_getBoolean(val) ? "true" : "false");
            break;
        case MMS_FLOAT:
            out << QString::number(MmsValue_toFloat(val), 'g', 6);
            break;
        case MMS_INTEGER:
            out << MmsValue_toInt32(val);
            break;
        case MMS_UNSIGNED:
            out << MmsValue_toUint32(val);
            break;
        case MMS_VISIBLE_STRING:
        case MMS_STRING: {
            const char *s = MmsValue_toString(val);
            out << "\"" << (s ? s : "") << "\"";
            break;
        }
        case MMS_BIT_STRING:
            MmsValue_printToBuffer(val, buf, sizeof(buf));
            out << buf;
            break;
        case MMS_OCTET_STRING: {
            uint16_t len = MmsValue_getOctetStringSize(val);
            uint8_t *data = MmsValue_getOctetStringBuffer(val);
            for (int i = 0; i < len; i++) {
                out << QString("%1").arg(data[i], 2, 16, QChar('0'));
                if (i < len - 1) out << ":";
            }
            break;
        }
        case MMS_UTC_TIME:
            out << "T" << MmsValue_toUnixTimestamp(val)
                  << "." << QString("%1").arg(MmsValue_getUtcTimeInMs(val) % 1000, 3, 10, QChar('0'))
                  << "Z";
            break;
        case MMS_BINARY_TIME:
            MmsValue_printToBuffer(val, buf, sizeof(buf));
            out << buf;
            break;
        case MMS_DATA_ACCESS_ERROR:
            out << "ACCESS_ERROR(" << MmsValue_getDataAccessError(val) << ")";
            break;
        default:
            MmsValue_printToBuffer(val, buf, sizeof(buf));
            out << buf;
            break;
        }
    }

    const char* mmsTypeStr(MmsType t)
    {
        switch (t) {
        case MMS_BOOLEAN:        return "BOOL";
        case MMS_INTEGER:        return "INT";
        case MMS_UNSIGNED:       return "UINT";
        case MMS_FLOAT:          return "FLOAT";
        case MMS_BIT_STRING:     return "BITS";
        case MMS_OCTET_STRING:   return "OSTR";
        case MMS_VISIBLE_STRING: return "VSTR";
        case MMS_STRING:         return "STR";
        case MMS_UTC_TIME:       return "UTC";
        case MMS_BINARY_TIME:    return "BTIME";
        case MMS_STRUCTURE:      return "STRUCT";
        case MMS_ARRAY:          return "ARRAY";
        case MMS_DATA_ACCESS_ERROR: return "ERR";
        default:                 return "?";
        }
    }
}

namespace Cmd
{
    void MmsDumpCmd::execute(Cmd::Interface::IEC61850_API::ptr)
    {
        QString target = QString("%1:%2").arg(m_cred.ip()).arg(m_cred.port());

        emit sigCmdEvent(CmdEvent::StartEvent("",
                QString("MMS dump: connecting to %1").arg(target)));

        // Create our own MMS connection
        MmsConnection con = MmsConnection_create();
        MmsConnection_setLocalDetail(con, 65000);

        MmsError err;
        if (!MmsConnection_connect(con, &err,
                m_cred.ip().toUtf8().constData(), m_cred.port())) {
            MmsConnection_destroy(con);
            emit sigCmdEvent(CmdEvent::FinishEvent("",
                    QString("MMS dump: connect failed (error %1)").arg(err), false));
            return;
        }

        emit sigCmdEvent(CmdEvent::ProcessEvent("",
                QString("MMS dump: connected to %1, reading model...").arg(target), 5));

        // Prepare output file
        QString downloadDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        if (downloadDir.isEmpty())
            downloadDir = QDir::currentPath();

        QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
        QString safeIp = m_cred.ip();
        safeIp.replace('.', '_').replace(':', '_');
        QString filePath = QString("%1/mms_dump_%2_%3.txt").arg(downloadDir, safeIp, timestamp);

        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            MmsConnection_destroy(con);
            emit sigCmdEvent(CmdEvent::FinishEvent("",
                    QString("MMS dump: cannot create file %1").arg(filePath), false));
            return;
        }

        QTextStream out(&file);

        out << "=== MMS Model Dump — " << target << " ===\n";
        out << "Date: " << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "\n\n";

        // Server identity
        MmsServerIdentity *id = MmsConnection_identify(con, &err);
        if (id) {
            out << "Server: vendor=\"" << (id->vendorName ? id->vendorName : "")
                << "\" model=\"" << (id->modelName ? id->modelName : "")
                << "\" rev=\"" << (id->revision ? id->revision : "") << "\"\n\n";
        }

        // Get domains
        LinkedList domains = MmsConnection_getDomainNames(con, &err);
        if (!domains) {
            out << "(Failed to get domains, err=" << err << ")\n";
            file.close();
            MmsConnection_destroy(con);
            emit sigCmdEvent(CmdEvent::FinishEvent("",
                    QString("MMS dump: failed to get domains"), false));
            return;
        }

        int totalDomains = LinkedList_size(domains) - 1;
        int domainIndex = 0;

        LinkedList dom = LinkedList_getNext(domains);
        while (dom) {
            const char *domainName = (const char *)dom->data;

            int perc = totalDomains > 0 ? (10 + domainIndex * 85 / totalDomains) : 50;
            emit sigCmdEvent(CmdEvent::ProcessEvent("",
                    QString("MMS dump: %1 (%2/%3)")
                        .arg(domainName).arg(domainIndex + 1).arg(totalDomains), perc));

            out << "DOMAIN: " << domainName << "\n";

            // Variables
            LinkedList vars = MmsConnection_getDomainVariableNames(con, &err, domainName);
            if (vars) {
                LinkedList v = LinkedList_getNext(vars);
                while (v) {
                    const char *varName = (const char *)v->data;
                    MmsValue *val = MmsConnection_readVariable(con, &err, domainName, varName);

                    out << "  " << varName << " = ";
                    if (err != MMS_ERROR_NONE) {
                        out << "(read error " << err << ")\n";
                    } else if (val) {
                        out << "[" << mmsTypeStr(MmsValue_getType(val)) << "] ";
                        writeValue(out, val, 2);
                        out << "\n";
                        MmsValue_delete(val);
                    } else {
                        out << "(null)\n";
                    }

                    v = LinkedList_getNext(v);
                }
                LinkedList_destroy(vars);
            }

            // Named variable lists (datasets)
            LinkedList nvls = MmsConnection_getDomainVariableListNames(con, &err, domainName);
            if (nvls) {
                LinkedList n = LinkedList_getNext(nvls);
                while (n) {
                    const char *listName = (const char *)n->data;
                    bool deletable = false;

                    out << "  DS " << listName;

                    LinkedList dir = MmsConnection_readNamedVariableListDirectory(
                        con, &err, domainName, listName, &deletable);

                    if (deletable) out << " (deletable)";
                    out << "\n";

                    if (dir) {
                        LinkedList e = LinkedList_getNext(dir);
                        int idx = 0;
                        while (e) {
                            MmsVariableAccessSpecification *spec =
                                (MmsVariableAccessSpecification *)e->data;
                            out << "    [" << idx << "] "
                                << (spec->domainId ? spec->domainId : "(vmd)") << "/"
                                << (spec->itemId ? spec->itemId : "?");
                            if (spec->componentName)
                                out << " component=" << spec->componentName;
                            out << "\n";
                            idx++;
                            e = LinkedList_getNext(e);
                        }
                        LinkedList_destroyDeep(dir,
                            (LinkedListValueDeleteFunction)MmsVariableAccessSpecification_destroy);
                    } else {
                        out << "    (could not read directory, err=" << err << ")\n";
                    }

                    n = LinkedList_getNext(n);
                }
                LinkedList_destroy(nvls);
            }

            out << "\n";
            domainIndex++;
            dom = LinkedList_getNext(dom);
        }

        LinkedList_destroy(domains);
        file.close();
        MmsConnection_destroy(con);

        emit sigCmdEvent(CmdEvent::FinishEvent("",
                QString("MMS dump: saved to %1").arg(filePath), true));
    }
}
