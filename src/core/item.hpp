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

#include <QDebug>
#include <QList>
#include <QString>
#include <QSharedPointer>

/*
 * Basic class for all nodes in DataModel that we make by MMS/SCL
 * */
namespace Core
{
	class Item
	{
	public:
		QString		name() const {
			return m_name;
		}
		QString		parentName() const {
			if (m_parent) {
				return m_parent->name();
			}
			return "";
		}
		Item*		parent() const {
			return m_parent;
		}
		QString 	ref(Item *t_root=nullptr) {
			// Make full reference to item in this model
			QString path;
			if (m_parent && (m_parent != t_root)) {
				path = m_parent->ref(t_root) + m_delimetr;
			}
			return path + m_name;
		}

		auto&		getItemList() const {
			return m_items;
		}
		size_t		getItemCount() const {
			return m_items.size();
		}

		template<typename T>
		QSharedPointer< T >		getItem(int t_inx) {
			if ((t_inx >= 0) && (t_inx < m_items.size())) {
				return m_items[t_inx].staticCast<T>();
			}
			return nullptr;
		}
		QSharedPointer< Item >	getItem(int t_inx) {
			return Item::getItem<Item>(t_inx);
		}
		QSharedPointer< Item >	getItem(const QString &t_name) {
			for (auto it : m_items) {
				if (it->name() == t_name) {
					return it;
				}
			}
			return nullptr;
		}

		QSharedPointer<Item> 	find(const QString &t_name) {
			for (auto it : m_items) {
				if (it->name() == t_name) {
					return it;
				}
			}
			return nullptr;
		}
		template <typename... Names>
		QSharedPointer<Item> 	find(const QString &t_first, Names... rest) {
			for (auto it : m_items) {
				if (it->name() == t_first) {
					return it->find(rest...);
				}
			}
			return nullptr;
		}

		virtual void			push(QSharedPointer< Item > t_child) {
			m_items.push_back(t_child);
		}
		virtual QString 		value() {
			return "";
		}
		virtual void			update(const QString &t_value) {
		}

	public:
		Item() = delete;
		Item(Item *t_parent, const QString &t_name)
			: m_parent(t_parent), m_name(t_name)
		{
		}
		virtual ~Item() {
		}

	protected:
		virtual void 	debug(QDebug &t_debug) const {
			t_debug.noquote() << m_delimetr << m_name << "items:" << m_items.size();
		}
	public:
		friend QDebug operator<<(QDebug t_debug, const Item &t_item) {
			t_item.debug(t_debug);
			return t_debug;
		}

	protected:
		QString						m_name;
		QString 					m_delimetr = "/"; // Current node and its children
		Item*						m_parent = nullptr;
		QList<QSharedPointer<Item>>	m_items; // List of children
	};

	typedef QSharedPointer< Item > 	ptrItem;
}