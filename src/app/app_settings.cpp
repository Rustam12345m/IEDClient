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
#include <QDir>
#include <QFileInfo>

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
            // Prefer system config dir for new files (writable on all platforms)
            m_confFilepath = sysAppDirConfig;
        }
    }

    AppSettings::AppSettings(const QString &filepath) : QObject(nullptr)
    {
        m_confFilepath = filepath;
    }

    lisHistConnInfo AppSettings::getConnectionList()
    {
        lisHistConnInfo devs;

        readConfigFile(m_confFilepath, devs);

        return devs;
    }

    void AppSettings::putConnectionToConfig(const Cmd::IEDCredentials &creds, const QString &ied)
    {
        App::ConfConnectionInfo newCon(creds, ied, App::GetCurrentDateTime());

        auto devs = getConnectionList();
        for (auto it=devs.begin();it!=devs.end();) {
            if (*it == newCon) {
                newCon.setWatchlist(it->watchlist());
                it = devs.erase(it);
            } else {
                ++it;
            }
        }
        devs.push_front(newCon);

        writeConfigFile(m_confFilepath, devs);
        emit sigConfUpdated();
    }

    void AppSettings::saveHistoryList(const lisHistConnInfo &list)
    {
        writeConfigFile(m_confFilepath, list);
    }

    WatchlistRefs AppSettings::getWatchlistForDevice(const Cmd::IEDCredentials &creds)
    {
        auto devs = getConnectionList();
        for (const auto &d : devs) {
            ConfConnectionInfo tmp(creds, "", "");
            if (d == tmp) {
                return d.watchlist();
            }
        }
        return {};
    }

    void AppSettings::saveWatchlistForDevice(const Cmd::IEDCredentials &creds, const WatchlistRefs &wl)
    {
        auto devs = getConnectionList();
        for (auto &d : devs) {
            ConfConnectionInfo tmp(creds, "", "");
            if (d == tmp) {
                d.setWatchlist(wl);
                writeConfigFile(m_confFilepath, devs);
                return;
            }
        }
    }

    int AppSettings::readConfigFile(const QString &filepath, lisHistConnInfo &list)
    {
        QFile conf(filepath);
        if (!conf.open(QFile::ReadOnly | QFile::Text)) {
            return -1;
        }

        QXmlStreamReader xml(&conf);
        while (!xml.atEnd() && !xml.hasError()) {
            QXmlStreamReader::TokenType token = xml.readNext();
            if (token == QXmlStreamReader::StartElement) {
                QString name = xml.name().toString();

                if (name == "device") {
                    Cmd::IEDCredentials cred(
                        xml.attributes().value("ip").toString(),
                        xml.attributes().value("port").toInt(),
                        xml.attributes().value("tls").toInt() == 1 ? true : false,
                        "",
                        "");

                    QString iedName = xml.attributes().value("ied").toString();
                    QString date = xml.attributes().value("date").toString();

                    App::ConfConnectionInfo info(cred, iedName, date);

                    WatchlistRefs wl;
                    while (!xml.atEnd()) {
                        xml.readNext();
                        if (xml.isEndElement() && xml.name().toString() == "device") {
                            break;
                        }
                        if (xml.isStartElement() && xml.name().toString() == "node") {
                            QString ref = xml.attributes().value("ref").toString();
                            QString fc = xml.attributes().value("fc").toString();
                            if (!ref.isEmpty()) {
                                wl.append({ref, fc});
                            }
                        }
                    }
                    info.setWatchlist(wl);
                    list.push_front(info);
                }
            }
        }
        conf.close();
        return 0;
    }

    int AppSettings::writeConfigFile(const QString &filepath, const lisHistConnInfo &list)
    {
        // Ensure parent directory exists
        QDir dir = QFileInfo(filepath).absoluteDir();
        if (!dir.exists()) {
            dir.mkpath(".");
        }

        QFile file(filepath);
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
        for (auto &d : list) {
            xml.writeStartElement("device");
            xml.writeAttribute("ip", d.ip());
            xml.writeAttribute("port", QString::number(d.port()));
            xml.writeAttribute("tls", QString::number(d.tls() ? 1 : 0));
            xml.writeAttribute("ied", d.ied());
            xml.writeAttribute("date", d.date());

            if (!d.watchlist().isEmpty()) {
                xml.writeStartElement("watchlist");
                for (const auto &[ref, fc] : d.watchlist()) {
                    xml.writeStartElement("node");
                    xml.writeAttribute("ref", ref);
                    xml.writeAttribute("fc", fc);
                    xml.writeEndElement();
                }
                xml.writeEndElement();
            }

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
