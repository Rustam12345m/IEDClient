#include <gtest/gtest.h>
#include <QSignalSpy>

#include "app/event_storage.hpp"

namespace UnitTests
{
    TEST(EventStorage, AddEvent_Single)
    {
        App::EventStorage storage;

        auto ev = Cmd::CmdEvent::StartEvent("192.168.1.1", "Test message");
        storage.putEventToStorage(ev);

        EXPECT_EQ(storage.getLastMessage(), "Test message");
    }

    TEST(EventStorage, AddEvent_FillBuffer)
    {
        App::EventStorage storage;

        for (int i = 0; i < App::EventStorage::Capacity; ++i) {
            auto ev = Cmd::CmdEvent::StartEvent("ip", QString("msg_%1").arg(i));
            storage.putEventToStorage(ev);
        }

        // Last message should be the last one added
        EXPECT_EQ(storage.getLastMessage(),
                  QString("msg_%1").arg(App::EventStorage::Capacity - 1));
    }

    TEST(EventStorage, AddEvent_Overflow)
    {
        App::EventStorage storage;
        constexpr int total = 300;

        for (int i = 0; i < total; ++i) {
            auto ev = Cmd::CmdEvent::StartEvent("ip", QString("msg_%1").arg(i));
            storage.putEventToStorage(ev);
        }

        // After overflow, newest message should still be the last added
        EXPECT_EQ(storage.getLastMessage(), QString("msg_%1").arg(total - 1));
    }

    TEST(EventStorage, EmptyStorage_ReturnsEmpty)
    {
        App::EventStorage storage;
        EXPECT_EQ(storage.getLastMessage(), "");
    }

    TEST(EventStorage, Signal_Emitted)
    {
        App::EventStorage storage;
        QSignalSpy spy(&storage, &App::EventStorage::sigNewEvent);

        auto ev1 = Cmd::CmdEvent::StartEvent("ip1", "first");
        auto ev2 = Cmd::CmdEvent::FinishEvent("ip2", "second", true);

        storage.putEventToStorage(ev1);
        storage.putEventToStorage(ev2);

        ASSERT_EQ(spy.count(), 2);

        auto emitted1 = spy.at(0).at(0).value<Cmd::CmdEvent>();
        EXPECT_EQ(emitted1.m_msg, "first");
        EXPECT_EQ(emitted1.m_type, Cmd::START_EVENT);

        auto emitted2 = spy.at(1).at(0).value<Cmd::CmdEvent>();
        EXPECT_EQ(emitted2.m_msg, "second");
        EXPECT_EQ(emitted2.m_type, Cmd::FINISH_EVENT);
        EXPECT_TRUE(emitted2.m_result);
    }
}
