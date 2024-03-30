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

#include "model_item.hpp"

namespace Core
{
    // Forward declaration
    class DataObject;
    class LogicalNode;

    /*
     * 
     * */
    class SignalMatrixRow
    {
    public:
        SignalMatrixRow() = default;
        SignalMatrixRow(const QString &t_path, const QString &t_fc,
                        ptrModelItem t_v, ptrModelItem t_q, ptrModelItem t_ts,
                        ptrModelItem t_desc)
            : m_path{t_path}, m_fc{t_fc}, m_value(t_v), m_quality(t_q), m_timestamp(t_ts),
            m_desc(t_desc)
        {}

        ptrModelItem base() const { return m_dataObject; }
        QString    name() const { return m_path; }
        QString    fc() const { return m_fc; }
        QString    value() const {
            if (m_value) {
                return m_value->getValue();
            }
            return QString();
        }
        QString    quality() const {
            if (m_quality) {
                return m_quality->getValue();
            }
            return QString();
        }
        QString    timestamp() const {
            if (m_timestamp) {
                return m_timestamp->getValue();
            }
            return QString();
        }
        QString    desc() const {
            if (m_desc) {
                return m_desc->getValue();
            }
            return QString();
        }

    private:
        ptrModelItem    m_dataObject;
        QString         m_path;
        QString         m_fc; // Functional constraint of the Value
        ptrModelItem    m_value;
        ptrModelItem    m_quality;
        ptrModelItem    m_timestamp;
        ptrModelItem    m_desc;

        friend class LN_SignalMatrixBuilder;
    };

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
    class LN_SignalMatrix
    {
    public:
        LN_SignalMatrix() {}

        int         size() const {
            return m_signals.size();
        }

        QString     name(int t_row) const
        {
            if (t_row >= 0 && t_row < m_signals.size()) {
                return m_signals[t_row].name();
            }
            return "";
        }
        QString     fc(int t_row) const
        {
            if (t_row >= 0 && t_row < m_signals.size()) {
                return m_signals[t_row].fc();
            }
            return "";
        }
        QString     value(int t_row) const
        {
            if (t_row >= 0 && t_row < m_signals.size()) {
                return m_signals[t_row].value();
            }
            return "";
        }
        QString     quality(int t_row) const
        {
            if (t_row >= 0 && t_row < m_signals.size()) {
                return m_signals[t_row].quality();
            }
            return "";
        }
        QString     timestamp(int t_row) const
        {
            if (t_row >= 0 && t_row < m_signals.size()) {
                return m_signals[t_row].timestamp();
            }
            return "";
        }
        QString     description(int t_row) const
        {
            if (t_row >= 0 && t_row < m_signals.size()) {
                return m_signals[t_row].desc();
            }
            return "";
        }

        auto&       getRows() const {
            return m_signals;
        }

    protected:
        QList<SignalMatrixRow>    m_signals;

        friend class LN_SignalMatrixBuilder;
    };

    /*
     * SignalsTable's builder
     * */
    class LN_SignalMatrixBuilder
    {
    public:
        static QSharedPointer<LN_SignalMatrix> create(QSharedPointer<LogicalNode> t_ln);

    private:
        static void recursiveFillMatrix(QSharedPointer<LN_SignalMatrix> t_table,
                                        QSharedPointer<ModelItem> t_root,
                                        QSharedPointer<ModelItem> t_item,
                                        SignalMatrixRow t_prototype);
    };
}