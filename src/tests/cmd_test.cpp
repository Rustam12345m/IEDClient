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

#include <gtest/gtest.h>
#include <QSignalSpy>
#include <QVariant>

#include "mock_api.hpp"
#include "cmd/control_cmd.hpp"
#include "cmd/write_value_cmd.hpp"
#include "core/sgcb.hpp"

using namespace testing;
using namespace Cmd;
using namespace Cmd::Interface;

namespace UnitTests
{
    // ── AddCause string mapping ──────────────────────────────────────

    TEST(ControlTypes, AddCauseToString_KnownValues)
    {
        EXPECT_EQ(addCauseToString(0),  "Unknown");
        EXPECT_EQ(addCauseToString(1),  "Not supported");
        EXPECT_EQ(addCauseToString(2),  "Blocked by switching hierarchy");
        EXPECT_EQ(addCauseToString(3),  "Select failed");
        EXPECT_EQ(addCauseToString(10), "Blocked by interlocking");
        EXPECT_EQ(addCauseToString(11), "Blocked by synchrocheck");
        EXPECT_EQ(addCauseToString(18), "Object not selected");
        EXPECT_EQ(addCauseToString(25), "None");
    }

    TEST(ControlTypes, AddCauseToString_UnknownValues)
    {
        EXPECT_EQ(addCauseToString(99),  "Unknown (99)");
        EXPECT_EQ(addCauseToString(-1),  "Unknown (-1)");
    }

    // ── ControlOperate_Cmd tests ─────────────────────────────────────

    class ControlOperateTest : public ::testing::Test
    {
    protected:
        void SetUp() override {
            m_api = QSharedPointer<MockIEC61850API>::create();
        }

        QSharedPointer<MockIEC61850API> m_api;
    };

    TEST_F(ControlOperateTest, DirectNormal_Operate_Success)
    {
        EXPECT_CALL(m_api->mockControl(), controlOperate(
            QString("LD0/LLN0.GGIO1.SPCSO1"),
            CtlModel::DirectNormal,
            CtlValType::Boolean,
            _))
            .WillOnce(Return(true));

        auto cmd = ControlOperate_Cmd::create(
            "LD0/LLN0.GGIO1.SPCSO1",
            ControlOperate_Cmd::Action::Operate,
            CtlModel::DirectNormal,
            CtlValType::Boolean,
            QVariant(true));

        QSignalSpy resultSpy(cmd.get(), &ControlOperate_Cmd::sigControlResult);
        QSignalSpy eventSpy(cmd.get(), &CmdInterface::sigCmdEvent);

        cmd->execute(m_api);

        ASSERT_EQ(resultSpy.count(), 1);
        auto args = resultSpy.takeFirst();
        EXPECT_EQ(args.at(0).toString(), "LD0/LLN0.GGIO1.SPCSO1");
        EXPECT_TRUE(args.at(1).toBool());

        // Verify FINISH event with result=true
        ASSERT_GE(eventSpy.count(), 2); // START + FINISH
        auto finishArgs = eventSpy.last();
        CmdEvent ev = finishArgs.at(0).value<CmdEvent>();
        EXPECT_EQ(ev.m_type, FINISH_EVENT);
        EXPECT_TRUE(ev.m_result);
    }

    TEST_F(ControlOperateTest, DirectNormal_Operate_Failure)
    {
        EXPECT_CALL(m_api->mockControl(), controlOperate(_, _, _, _))
            .WillOnce(Return(false));

        auto cmd = ControlOperate_Cmd::create(
            "LD0/LLN0.GGIO1.SPCSO1",
            ControlOperate_Cmd::Action::Operate,
            CtlModel::DirectNormal,
            CtlValType::Boolean,
            QVariant(true));

        QSignalSpy resultSpy(cmd.get(), &ControlOperate_Cmd::sigControlResult);

        cmd->execute(m_api);

        ASSERT_EQ(resultSpy.count(), 1);
        auto args = resultSpy.takeFirst();
        EXPECT_FALSE(args.at(1).toBool());
    }

    TEST_F(ControlOperateTest, SBONormal_Select_Success)
    {
        EXPECT_CALL(m_api->mockControl(), controlSelect(
            QString("LD0/LLN0.GGIO1.SPCSO1"),
            CtlModel::SBONormal,
            CtlValType::Boolean,
            _))
            .WillOnce(Return(true));

        auto cmd = ControlOperate_Cmd::create(
            "LD0/LLN0.GGIO1.SPCSO1",
            ControlOperate_Cmd::Action::Select,
            CtlModel::SBONormal,
            CtlValType::Boolean,
            QVariant(true));

        QSignalSpy resultSpy(cmd.get(), &ControlOperate_Cmd::sigControlResult);

        cmd->execute(m_api);

        ASSERT_EQ(resultSpy.count(), 1);
        auto args = resultSpy.takeFirst();
        EXPECT_TRUE(args.at(1).toBool());
    }

    TEST_F(ControlOperateTest, SBOEnhanced_Select_Success)
    {
        EXPECT_CALL(m_api->mockControl(), controlSelect(
            QString("LD0/LLN0.GGIO1.SPCSO1"),
            CtlModel::SBOEnhanced,
            CtlValType::Boolean,
            _))
            .WillOnce(Return(true));

        auto cmd = ControlOperate_Cmd::create(
            "LD0/LLN0.GGIO1.SPCSO1",
            ControlOperate_Cmd::Action::Select,
            CtlModel::SBOEnhanced,
            CtlValType::Boolean,
            QVariant(false));

        QSignalSpy resultSpy(cmd.get(), &ControlOperate_Cmd::sigControlResult);

        cmd->execute(m_api);

        ASSERT_EQ(resultSpy.count(), 1);
        auto args = resultSpy.takeFirst();
        EXPECT_TRUE(args.at(1).toBool());
    }

    TEST_F(ControlOperateTest, Cancel_Success)
    {
        EXPECT_CALL(m_api->mockControl(), controlCancel(
            QString("LD0/LLN0.GGIO1.SPCSO1")))
            .WillOnce(Return(true));

        auto cmd = ControlOperate_Cmd::create(
            "LD0/LLN0.GGIO1.SPCSO1",
            ControlOperate_Cmd::Action::Cancel,
            CtlModel::StatusOnly,
            CtlValType::Unknown,
            QVariant());

        QSignalSpy resultSpy(cmd.get(), &ControlOperate_Cmd::sigControlResult);

        cmd->execute(m_api);

        ASSERT_EQ(resultSpy.count(), 1);
        auto args = resultSpy.takeFirst();
        EXPECT_TRUE(args.at(1).toBool());
    }

    // ── WriteValue_Cmd tests ─────────────────────────────────────────

    class WriteValueTest : public ::testing::Test
    {
    protected:
        void SetUp() override {
            m_api = QSharedPointer<MockIEC61850API>::create();
        }

        QSharedPointer<MockIEC61850API> m_api;
    };

    TEST_F(WriteValueTest, Write_Success)
    {
        EXPECT_CALL(m_api->mockState(), writeValueByRef(
            QString("LD0/PTOC1.StrVal.setMag.f"),
            QString("SP"),
            QString("1.5")))
            .WillOnce(Return(QString()));

        auto cmd = WriteValue_Cmd::create(
            "LD0/PTOC1.StrVal.setMag.f", "SP", "1.5");

        QSignalSpy resultSpy(cmd.get(), &WriteValue_Cmd::sigWriteResult);
        QSignalSpy eventSpy(cmd.get(), &CmdInterface::sigCmdEvent);

        cmd->execute(m_api);

        ASSERT_EQ(resultSpy.count(), 1);
        auto args = resultSpy.takeFirst();
        EXPECT_EQ(args.at(0).toString(), "LD0/PTOC1.StrVal.setMag.f");
        EXPECT_TRUE(args.at(1).toBool());

        ASSERT_GE(eventSpy.count(), 2);
        CmdEvent ev = eventSpy.last().at(0).value<CmdEvent>();
        EXPECT_EQ(ev.m_type, FINISH_EVENT);
        EXPECT_TRUE(ev.m_result);
    }

    TEST_F(WriteValueTest, Write_Failure)
    {
        EXPECT_CALL(m_api->mockState(), writeValueByRef(_, _, _))
            .WillOnce(Return(QString("Write failed: access-denied")));

        auto cmd = WriteValue_Cmd::create(
            "LD0/PTOC1.StrVal.setMag.f", "CF", "999");

        QSignalSpy resultSpy(cmd.get(), &WriteValue_Cmd::sigWriteResult);

        cmd->execute(m_api);

        ASSERT_EQ(resultSpy.count(), 1);
        auto args = resultSpy.takeFirst();
        EXPECT_FALSE(args.at(1).toBool());
    }

    // ── SGCB model tests ─────────────────────────────────────────────

    TEST(SGCBModel, DefaultValues)
    {
        auto sgcb = Core::SGCB::ptr::create("LD0", 5, 1);

        EXPECT_EQ(sgcb->ldRef(), "LD0");
        EXPECT_EQ(sgcb->sgcbRef(), "LD0/LLN0.SGCB");
        EXPECT_EQ(sgcb->numOfSG(), 5);
        EXPECT_EQ(sgcb->actSG(), 1);
        EXPECT_EQ(sgcb->editSG(), 0);
        EXPECT_FALSE(sgcb->cnfEdit());
        EXPECT_EQ(sgcb->lActTm(), 0u);
        EXPECT_EQ(sgcb->resvTms(), 0);
    }

    TEST(SGCBModel, SettersAndGetters)
    {
        auto sgcb = Core::SGCB::ptr::create("LD0", 3, 1);

        sgcb->setActSG(2);
        sgcb->setEditSG(1);
        sgcb->setCnfEdit(true);
        sgcb->setLActTm(1711234567000ULL);
        sgcb->setResvTms(300);

        EXPECT_EQ(sgcb->actSG(), 2);
        EXPECT_EQ(sgcb->editSG(), 1);
        EXPECT_TRUE(sgcb->cnfEdit());
        EXPECT_EQ(sgcb->lActTm(), 1711234567000ULL);
        EXPECT_EQ(sgcb->resvTms(), 300);
    }
}
