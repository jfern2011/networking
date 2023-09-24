#include <array>
#include <cstddef>
#include <memory>
#include <thread>

#include "gtest/gtest.h"
#include "networking/fd_internal.h"

namespace {
TEST(shared_internal, reference_counting) {
    constexpr std::size_t num_threads = 10;
    constexpr std::size_t sum_per_thread = 1000;

    std::array<std::shared_ptr<std::thread>, num_threads> threads;

    jfern::fd::shared_internal internal(1);

    auto task1 = [&]() -> void {
        for (std::size_t count = 1; count <= sum_per_thread; count++) {
            internal.add_reference();
        }
    };

    for (auto& worker : threads) {
        worker = std::make_shared<std::thread>(task1);
    }

    for (auto& worker : threads) {
        worker->join();
    }

    const std::size_t expected_sum = sum_per_thread * num_threads;

    EXPECT_EQ(internal.count(), expected_sum);
}
}  // namespace
