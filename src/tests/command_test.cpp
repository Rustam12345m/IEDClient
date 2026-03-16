#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QSignalSpy>

#include "mock_api.hpp"
#include "cmd/set_rcb_values_cmd.hpp"
#include "cmd/update_rcbs_cmd.hpp"
#include "cmd/connect_cmd.hpp"
#include "cmd/disconnect_cmd.hpp"

using ::testing::_;
using ::testing::Return;

namespace UnitTests
{
    // ─── Helper: create a mock API shared pointer ───────────────────

    static auto makeMockAPI()
    {
        return QSharedPointer<MockIEC61850API>::create();
    }

    // Helper: build an IED with a minimal model containing RCBs
    static Core::IED::ptr makeIEDWithRCBs(int t_count)
    {
        auto ied = Core::IED::ptr::create();

        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
               .createLN("LLN0");

        for (int i = 0; i < t_count; ++i) {
            builder.createRCB(QString("brcb%1").arg(i), "IEDNameA_LD_1/LLN0", true);
        }

        ied->setModel(builder.build(nullptr));
        return ied;
    }

    // ─── SetRCBValues_Cmd ───────────────────────────────────────────

    TEST(SetRCBValues, Enable_Success)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
               .createLN("LLN0")
               .createRCB("brcbAnalog", "IEDNameA_LD_1/LLN0", true);
        auto rcb = builder.lastRCB();

        auto cmd = Cmd::SetRCBValues_Cmd::create(
            rcb, true, 0x1F, 500, 2000, "rptId01", "dsAnalog");

        QSignalSpy spy(cmd.get(), &Cmd::CmdInterface::sigCmdEvent);

        auto api = makeMockAPI();
        EXPECT_CALL(api->mockControl(), setRCBValues(_, true, 0x1F, 500u, 2000u, _, _))
            .WillOnce(Return(true));

        cmd->execute(api);

        // RCB model should be updated
        EXPECT_TRUE(rcb->rptEna());
        EXPECT_EQ(rcb->trgOps(), 0x1F);
        EXPECT_EQ(rcb->bufTm(), 500u);
        EXPECT_EQ(rcb->intgPd(), 2000u);
        EXPECT_EQ(rcb->rptId(), "rptId01");
        EXPECT_EQ(rcb->dsRef(), "dsAnalog");

        // Should emit START + FINISH(true)
        ASSERT_EQ(spy.count(), 2);
        auto finishEvent = spy.at(1).at(0).value<Cmd::CmdEvent>();
        EXPECT_EQ(finishEvent.m_type, Cmd::FINISH_EVENT);
        EXPECT_TRUE(finishEvent.m_result);
    }

    TEST(SetRCBValues, Enable_Failure)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
               .createLN("LLN0")
               .createRCB("brcbAnalog", "IEDNameA_LD_1/LLN0", true);
        auto rcb = builder.lastRCB();

        auto cmd = Cmd::SetRCBValues_Cmd::create(
            rcb, true, 0x0F, 100, 1000, "rptId02", "dsTest");

        QSignalSpy spy(cmd.get(), &Cmd::CmdInterface::sigCmdEvent);

        auto api = makeMockAPI();
        EXPECT_CALL(api->mockControl(), setRCBValues(_, _, _, _, _, _, _))
            .WillOnce(Return(false));

        cmd->execute(api);

        // RCB model should NOT be updated on failure
        EXPECT_FALSE(rcb->rptEna());
        EXPECT_EQ(rcb->trgOps(), 0);
        EXPECT_EQ(rcb->bufTm(), 0u);

        // Should emit START + FINISH(false)
        ASSERT_EQ(spy.count(), 2);
        auto finishEvent = spy.at(1).at(0).value<Cmd::CmdEvent>();
        EXPECT_EQ(finishEvent.m_type, Cmd::FINISH_EVENT);
        EXPECT_FALSE(finishEvent.m_result);
    }

    TEST(SetRCBValues, Disable_Success)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
               .createLN("LLN0")
               .createRCB("brcbAnalog", "IEDNameA_LD_1/LLN0", true);
        auto rcb = builder.lastRCB();
        rcb->setRptEna(true); // Pre-enable

        auto cmd = Cmd::SetRCBValues_Cmd::create(
            rcb, false, 0, 0, 0, "", "");

        auto api = makeMockAPI();
        EXPECT_CALL(api->mockControl(), setRCBValues(_, false, _, _, _, _, _))
            .WillOnce(Return(true));

        cmd->execute(api);

        EXPECT_FALSE(rcb->rptEna());
    }

    TEST(SetRCBValues, RcbRef_Buffered)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
               .createLN("LLN0")
               .createRCB("brcb01", "IEDNameA_LD_1/LLN0", true);
        auto rcb = builder.lastRCB();

        auto cmd = Cmd::SetRCBValues_Cmd::create(rcb, true, 0, 0, 0, "", "");

        auto api = makeMockAPI();
        // Verify the RCB reference format: lnRef.BR.name
        QString expectedRef = "IEDNameA_LD_1/LLN0.BR.brcb01";
        EXPECT_CALL(api->mockControl(), setRCBValues(expectedRef, _, _, _, _, _, _))
            .WillOnce(Return(true));

        cmd->execute(api);
    }

    TEST(SetRCBValues, RcbRef_Unbuffered)
    {
        Core::DataModelBuilder builder;
        builder.createLD("IEDNameA_LD_1")
               .createLN("LLN0")
               .createRCB("urcb01", "IEDNameA_LD_1/LLN0", false);
        auto rcb = builder.lastRCB();

        auto cmd = Cmd::SetRCBValues_Cmd::create(rcb, true, 0, 0, 0, "", "");

        auto api = makeMockAPI();
        // Verify the RCB reference format: lnRef.RP.name for unbuffered
        QString expectedRef = "IEDNameA_LD_1/LLN0.RP.urcb01";
        EXPECT_CALL(api->mockControl(), setRCBValues(expectedRef, _, _, _, _, _, _))
            .WillOnce(Return(true));

        cmd->execute(api);
    }

    // ─── UpdateRCBs_Cmd ─────────────────────────────────────────────

    TEST(UpdateRCBs, Refresh_All)
    {
        auto ied = makeIEDWithRCBs(3);
        auto cmd = Cmd::UpdateRCBs_Cmd::create(ied);

        QSignalSpy spy(cmd.get(), &Cmd::CmdInterface::sigCmdEvent);

        auto api = makeMockAPI();
        EXPECT_CALL(api->mockControl(), refreshRCBValues(_))
            .Times(3)
            .WillRepeatedly(Return(true));

        cmd->execute(api);

        ASSERT_EQ(spy.count(), 2); // START + FINISH
        auto finishEvent = spy.at(1).at(0).value<Cmd::CmdEvent>();
        EXPECT_TRUE(finishEvent.m_result);
        EXPECT_TRUE(finishEvent.m_msg.contains("3 of 3"));
    }

    TEST(UpdateRCBs, Refresh_Partial)
    {
        auto ied = makeIEDWithRCBs(3);
        auto cmd = Cmd::UpdateRCBs_Cmd::create(ied);

        QSignalSpy spy(cmd.get(), &Cmd::CmdInterface::sigCmdEvent);

        auto api = makeMockAPI();
        EXPECT_CALL(api->mockControl(), refreshRCBValues(_))
            .WillOnce(Return(true))
            .WillOnce(Return(false))
            .WillOnce(Return(true));

        cmd->execute(api);

        ASSERT_EQ(spy.count(), 2);
        auto finishEvent = spy.at(1).at(0).value<Cmd::CmdEvent>();
        EXPECT_TRUE(finishEvent.m_result); // updated > 0
        EXPECT_TRUE(finishEvent.m_msg.contains("2 of 3"));
    }

    TEST(UpdateRCBs, Refresh_Empty)
    {
        auto ied = makeIEDWithRCBs(0);
        auto cmd = Cmd::UpdateRCBs_Cmd::create(ied);

        QSignalSpy spy(cmd.get(), &Cmd::CmdInterface::sigCmdEvent);

        auto api = makeMockAPI();
        // No calls expected since there are no RCBs
        EXPECT_CALL(api->mockControl(), refreshRCBValues(_)).Times(0);

        cmd->execute(api);

        ASSERT_EQ(spy.count(), 2);
        auto finishEvent = spy.at(1).at(0).value<Cmd::CmdEvent>();
        EXPECT_FALSE(finishEvent.m_result); // 0 updated → false
    }

    // ─── ConnectCmd ─────────────────────────────────────────────────

    TEST(ConnectCmd, Connect_Failure)
    {
        auto ied = Core::IED::ptr::create();
        Cmd::IEDCredentials creds("192.168.1.1", 102, false, "", "");

        auto cmd = Cmd::ConnectCmd::create(creds, ied);
        QSignalSpy spy(cmd.get(), &Cmd::CmdInterface::sigCmdEvent);

        auto api = makeMockAPI();
        EXPECT_CALL(*api, connect(_)).WillOnce(Return(false));
        // fetchDataModel should NOT be called when connect fails
        EXPECT_CALL(api->mockModel(), fetchDataModel(_)).Times(0);

        cmd->execute(api);

        ASSERT_GE(spy.count(), 2); // START + FINISH
        auto lastEvent = spy.last().at(0).value<Cmd::CmdEvent>();
        EXPECT_EQ(lastEvent.m_type, Cmd::FINISH_EVENT);
        EXPECT_FALSE(lastEvent.m_result);
    }

    // ─── DisConnectCmd ──────────────────────────────────────────────

    TEST(DisConnectCmd, Disconnect)
    {
        auto ied = Core::IED::ptr::create();
        auto cmd = Cmd::DisConnectCmd::create(ied);

        auto api = makeMockAPI();
        EXPECT_CALL(*api, disconnect()).Times(1);

        cmd->execute(api);
    }
}
