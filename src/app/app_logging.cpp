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

#include "app_logging.hpp"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMutex>
#include <QStandardPaths>
#include <QThread>

#include <cstdio>
#include <cstdlib>

namespace App
{
    namespace
    {
        constexpr qint64 ROTATE_BYTES = 1 * 1024 * 1024;

        QMutex   g_mutex;
        QFile   *g_file       = nullptr;
        bool     g_openFailed = false;
        QString  g_logPath;

        // Open the log file lazily so we have a chance to honour
        // QCoreApplication::setApplicationName before resolving the path.
        // Rotates the previous file to `.1` if it crossed ROTATE_BYTES on
        // the previous run — keeping at most one historical file.
        void openLogFileLocked()
        {
            if (g_file || g_openFailed) return;

            const QString dir = QStandardPaths::writableLocation(
                                    QStandardPaths::AppLocalDataLocation) + "/logs";
            QDir().mkpath(dir);

            g_logPath = dir + "/iedclient.log";

            const QFileInfo fi(g_logPath);
            if (fi.exists() && fi.size() > ROTATE_BYTES) {
                QFile::remove(g_logPath + ".1");
                QFile::rename(g_logPath, g_logPath + ".1");
            }

            g_file = new QFile(g_logPath);
            if (!g_file->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
                delete g_file;
                g_file = nullptr;
                g_openFailed = true;
            }
        }

        const char *levelTag(QtMsgType type)
        {
            switch (type) {
            case QtDebugMsg:    return "D";
            case QtInfoMsg:     return "I";
            case QtWarningMsg:  return "W";
            case QtCriticalMsg: return "C";
            case QtFatalMsg:    return "F";
            }
            return "?";
        }

        QString threadTag()
        {
            const QString name = QThread::currentThread()->objectName();
            if (!name.isEmpty()) return name;
            return QString::number(reinterpret_cast<quintptr>(QThread::currentThreadId()), 16);
        }

        void messageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg)
        {
            const QString line = QString("%1 [%2] [%3] %4\n")
                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
                .arg(levelTag(type), threadTag(), msg);

            const QByteArray utf = line.toLocal8Bit();

            // stderr: terminal users / CI logs see output too.
            std::fputs(utf.constData(), stderr);

            QMutexLocker locker(&g_mutex);
            openLogFileLocked();
            if (g_file) {
                g_file->write(utf);
                g_file->flush();
            }

            if (type == QtFatalMsg) {
                std::abort();
            }
        }
    }

    void installLogHandler()
    {
        qInstallMessageHandler(messageHandler);
    }

    QString logFilePath()
    {
        QMutexLocker locker(&g_mutex);
        return g_logPath;
    }
}
