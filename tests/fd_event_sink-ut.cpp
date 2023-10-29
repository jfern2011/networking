/**
 *  \file   fd_event_sink-ut.cpp
 *  \author Jason Fernandez
 *  \date   09/24/2023
 *
 *  https://github.com/jfern2011/networking
 */

#include <algorithm>
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "fd_interface_mock.h"
#include "networking/fd_event_sink.h"
#include "networking/fd_interface.h"

namespace {
class FdEventSinkTest : public testing::Test {
protected:
    constexpr static std::array<short, 15> events = {
        0x00001,
        0x00002,
        0x00004,
        0x00008,
        0x00010,
        0x00020,
        0x00040,
        0x00080,
        0x00100,
        0x00200,
        0x00400,
        0x00800,
        0x01000,
        0x02000,
        0x04000
    };

    struct CallbackInfo {
        void handle_events(short events, jfern::fd_interface& fd) {
            input_args.emplace_back(events, fd.get());
        }

        std::vector<std::pair<short, int>>
            input_args;
    };

    FdEventSinkTest() : m_infos(events.size()) {}

    std::vector<CallbackInfo> m_infos;
};

TEST(fd_event_sink, invalid_events) {
    auto fd = std::make_unique<fd_interface_mock>();

    jfern::fd_event_sink sink(std::move(fd));

    ASSERT_FALSE(sink.add_events(0, nullptr));
    ASSERT_FALSE(sink.add_events(0, [](short, jfern::fd_interface& ) {}));
    ASSERT_FALSE(sink.add_events(1, nullptr));

    ASSERT_TRUE(sink.add_events(1, [](short, jfern::fd_interface& ) {}));
}

TEST_F(FdEventSinkTest, single_bit_events) {
    auto fd = std::make_unique<fd_interface_mock>();

    const std::size_t nCalls = 1 + events.size();

    EXPECT_CALL(*fd, get())
        .Times(nCalls).WillRepeatedly(testing::Return(0xabe));

    const int fd_ = fd->get();

    jfern::fd_event_sink sink(std::move(fd));

    for (std::size_t i = 0; i < events.size(); i++) {
        const short event = events[i];
        const auto handler = std::bind(&CallbackInfo::handle_events,
                                       &m_infos[i],
                                       std::placeholders::_1,
                                       std::placeholders::_2);

        ASSERT_TRUE(sink.add_events(event, handler));
    }

    std::for_each(events.begin(), events.end(),
                  [&](short events) { sink.handle_events(events); });
    
    for (std::size_t i = 0; i < events.size(); i++) {
        const CallbackInfo& info = m_infos[i];

        const short expected_event = events[i];
        const std::vector<std::pair<short, int>>& args = info.input_args;

        ASSERT_EQ(args.size(), 1u);

        EXPECT_EQ(args[0].first, expected_event);
        EXPECT_EQ(args[0].second, fd_);
    }

    // Remove events

    // Replace events
}

}  // namespace
