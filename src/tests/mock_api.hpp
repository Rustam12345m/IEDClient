#pragma once

#include <gmock/gmock.h>

#include "cmd/interface/iec61850_api.hpp"

namespace UnitTests
{
    class MockControlAPI : public Cmd::Interface::IED_ControlAPI
    {
    public:
        MOCK_METHOD(bool, setRCBValues,
                    (const QString &rcbRef, bool enable,
                     int trgOps, uint32_t bufTm, uint32_t intgPd,
                     const QString &rptId, const QString &datSet),
                    (override));

        MOCK_METHOD(bool, refreshRCBValues,
                    (Core::ReportBlock::ptr rcb),
                    (override));

        MOCK_METHOD(bool, refreshGOOSEValues,
                    (Core::GooseControlBlock::ptr gocb),
                    (override));

        MOCK_METHOD(QString, setGOOSEEnable,
                    (const QString &gocbRef, bool enable),
                    (override));

        MOCK_METHOD(bool, refreshSVValues,
                    (Core::SV_ControlBlock::ptr svcb),
                    (override));

        MOCK_METHOD(QString, setSVEnable,
                    (const QString &svcbRef, bool enable),
                    (override));

        MOCK_METHOD(bool, installReportHandler,
                    (const QString &rcbRef, const QString &rptId,
                     Core::ReportStorage *storage),
                    (override));

        MOCK_METHOD(void, uninstallReportHandler,
                    (const QString &rcbRef),
                    (override));

        MOCK_METHOD(Cmd::Interface::ControlInfo, getControlInfo,
                    (const QString &objRef),
                    (override));

        MOCK_METHOD(bool, controlOperate,
                    (const QString &objRef, Cmd::Interface::CtlModel model,
                     Cmd::Interface::CtlValType valType, const QVariant &value),
                    (override));

        MOCK_METHOD(bool, controlSelect,
                    (const QString &objRef, Cmd::Interface::CtlModel model,
                     Cmd::Interface::CtlValType valType, const QVariant &value),
                    (override));

        MOCK_METHOD(bool, controlCancel,
                    (const QString &objRef),
                    (override));

        MOCK_METHOD(void, setTestMode, (bool), (override));
        MOCK_METHOD(void, setInterlockCheck, (bool), (override));
        MOCK_METHOD(void, setSynchroCheck, (bool), (override));
        MOCK_METHOD(void, setOrigin, (const QString &, int), (override));
    };

    class MockStateAPI : public Cmd::Interface::IED_StateAPI
    {
    public:
        MOCK_METHOD(Core::ModelStateUpdater::ptr, getStatusForAllLD,
                    (Core::DataModel::ptr model), (override));
        MOCK_METHOD(Core::ModelStateUpdater::ptr, getStatusForAllLN,
                    (Core::LogicalDevice::ptr ld), (override));
        MOCK_METHOD(Core::ModelStateUpdater::ptr, getValsForLN,
                    (Core::LogicalNode::ptr ln), (override));
        MOCK_METHOD(Core::ModelStateUpdater::ptr, getValsForDS,
                    (Core::DataSet::ptr ds), (override));
        MOCK_METHOD(QVariantList, readValuesByRef,
                    (const QStringList &refs, const QStringList &fcs), (override));

        MOCK_METHOD(QString, writeValueByRef,
                    (const QString &ref, const QString &fc, const QString &value), (override));
    };

    class MockModelAPI : public Cmd::Interface::IED_ModelAPI
    {
        Q_OBJECT
    public:
        MOCK_METHOD(int, fetchDataModel,
                    (Core::DataModelBuilder &builder), (override));
    };

    class MockFS_API : public Cmd::Interface::IED_FS_API
    {
        Q_OBJECT
    public:
        MOCK_METHOD(int, getFileList, (Core::DirOn &dir), (override));
        MOCK_METHOD(bool, download, (const QString &filename, const QString &localPath, uint32_t fileSize), (override));
        MOCK_METHOD(int, remove, (const QString &filename), (override));
    };

    class MockIEC61850API : public Cmd::Interface::IEC61850_API
    {
        Q_OBJECT
    public:
        MockIEC61850API()
            : m_mockControl{}, m_mockState{}, m_mockModel{}, m_mockFS{}
        {}

        MOCK_METHOD(QString, connect, (const Cmd::IEDCredentials &creds), (override));
        MOCK_METHOD(void, disconnect, (), (override));
        MOCK_METHOD(bool, isConnected, (), (const, override));
        MOCK_METHOD(QString, getVersion, (), (const, override));
        MOCK_METHOD(Core::DevServIdentity, getServIdentity, (), (const, override));

        Cmd::Interface::IED_ModelAPI&   model() override   { return m_mockModel; }
        Cmd::Interface::IED_StateAPI&   state() override   { return m_mockState; }
        Cmd::Interface::IED_ControlAPI& control() override { return m_mockControl; }
        Cmd::Interface::IED_FS_API&     fs() override      { return m_mockFS; }

        uint64_t getTxBytes() const override { return 0; }
        uint64_t getRxBytes() const override { return 0; }

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
