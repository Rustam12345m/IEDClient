/*
 *  main.cpp
 *
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

#include <stdint.h>

class FilesTableModel;

namespace Core
{
	class FileOn
	{
	public:
		FileOn(const QString &t_name, uint32_t t_size, uint64_t t_ts)
			: m_fileName{t_name}, m_size{t_size}, m_mts{t_ts}
		{
		}
		FileOn() = delete;

		QString 	name() const {
			return m_fileName;
		}
		uint32_t 	size() const {
			return m_size;
		}
		uint64_t 	timestamp() const {
			return m_mts;
		}

	private:
		QString 	m_fileName;
		uint32_t 	m_size = 0;
		uint64_t 	m_mts = 0; // last modified ts

		friend class ::FilesTableModel;
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
		DirOn(const QString &t_name) : m_name{t_name} {}

		void 		put(const FileOn &t_file)
		{
			m_file.push_back(t_file);
		}

		std::tuple<size_t, size_t> 		getDirectoryInfo() {
			size_t total = 0;
			for (auto &f : m_file) {
				total += f.size();
			}
			size_t count = m_file.size();
			return std::make_tuple(count, total);
		}
		size_t 		getCount() {
			return m_file.size();
		}
		void 		removeFileFromList(int t_index) {
			m_file.removeAt(t_index);
		}

	private:
		QString 		m_name;
		QList<FileOn> 	m_file;

		friend class ::FilesTableModel;
	};

	class FS_Tree : public QObject
	{
		Q_OBJECT
	public:
		FS_Tree(QObject *t_parent = nullptr) : QObject(t_parent) {}

		void put(const DirOn &t_dir)
		{
			m_dir = t_dir;
			emit sigFS_Updated();
		}

		auto 		getFS_StatInfo() {
			return m_dir.getDirectoryInfo();
		}
		size_t 		getCount() {
			return m_dir.getCount();
		}
		void 		removeFileFromList(int t_index) {
			m_dir.removeFileFromList(t_index);
		}

	signals:
		void sigFS_Updated();

	private:
		DirOn 			m_dir;

		friend class ::FilesTableModel;
	};
}