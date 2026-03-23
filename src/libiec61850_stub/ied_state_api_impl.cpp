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

#include "ied_state_api_impl.hpp"
#include "libiec61850_adapter.hpp"
#include "core/data_attribute.hpp"

#include <QDateTime>
#include <algorithm>

extern "C"
{
#include <iec61850_client.h>
}

namespace
{
    QString convertTimestampMsToUserString(uint64_t ms)
    {
        uint64_t sec = ms / 1000;
        QDateTime dt = QDateTime::fromSecsSinceEpoch(sec);
        return dt.toString("HH:mm:ss dd.MM.yyyy"); // ms isn't important
    }

    void     getValuesForItemByMmsValue(Core::ModelItem::ptr item, Core::ModelStateUpdater::ptr vals,
                                       MmsValue *mmsValue)
    {
        if (item == nullptr || vals == nullptr || mmsValue == nullptr) {
            return;
        }

        switch (MmsValue_getType(mmsValue)) {
        case MMS_ARRAY:
        case MMS_STRUCTURE: {
            int count = MmsValue_getArraySize(mmsValue);
            int items = item->getItemCount();
            int common = std::min(count, items);

            for (int i = 0; i < common; i++) {
                MmsValue *subMmsValue = (MmsValue *)MmsValue_getElement(mmsValue, i);
                Core::ModelItem::ptr subItem = item->getItem(i);

                getValuesForItemByMmsValue(subItem, vals, subMmsValue);
            }
            break;
        }
        case MMS_BOOLEAN: {
            QString v = MmsValue_getBoolean(mmsValue) ? "True" : "False";
            vals->push(item, Core::ModelItemValue::ptr::create(v));
            break;
        }
        case MMS_BIT_STRING: {
            uint64_t value = 0;
            int size = MmsValue_getBitStringSize(mmsValue);
            if (size < sizeof(value)*8) {
                for (int i=0;i<size;i++) {
                    value |= MmsValue_getBitStringBit(mmsValue, i) ? 1 << i : 0;
                }
            }

            QString v = QString::number((qulonglong)value, 2);
            vals->push(item, Core::ModelItemValue::ptr::create(v));
            break;
        }
        case MMS_INTEGER: {
            QString v = QString::number((long long)MmsValue_toInt64(mmsValue));
            vals->push(item, Core::ModelItemValue::ptr::create(v));
            break;
        }
        case MMS_UNSIGNED: {
            QString v = QString::number(MmsValue_toUint32(mmsValue));
            vals->push(item, Core::ModelItemValue::ptr::create(v));
            break;
        }
        case MMS_FLOAT: {
            QString v = QString::number(MmsValue_toFloat(mmsValue));
            vals->push(item, Core::ModelItemValue::ptr::create(v));
            break;
        }
        case MMS_OCTET_STRING: {
            QString tmp;
            int size = MmsValue_getOctetStringSize(mmsValue);
            tmp.reserve(2 * size);

            for (int i=0;i<size;i++) {
                tmp.push_back(QString::number(MmsValue_getOctetStringOctet(mmsValue, i), 16));
            }
            vals->push(item, Core::ModelItemValue::ptr::create(tmp));
            break;
        }
        case MMS_GENERALIZED_TIME: {
            vals->push(item, Core::ModelItemValue::ptr::create("Unsupported"));
            break;
        }
        case MMS_BINARY_TIME: {
            uint64_t ms = MmsValue_getBinaryTimeAsUtcMs(mmsValue);
            QString v = convertTimestampMsToUserString(ms);
            vals->push(item, Core::ModelItemValue::ptr::create(v));
            break;
        }
        case MMS_BCD: {
            vals->push(item, Core::ModelItemValue::ptr::create("? BCD"));
            break;
        }
        case MMS_OBJ_ID: {
            vals->push(item, Core::ModelItemValue::ptr::create("? OBJID"));
            break;
        }
        case MMS_STRING:
        case MMS_VISIBLE_STRING: {
            const char *src = MmsValue_toString(mmsValue);
            QString v = src ? QString::fromLocal8Bit(src) : QString();
            vals->push(item, Core::ModelItemValue::ptr::create(v));
            break;
        }
        case MMS_UTC_TIME: {
            QString v = convertTimestampMsToUserString(MmsValue_getUtcTimeInMs(mmsValue));
            vals->push(item, Core::ModelItemValue::ptr::create(v));
            break;
        }
        default: {
            char tmp[1024] = { 0 };
            MmsValue_printToBuffer(mmsValue, tmp, 1024);
            QString v = QString::fromLocal8Bit(tmp);
            vals->push(item, Core::ModelItemValue::ptr::create(v));
        }
        }
    }

    int     getValuesForDataAttribute(Core::ModelItem::ptr item, Core::ModelStateUpdater::ptr vals,
                                      sIedConnection *con, FunctionalConstraint fc)
    {
        IedClientError retval = IED_ERROR_OK;
        auto ref = item->getReference().toStdString();

        MmsValue *rawValue = IedConnection_readObject(con, &retval, ref.data(), fc);
        if (retval != IED_ERROR_OK || rawValue == nullptr) {
            MmsValue_delete(rawValue);
            return -1;
        }

        getValuesForItemByMmsValue(item, vals, rawValue);

        MmsValue_delete(rawValue);
        return 0;
    }

    int     getValuesForDataObject(Core::DataObject::ptr dataObj, Core::ModelStateUpdater::ptr vals,
                                   sIedConnection *con)
    {
        if (dataObj == nullptr || vals == nullptr || con == nullptr) {
            return -1;
        }

        for (size_t j=0;j<dataObj->getItemCount();j++) {
            auto daNode = dataObj->getItem< Core::DataAttribute >(j);
            if (!daNode) continue;
            auto fcNum = (FunctionalConstraint)daNode->fcNum();

            getValuesForDataAttribute(daNode, vals, con, fcNum);
        }
        return 0;
    }
}

namespace Libiec61850
{
    Core::ModelStateUpdater::ptr IED_StateAPI_Impl::getStatusForAllLD(Core::DataModel::ptr model)
    {
        if (!m_api.isConnected()) {
            return nullptr;
        }

        auto vals = Core::ModelStateUpdater::ptr::create();

        for (size_t i=0;i<model->getItemCount();i++) {
            auto ld = model->getItem< Core::LogicalDevice >(i);

            if (ld->lln0()) {
                getValuesForDataObject(ld->lln0()->getModItem(), vals, m_api.m_libConn);
                getValuesForDataObject(ld->lln0()->getBehItem(), vals, m_api.m_libConn);
                getValuesForDataObject(ld->lln0()->getHealthItem(), vals, m_api.m_libConn);
            }
        }
        return vals;
    }

    Core::ModelStateUpdater::ptr IED_StateAPI_Impl::getStatusForAllLN(Core::LogicalDevice::ptr ld)
    {
        if (!m_api.isConnected()) {
            return nullptr;
        }

        auto vals = Core::ModelStateUpdater::ptr::create();
        for (size_t i=0;i<ld->getItemCount();i++) {
            auto ln = ld->getItem< Core::LogicalNode >(i);

            getValuesForDataObject(ln->getModItem(), vals, m_api.m_libConn);
            getValuesForDataObject(ln->getBehItem(), vals, m_api.m_libConn);
            getValuesForDataObject(ln->getHealthItem(), vals, m_api.m_libConn);
        }
        return vals;
    }

    Core::ModelStateUpdater::ptr IED_StateAPI_Impl::getValsForLN(Core::LogicalNode::ptr ln)
    {
        if (!m_api.isConnected()) {
            return nullptr;
        }

        auto vals = Core::ModelStateUpdater::ptr::create();
        for (size_t i=0;i<ln->getItemCount();i++) {
            auto item = ln->getItem< Core::DataObject >(i);

            getValuesForDataObject(item, vals, m_api.m_libConn);
        }
        return vals;
    }

    static QString mmsValueToString(MmsValue *mmsValue)
    {
        if (!mmsValue) return {};

        switch (MmsValue_getType(mmsValue)) {
        case MMS_BOOLEAN:
            return MmsValue_getBoolean(mmsValue) ? "True" : "False";
        case MMS_INTEGER:
            return QString::number((long long)MmsValue_toInt64(mmsValue));
        case MMS_UNSIGNED:
            return QString::number(MmsValue_toUint32(mmsValue));
        case MMS_FLOAT:
            return QString::number(MmsValue_toFloat(mmsValue));
        case MMS_VISIBLE_STRING:
        case MMS_STRING: {
            const char *src = MmsValue_toString(mmsValue);
            return src ? QString::fromLocal8Bit(src) : QString();
        }
        case MMS_UTC_TIME:
            return convertTimestampMsToUserString(MmsValue_getUtcTimeInMs(mmsValue));
        case MMS_BINARY_TIME:
            return convertTimestampMsToUserString(MmsValue_getBinaryTimeAsUtcMs(mmsValue));
        case MMS_BIT_STRING: {
            uint64_t value = 0;
            int size = MmsValue_getBitStringSize(mmsValue);
            if (size < (int)(sizeof(value) * 8)) {
                for (int i = 0; i < size; i++)
                    value |= MmsValue_getBitStringBit(mmsValue, i) ? (1ULL << i) : 0;
            }
            return QString::number((qulonglong)value, 2);
        }
        case MMS_ARRAY:
        case MMS_STRUCTURE: {
            char tmp[1024] = {0};
            MmsValue_printToBuffer(mmsValue, tmp, 1024);
            return QString::fromLocal8Bit(tmp);
        }
        default: {
            char tmp[1024] = {0};
            MmsValue_printToBuffer(mmsValue, tmp, 1024);
            return QString::fromLocal8Bit(tmp);
        }
        }
    }

    QVariantList IED_StateAPI_Impl::readValuesByRef(const QStringList &refs, const QStringList &fcs)
    {
        QVariantList results;
        if (!m_api.isConnected()) return results;

        for (int i = 0; i < refs.size(); i++) {
            auto ref = refs[i].toStdString();
            auto fc = (FunctionalConstraint)Core::DataAttribute::fcStringToNum(
                i < fcs.size() ? fcs[i] : "ST");

            IedClientError retval = IED_ERROR_OK;
            MmsValue *rawValue = IedConnection_readObject(m_api.m_libConn, &retval, ref.data(), fc);

            QVariantMap entry;
            entry["ref"] = refs[i];
            if (retval == IED_ERROR_OK && rawValue) {
                entry["value"] = mmsValueToString(rawValue);
                MmsValue_delete(rawValue);
            } else {
                entry["value"] = QString("error %1").arg((int)retval);
            }
            results.append(entry);
        }
        return results;
    }

    Core::ModelStateUpdater::ptr IED_StateAPI_Impl::getValsForDS(Core::DataSet::ptr ds)
    {
        if (!m_api.isConnected()) {
            return nullptr;
        }

        QString dsRef = ds->ref() + "." + ds->getName();

        IedClientError retval = IED_ERROR_OK;
        ClientDataSet clientDataSet = IedConnection_readDataSetValues(
            m_api.m_libConn, &retval, dsRef.toStdString().data(), nullptr);

        if (retval != IED_ERROR_OK || clientDataSet == nullptr) {
            return nullptr;
        }

        auto vals = Core::ModelStateUpdater::ptr::create();

        MmsValue *dataSetValues = ClientDataSet_getValues(clientDataSet);
        if (dataSetValues != nullptr) {
            int count = MmsValue_getArraySize(dataSetValues);
            for (int i = 0; i < count && i < (int)ds->getItemCount(); i++) {
                auto dsItem = ds->getItem<Core::DataSetItem>(i);
                if (dsItem && dsItem->item()) {
                    MmsValue *itemValue = MmsValue_getElement(dataSetValues, i);
                    getValuesForItemByMmsValue(dsItem->item(), vals, itemValue);
                }
            }
        }

        ClientDataSet_destroy(clientDataSet);
        return vals;
    }
}