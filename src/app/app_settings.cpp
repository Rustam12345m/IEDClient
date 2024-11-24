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

#include "app_settings.hpp"
#include "app_utils.hpp"

#include <QFile>
#include <QXmlStreamReader>

#include <QStandardPaths>
#include <QCoreApplication>

namespace App
{
    AppSettings::AppSettings() : QObject(nullptr)
    {
        // Looking for the config file
        const QString curDirConfig = QCoreApplication::applicationDirPath()
                                    + "/" + App::ConfigFileName;
        const QString sysAppDirConfig =  QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)
                                    + "/" + App::ConfigFileName;
        if (QFile::exists(curDirConfig)) {
            m_confFilepath = curDirConfig;
        } else if (QFile::exists(sysAppDirConfig)) {
            m_confFilepath = sysAppDirConfig;
        } else {
            m_confFilepath = curDirConfig;
        }
    }

    AppSettings::AppSettings(const QString &t_filepath) : QObject(nullptr)
    {
        m_confFilepath = t_filepath;
    }

    lisHistConnInfo AppSettings::getConnectionList()
    {
        lisHistConnInfo devs;

        readConfigFile(m_confFilepath, devs);

        return devs;
    }

    void AppSettings::putConnectionToConfig(const Cmd::IEDCredentials &t_creds, const QString &t_ied)
    {
        App::ConfConnectionInfo newCon(t_creds, t_ied, App::GetCurrentDateTime());

        auto devs = getConnectionList();
        for (auto it=devs.begin();it!=devs.end();) {
            if (*it == newCon) {
                it = devs.erase(it);
            } else {
                ++it;
            }
        }
        devs.push_front(newCon);

        writeConfigFile(m_confFilepath, devs);
        emit sigConfUpdated();
    }

    void AppSettings::saveHistoryList(const lisHistConnInfo &t_list)
    {
        writeConfigFile(m_confFilepath, t_list);
    }

    int AppSettings::readConfigFile(const QString &t_filepath, lisHistConnInfo &t_list)
    {
        QFile conf(t_filepath);
        if (!conf.open(QFile::ReadOnly | QFile::Text)) {
            return -1;
        }

        QXmlStreamReader xml(&conf);
        while (!xml.atEnd() && !xml.hasError()) {
            QXmlStreamReader::TokenType token = xml.readNext();
            if (token == QXmlStreamReader::StartElement) {
                QString name = xml.name().toString();

                if (name == "main") {
                    // 
                } else if (name == "history_connections") {
                    // 
                } else if (name == "history_scl_files") {
                    // 
                }

                if (name == "device") {
                    Cmd::IEDCredentials cred(
                        xml.attributes().value("ip").toString(),
                        xml.attributes().value("port").toInt(),
                        xml.attributes().value("tls").toInt() == 1 ? true : false,
                        "",
                        "");

                    QString iedName = xml.attributes().value("ied").toString();
                    QString date = xml.attributes().value("date").toString();
                    t_list.push_front(App::ConfConnectionInfo(cred, iedName, date));
                }
            }
        }
        conf.close();
        return 0;
    }

    int AppSettings::writeConfigFile(const QString &t_filepath, const lisHistConnInfo &t_list)
    {
        QFile file(t_filepath);
        if (!file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)) {
            return -1;
        }

        QXmlStreamWriter xml(&file);
        xml.setAutoFormatting(true);

        xml.writeStartDocument();
        xml.writeStartElement("app");

        // main
        xml.writeStartElement("main");
        xml.writeEndElement();

        // history_connections
        xml.writeStartElement("history_connections");
        for (auto &d : t_list) {
            xml.writeStartElement("device");
            xml.writeAttribute("ip", d.ip());
            xml.writeAttribute("port", QString::number(d.port()));
            xml.writeAttribute("tls", QString::number(d.tls() ? 1 : 0));
            xml.writeAttribute("ied", d.ied());
            xml.writeAttribute("date", d.date());
            xml.writeEndElement();
        }
        xml.writeEndElement();

        // history_scl_files
        xml.writeStartElement("history_scl_files");
        xml.writeEndElement();

        xml.writeEndElement(); // app
        xml.writeEndDocument();

        file.close();
        return 0;
    }
}
