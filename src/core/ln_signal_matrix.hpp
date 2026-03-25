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
    class LogicalDevice;

    class SignalMatrixRow
    {
    public:
        SignalMatrixRow() = default;
        SignalMatrixRow(const QString &path, const QString &fc,
                        ModelItem::ptr v, ModelItem::ptr q, ModelItem::ptr ts,
                        ModelItem::ptr desc)
            : m_path{path}, m_fc{fc}, m_value(v), m_quality(q), m_timestamp(ts),
            m_desc(desc)
        {}

        ModelItem::ptr base() const { return m_dataObject; }
        ModelItem::ptr valueItem() const { return m_value; }
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

        // Control-specific accessors (resolved once during matrix build)
        QString    ctlType() const { return m_ctlType; }
        QString    ctlModel() const {
            return m_ctlModel ? m_ctlModel->getValue() : QString();
        }
        QString    stSeld() const {
            return m_stSeld ? m_stSeld->getValue() : QString();
        }
        QString    opOk() const {
            return m_opOk ? m_opOk->getValue() : QString();
        }

        // LD-level settings (populated only by createLD_SettingsMatrix)
        QString    lnName() const { return m_lnName; }

    private:
        ModelItem::ptr    m_dataObject;
        QString           m_path;
        QString           m_fc; // Functional constraint of the Value
        ModelItem::ptr    m_value;
        ModelItem::ptr    m_quality;
        ModelItem::ptr    m_timestamp;
        ModelItem::ptr    m_desc;

        // LD-level settings (populated only by createLD_SettingsMatrix)
        QString           m_lnName;     // LN name (e.g., "LLN0", "PTOC1")

        // Control-specific (populated only by createControlsMatrix)
        QString           m_ctlType;    // CDC: "SPC", "DPC", "INC", "APC", "BSC"
        ModelItem::ptr    m_ctlModel;   // ctlModel (CF)
        ModelItem::ptr    m_stSeld;     // stSeld (ST, optional)
        ModelItem::ptr    m_opOk;       // opOk (ST, optional)

        friend class LN_SignalMatrixBuilder;
    };

    /**
     * @brief Representation Logical Node as a Table by particular FC or their combination
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
     */
    class LN_SignalMatrix
    {
    public:
        using ptr = QSharedPointer< LN_SignalMatrix >;

        LN_SignalMatrix() {}

        int         size() const {
            return m_signals.size();
        }

        QString     name(int row) const
        {
            if (row >= 0 && row < m_signals.size()) {
                return m_signals[row].name();
            }
            return "";
        }
        QString     fc(int row) const
        {
            if (row >= 0 && row < m_signals.size()) {
                return m_signals[row].fc();
            }
            return "";
        }
        QString     value(int row) const
        {
            if (row >= 0 && row < m_signals.size()) {
                return m_signals[row].value();
            }
            return "";
        }
        QString     quality(int row) const
        {
            if (row >= 0 && row < m_signals.size()) {
                return m_signals[row].quality();
            }
            return "";
        }
        QString     timestamp(int row) const
        {
            if (row >= 0 && row < m_signals.size()) {
                return m_signals[row].timestamp();
            }
            return "";
        }
        QString     description(int row) const
        {
            if (row >= 0 && row < m_signals.size()) {
                return m_signals[row].desc();
            }
            return "";
        }

        QString     ctlType(int row) const
        {
            if (row >= 0 && row < m_signals.size()) {
                return m_signals[row].ctlType();
            }
            return "";
        }
        QString     ctlModel(int row) const
        {
            if (row >= 0 && row < m_signals.size()) {
                return m_signals[row].ctlModel();
            }
            return "";
        }
        QString     stSeld(int row) const
        {
            if (row >= 0 && row < m_signals.size()) {
                return m_signals[row].stSeld();
            }
            return "";
        }
        QString     opOk(int row) const
        {
            if (row >= 0 && row < m_signals.size()) {
                return m_signals[row].opOk();
            }
            return "";
        }

        QString     lnName(int row) const
        {
            if (row >= 0 && row < m_signals.size()) {
                return m_signals[row].lnName();
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

    /**
     * @brief SignalsTable's builder
     */
    class LN_SignalMatrixBuilder
    {
    public:
        static LN_SignalMatrix::ptr create(QSharedPointer< LogicalNode > ln,
                                           const QList<QString> &fcFilter);
        static LN_SignalMatrix::ptr createControlsMatrix(QSharedPointer< LogicalNode > ln);
        static LN_SignalMatrix::ptr createLD_SettingsMatrix(QSharedPointer< LogicalDevice > ld);

    private:
        static void recursiveFillMatrix(LN_SignalMatrix::ptr table,
                                        ModelItem::ptr root,
                                        ModelItem::ptr item,
                                        SignalMatrixRow prototype,
                                        const QList<QString> &fcFilter);
    };
}
