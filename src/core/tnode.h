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

#include <QString>
#include <QList>
#include <QSharedPointer>
#include <QDebug>

#include <functional>

namespace Core
{
	class TNode
	{
	protected:
		TNode*		m_parent = nullptr;
		QString		m_name;
		QString 	m_delimetr = "/";
		QList<QSharedPointer<TNode>>	m_child; // list of children

	public:
		QString		name() const {
			return m_name;
		}
		QString		parentName() {
			if (m_parent) {
				return m_parent->name();
			}
			return "";
		}
		TNode*		parentNode() const {
			return m_parent;
		}

		auto&		getChildList() const {
			return m_child;
		}
		size_t		getChildCount() const {
			return m_child.size();
		}

		template<typename T>
		QSharedPointer< T >		getChild(int t_inx) {
			if (t_inx >= 0 && t_inx < m_child.size()) {
				return m_child[t_inx].staticCast<T>();
			}
			return nullptr;
		}
		QSharedPointer< TNode >	getChild(int t_inx) {
			if (t_inx >= 0 && t_inx < m_child.size()) {
				return m_child[t_inx];
			}
			return nullptr;
		}

		QString 	ref(TNode *t_root=nullptr) {
			// Make full reference to Node in Tree
			QString path;
			if (m_parent && (m_parent != t_root)) {
				path = m_parent->ref(t_root) + m_delimetr;
			}
			return path + m_name;
		}

	public:
		TNode() = delete;
		TNode(TNode *t_parent, const QString &t_name)
			: m_parent(t_parent), m_name(t_name)
		{
		}
		virtual ~TNode() {
		}

		virtual void		addChild(QSharedPointer< TNode > t_child) {
			m_child.push_back(t_child);
		}
	};
}
