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

#include "item.hpp"

namespace Core
{
	// Forward declaration
	class DataObject;
	class LogicalNode;

	/*
	 * Representation Logical Node as a Table by particular FC or their combination
	 * 1. ST + MX + DC(d)
	 * 2. CO
	 * 3. CF + SG ?
	 * 
	 * ST + MX + DC(d):
	 * - Path relate on LN
	 * - FC
	 * - Value (stVal, mag)
	 * - Quality
	 * - Timestamp
	 * - Description
	 * */
	class LN_StateTable
	{
		class OneSignal
		{
		public:
			OneSignal() = default;
			OneSignal(const QString &t_path, const QString &t_fc, ptrItem t_v, ptrItem t_q, ptrItem t_ts, ptrItem t_desc)
				: m_name{t_path}, m_fc{t_fc}, m_value(t_v), m_quality(t_q), m_timestamp(t_ts), m_desc(t_desc)
			{}

			QString 	name() const { return m_name; }
			QString 	fc() const { return m_fc; }
			QString 	value() const {
				if (m_value) {
					return m_value->value();
				}
				return QString();
			}
			QString 	quality() const {
				if (m_quality) {
					return m_quality->value();
				}
				return QString();
			}
			QString 	timestamp() const {
				if (m_timestamp) {
					return m_timestamp->value();
				}
				return QString();
			}
			QString 	desc() const {
				if (m_desc) {
					return m_desc->value();
				}
				return QString();
			}

		private:
			QString 				m_name;
			QString 				m_fc; // Functional constraint of the Value
			QSharedPointer<Item>	m_value;
			QSharedPointer<Item>	m_quality;
			QSharedPointer<Item>	m_timestamp;
			QSharedPointer<Item>	m_desc;

			friend class LN_StateTableBuilder;
		};

	public:
		LN_StateTable() {}

		int 		size() const {
			return m_signals.size();
		}

		QString 	name(int t_row) const
		{
			if (t_row >= 0 && t_row < m_signals.size()) {
				return m_signals[t_row].name();
			}
			return "";
		}
		QString 	fc(int t_row) const
		{
			if (t_row >= 0 && t_row < m_signals.size()) {
				return m_signals[t_row].fc();
			}
			return "";
		}
		QString 	value(int t_row) const
		{
			if (t_row >= 0 && t_row < m_signals.size()) {
				return m_signals[t_row].value();
			}
			return "";
		}
		QString 	quality(int t_row) const
		{
			if (t_row >= 0 && t_row < m_signals.size()) {
				return m_signals[t_row].quality();
			}
			return "";
		}
		QString 	timestamp(int t_row) const
		{
			if (t_row >= 0 && t_row < m_signals.size()) {
				return m_signals[t_row].timestamp();
			}
			return "";
		}
		QString 	description(int t_row) const
		{
			if (t_row >= 0 && t_row < m_signals.size()) {
				return m_signals[t_row].desc();
			}
			return "";
		}

	protected:
		QList<OneSignal>	m_signals;

		friend class LN_StateTableBuilder;
	};

	/*
	 * SignalsTable's builder
	 * */
	class LN_StateTableBuilder
	{
	public:
		static QSharedPointer<LN_StateTable> create(QSharedPointer<LogicalNode> t_ln);

	private:
		static void recursFindSignals(QSharedPointer<LN_StateTable> t_table,
									  QSharedPointer<Item> t_root, QSharedPointer<Item> t_item,
									  LN_StateTable::OneSignal t_prototype);
	};
}