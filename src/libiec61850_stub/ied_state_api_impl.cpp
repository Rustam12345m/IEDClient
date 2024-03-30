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

#include <QDateTime>

extern "C"
{
#include <iec61850_client.h>
}

namespace
{
    QString convertTimestampMsToUserString(uint64_t t_ms)
    {
        uint64_t sec = t_ms / 1000;
        QDateTime dt = QDateTime::fromSecsSinceEpoch(sec);
        return dt.toString("HH:mm:ss dd.MM.yyyy"); // ms isn't important
    }

    void     getValuesForItemByMmsValue(Core::ptrModelItem t_item, Core::ptrModelStateUpd t_vals,
                                       MmsValue *t_mmsValue)
    {
        if (t_item == nullptr || t_vals == nullptr || t_mmsValue == nullptr) {
            return;
        }

        switch (MmsValue_getType(t_mmsValue)) {
        case MMS_ARRAY:
        case MMS_STRUCTURE: {
            int count = MmsValue_getArraySize(t_mmsValue);
            int items = t_item->getItemCount();
            if (count != items) {
                t_vals->push(t_item, Core::ptrModelValue::create("Mismatch number of elements"));
                break;
            }

            for (int i=0;i<count;i++) {
                MmsValue *subMmsValue = (MmsValue *)MmsValue_getElement(t_mmsValue, i);
                Core::ptrModelItem subItem = t_item->getItem(i);

                getValuesForItemByMmsValue(subItem, t_vals, subMmsValue);
            }
            break;
        }
        case MMS_BOOLEAN: {
            QString v = MmsValue_getBoolean(t_mmsValue) ? "True" : "False";
            t_vals->push(t_item, Core::ptrModelValue::create(v));
            break;
        }
        case MMS_BIT_STRING: {
            uint64_t value = 0;
            int size = MmsValue_getBitStringSize(t_mmsValue);
            if (size < sizeof(value)*8) {
                for (int i=0;i<size;i++) {
                    value |= MmsValue_getBitStringBit(t_mmsValue, i) ? 1 << i : 0;
                }
            }

            QString v = QString::number((qulonglong)value, 2);
            t_vals->push(t_item, Core::ptrModelValue::create(v));
            break;
        }
        case MMS_INTEGER: {
            QString v = QString::number((long long)MmsValue_toInt64(t_mmsValue));
            t_vals->push(t_item, Core::ptrModelValue::create(v));
            break;
        }
        case MMS_UNSIGNED: {
            QString v = QString::number(MmsValue_toUint32(t_mmsValue));
            t_vals->push(t_item, Core::ptrModelValue::create(v));
            break;
        }
        case MMS_FLOAT: {
            QString v = QString::number(MmsValue_toFloat(t_mmsValue));
            t_vals->push(t_item, Core::ptrModelValue::create(v));
            break;
        }
        case MMS_OCTET_STRING: {
            QString tmp;
            int size = MmsValue_getOctetStringSize(t_mmsValue);
            tmp.reserve(2 * size);

            for (int i=0;i<size;i++) {
                tmp.push_back(QString::number(MmsValue_getOctetStringOctet(t_mmsValue, i), 16));
            }
            t_vals->push(t_item, Core::ptrModelValue::create(tmp));
            break;
        }
        case MMS_GENERALIZED_TIME: {
            t_vals->push(t_item, Core::ptrModelValue::create("Unsupported"));
            break;
        }
        case MMS_BINARY_TIME: {
            uint64_t ms = MmsValue_getBinaryTimeAsUtcMs(t_mmsValue);
            QString v = convertTimestampMsToUserString(ms);
            t_vals->push(t_item, Core::ptrModelValue::create(v));
            break;
        }
        case MMS_BCD: {
            t_vals->push(t_item, Core::ptrModelValue::create("? BCD"));
            break;
        }
        case MMS_OBJ_ID: {
            t_vals->push(t_item, Core::ptrModelValue::create("? OBJID"));
            break;
        }
        case MMS_STRING:
        case MMS_VISIBLE_STRING: {
            char tmp[256] = { 0 };
            strncpy(tmp, MmsValue_toString(t_mmsValue), 256);
            tmp[255] = 0;
            QString v = QString::fromLocal8Bit(tmp);
            t_vals->push(t_item, Core::ptrModelValue::create(v));
            break;
        }
        case MMS_UTC_TIME: {
            QString v = convertTimestampMsToUserString(MmsValue_getUtcTimeInMs(t_mmsValue));
            t_vals->push(t_item, Core::ptrModelValue::create(v));
            break;
        }
        default: {
            char tmp[1024] = { 0 };
            MmsValue_printToBuffer(t_mmsValue, tmp, 1024);
            QString v = QString::fromLocal8Bit(tmp);
            t_vals->push(t_item, Core::ptrModelValue::create(v));
        }
        }
    }

    int     getValuesForDataAttribute(Core::ptrModelItem t_item, Core::ptrModelStateUpd t_vals,
                                      sIedConnection *t_con, FunctionalConstraint t_fc)
    {
        IedClientError retval = IED_ERROR_OK;
        auto ref = t_item->getReference().toStdString();

        MmsValue *rawValue = IedConnection_readObject(t_con, &retval, ref.data(), t_fc);
        if (retval != IED_ERROR_OK) {
            return -1;
        }

        getValuesForItemByMmsValue(t_item, t_vals, rawValue);

        MmsValue_delete(rawValue);
        return 0;
    }

    int     getValuesForDataObject(Core::ptrDO t_do, Core::ptrModelStateUpd t_vals,
                                   sIedConnection *t_con)
    {
        if (t_do == nullptr || t_vals == nullptr || t_con == nullptr) {
            return -1;
        }

        for (size_t j=0;j<t_do->getItemCount();j++) {
            auto daNode = t_do->getItem< Core::DataAttribute >(j);
            auto fcNum = (FunctionalConstraint)daNode->fcNum();

            getValuesForDataAttribute(daNode, t_vals, t_con, fcNum);
        }
        return 0;
    }
}

namespace Libiec61850
{
    Core::ptrModelStateUpd IED_StateAPI_Impl::getStatusForAllLD(Core::ptrDataModel t_model)
    {
        if (!m_api.isConnected()) {
            return nullptr;
        }

        auto vals = Core::ptrModelStateUpd::create();

        for (size_t i=0;i<t_model->getItemCount();i++) {
            auto ld = t_model->getItem< Core::LogicalDevice >(i);

            if (ld->lln0()) {
                getValuesForDataObject(ld->lln0()->getModItem(), vals, m_api.m_libConn);
                getValuesForDataObject(ld->lln0()->getBehItem(), vals, m_api.m_libConn);
                getValuesForDataObject(ld->lln0()->getHealthItem(), vals, m_api.m_libConn);
            }
        }
        return vals;
    }

    Core::ptrModelStateUpd IED_StateAPI_Impl::getStatusForAllLN(Core::ptrLD t_ld)
    {
        if (!m_api.isConnected()) {
            return nullptr;
        }

        auto vals = Core::ptrModelStateUpd::create();
        for (size_t i=0;i<t_ld->getItemCount();i++) {
            auto ln = t_ld->getItem< Core::LogicalNode >(i);

            for (size_t j=0;j<ln->getItemCount();j++) {
                getValuesForDataObject(ln->getModItem(), vals, m_api.m_libConn);
                getValuesForDataObject(ln->getBehItem(), vals, m_api.m_libConn);
                getValuesForDataObject(ln->getHealthItem(), vals, m_api.m_libConn);
            }
        }
        return vals;
    }

    Core::ptrModelStateUpd IED_StateAPI_Impl::getValsForLN(Core::ptrLN t_ln)
    {
        if (!m_api.isConnected()) {
            return nullptr;
        }

        auto vals = Core::ptrModelStateUpd::create();
        for (size_t i=0;i<t_ln->getItemCount();i++) {
            auto item = t_ln->getItem< Core::DataObject >(i);

            getValuesForDataObject(item, vals, m_api.m_libConn);
        }
        return vals;
    }

    Core::ptrModelStateUpd IED_StateAPI_Impl::getValsForDS(Core::ptrDataSet t_ds)
    {
        if (!m_api.isConnected()) {
            return nullptr;
        }

        //ClientDataSet clientDataSet = IedConnection_readDataSetValues(con, &error, "simpleIOGenericIO/LLN0.AnalogueValues", NULL);
        return nullptr;
    }
}