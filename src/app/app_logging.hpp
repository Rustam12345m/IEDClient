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

#include <QString>

namespace App
{
    // Install a Qt message handler that mirrors qDebug/qInfo/qWarning/qCritical
    // to stderr and to a rolling log file at <AppLocalDataLocation>/logs/iedclient.log.
    // Call once after QCoreApplication::setOrganizationName / setApplicationName
    // and before the first qWarning, so the data path resolves correctly.
    void    installLogHandler();

    // Absolute path to the active log file (resolved on first call to installLogHandler).
    // Empty string if logging has not been installed.
    QString logFilePath();
}
