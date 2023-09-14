/*
 *  main.cpp
 *
 *  Copyright 2023 Rustam Mustafin
 *
 *  This file is part of IEDMaster.
 *
 *  IEDMaster is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  IEDMaster is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with IEDMaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 * */

#pragma once

#include <QString>
#include <QList>
#include <QSharedPointer>
#include <QDebug>

#include <memory>

namespace Core
{
	class TNode;
	class FlatenTabRow
	{
		QString					m_path;
		QSharedPointer<TNode>	m_node;
	public:
		auto name() const { return m_path; }
		auto node() { return m_node; }

		FlatenTabRow(const QString &t_path, QSharedPointer<TNode> &t_node) {
			m_path = t_path;
			m_node = t_node;
		}
	};

	class TNode
	{
	protected:
		TNode*		m_parent = nullptr;
		QString		m_name;
		QList<QSharedPointer<TNode>>	m_child;

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

		auto&		getNodeList() const {
			return m_child;
		}
		size_t		getNodeCount() const {
			return m_child.size();
		}

		template<typename T>
		QSharedPointer< T >		getChildPtr(int t_inx) {
			if (t_inx >= 0 && t_inx < m_child.size()) {
				return m_child[t_inx].staticCast<T>();
			}
			return nullptr;
		}
		QSharedPointer< TNode >	getChildPtr(int t_inx) {
			if (t_inx >= 0 && t_inx < m_child.size()) {
				return m_child[t_inx];
			}
			return nullptr;
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

		virtual size_t		flatten() {
			QList<FlatenTabRow> table;
			DFS("[BAZA]", table, m_child);

			qDebug() << "DFS: " << m_name;
			for (auto &row : table) {
				qDebug() << "	" << row.name();
			}
			return table.size();
		}

		static void	DFS(const QString &t_prefix, QList<FlatenTabRow> &t_table,
						const QList<QSharedPointer<TNode>> &t_graph) {
			for (auto node : t_graph) {
				auto &subNode = node->getNodeList();
				if (subNode.empty()) {
					t_table.emplaceBack(FlatenTabRow(t_prefix + "/" + node->name(), node));
				} else {
					DFS(t_prefix + "/" + node->name(), t_table, subNode);
				}
			}
		}
	};
}
