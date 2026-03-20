#pragma once

#include <gmock/gmock.h>

#include "cmd/interface/iec61850_api.hpp"

namespace UnitTests
{
    class MockControlAPI : public Cmd::Interface::IED_ControlAPI
    {
    public:
        MOCK_METHOD(bool, setRCBValues,
                    (const QString &t_rcbRef, bool t_enable,
                     int t_trgOps, uint32_t t_bufTm, uint32_t t_intgPd,
                     const QString &t_rptId, const QString &t_datSet),
                    (override));

        MOCK_METHOD(bool, refreshRCBValues,
                    (Core::ReportBlock::ptr t_rcb),
                    (override));

        MOCK_METHOD(bool, refreshGOOSEValues,
                    (Core::GooseControlBlock::ptr t_gocb),
                    (override));

        MOCK_METHOD(QString, setGOOSEEnable,
                    (const QString &t_gocbRef, bool t_enable),
                    (override));

        MOCK_METHOD(bool, refreshSVValues,
                    (Core::SV_ControlBlock::ptr t_svcb),
                    (override));

        MOCK_METHOD(QString, setSVEnable,
                    (const QString &t_svcbRef, bool t_enable),
                    (override));

        MOCK_METHOD(bool, installReportHandler,
                    (const QString &t_rcbRef, const QString &t_rptId,
                     Core::ReportStorage *t_storage),
                    (override));

        MOCK_METHOD(void, uninstallReportHandler,
                    (const QString &t_rcbRef),
                    (override));

        MOCK_METHOD(Cmd::Interface::ControlInfo, getControlInfo,
                    (const QString &t_objRef),
                    (override));

        MOCK_METHOD(bool, controlOperate,
                    (const QString &t_objRef, Cmd::Interface::CtlModel t_model,
                     Cmd::Interface::CtlValType t_valType, const QVariant &t_value),
                    (override));

        MOCK_METHOD(bool, controlSelect,
                    (const QString &t_objRef, Cmd::Interface::CtlModel t_model,
                     Cmd::Interface::CtlValType t_valType, const QVariant &t_value),
                    (override));

        MOCK_METHOD(bool, controlCancel,
                    (const QString &t_objRef),
                    (override));
    };

    class MockStateAPI : public Cmd::Interface::IED_StateAPI
    {
    public:
        MOCK_METHOD(Core::ModelStateUpdater::ptr, getStatusForAllLD,
                    (Core::DataModel::ptr t_model), (override));
        MOCK_METHOD(Core::ModelStateUpdater::ptr, getStatusForAllLN,
                    (Core::LogicalDevice::ptr t_ld), (override));
        MOCK_METHOD(Core::ModelStateUpdater::ptr, getValsForLN,
                    (Core::LogicalNode::ptr t_ln), (override));
        MOCK_METHOD(Core::ModelStateUpdater::ptr, getValsForDS,
                    (Core::DataSet::ptr t_ds), (override));
    };

    class MockModelAPI : public Cmd::Interface::IED_ModelAPI
    {
        Q_OBJECT
    public:
        MOCK_METHOD(int, fetchDataModel,
                    (Core::DataModelBuilder &t_builder), (override));
    };

    class MockFS_API : public Cmd::Interface::IED_FS_API
    {
        Q_OBJECT
    public:
        MOCK_METHOD(int, getFileList, (Core::DirOn &t_dir), (override));
        MOCK_METHOD(bool, download, (const QString &t_filename, const QString &t_localPath), (override));
        MOCK_METHOD(int, remove, (const QString &t_filename), (override));
    };

    class MockIEC61850API : public Cmd::Interface::IEC61850_API
    {
        Q_OBJECT
    public:
        MockIEC61850API()
            : m_mockControl{}, m_mockState{}, m_mockModel{}, m_mockFS{}
        {}

        MOCK_METHOD(QString, connect, (const Cmd::IEDCredentials &t_creds), (override));
        MOCK_METHOD(void, disconnect, (), (override));
        MOCK_METHOD(bool, isConnected, (), (const, override));
        MOCK_METHOD(QString, getVersion, (), (const, override));
        MOCK_METHOD(Core::DevServIdentity, getServIdentity, (), (const, override));

        Cmd::Interface::IED_ModelAPI&   model() override   { return m_mockModel; }
        Cmd::Interface::IED_StateAPI&   state() override   { return m_mockState; }
        Cmd::Interface::IED_ControlAPI& control() override { return m_mockControl; }
        Cmd::Interface::IED_FS_API&     fs() override      { return m_mockFS; }

        MockControlAPI& mockControl() { return m_mockControl; }
        MockStateAPI&   mockState()   { return m_mockState; }
        MockModelAPI&   mockModel()   { return m_mockModel; }
        MockFS_API&     mockFS()      { return m_mockFS; }

    private:
        MockControlAPI m_mockControl;
        MockStateAPI   m_mockState;
        MockModelAPI   m_mockModel;
        MockFS_API     m_mockFS;
    };
}
