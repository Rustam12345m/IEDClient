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

#include <QObject>
#include <QString>
#include <QList>

#include <cstdint>

namespace Core
{
    class FileOn
    {
    public:
        FileOn(const QString &name, uint32_t size, uint64_t ts)
            : m_fileName{name}, m_size{size}, m_mts{ts}
        {
        }
        FileOn() = delete;

        QString     name() const {
            return m_fileName;
        }
        uint32_t     size() const {
            return m_size;
        }
        uint64_t     timestamp() const {
            return m_mts;
        }

    public:
        QString     m_fileName;
        uint32_t    m_size = 0;
        uint64_t    m_mts = 0; // last modified ts
    };

    class DirOn
    {
    public:
        QString name()
        {
            return m_name;
        }

    public:
        DirOn() {}
        DirOn(const QString &name) : m_name{name} {}

        void          put(const FileOn &file)
        {
            m_file.push_back(file);
        }

        std::tuple<size_t, size_t>         getDirectoryInfo() {
            size_t total = 0;
            for (auto &f : m_file) {
                total += f.size();
            }
            size_t count = m_file.size();
            return std::make_tuple(count, total);
        }
        size_t        getCount() {
            return m_file.size();
        }
        void          removeFileFromList(int index) {
            m_file.removeAt(index);
        }

    public:
        QString       m_name;
        QList<FileOn> m_file;
    };

    class FS_Model : public QObject
    {
        Q_OBJECT
    public:
        using ptr = QSharedPointer< FS_Model >;

        FS_Model(QObject *parent = nullptr) : QObject(parent) {}

        void put(const DirOn &dir)
        {
            m_dir = dir;
            emit sigFS_Updated();
        }

        auto getFS_StatInfo() {
            return m_dir.getDirectoryInfo();
        }
        size_t getCount() {
            return m_dir.getCount();
        }
        void removeFileFromList(int index) {
            m_dir.removeFileFromList(index);
        }

    signals:
        void sigFS_Updated();

    public:
        DirOn m_dir;
    };
}