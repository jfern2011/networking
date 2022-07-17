#include <cstdint>
#include <limits.h>

#include "gtest/gtest.h"
#include "networking/net.h"

namespace {
TEST(net, byte_swap_I8) {
    for (std::int8_t i = std::numeric_limits<std::int8_t>::min();
         i <= 0; i++) {
        ASSERT_EQ(jfern::byte_swap(i), i);
    }

    for (std::int8_t i = std::numeric_limits<std::int8_t>::max();
         i >= 0; i--) {
        ASSERT_EQ(jfern::byte_swap(i), i);
    }
}

TEST(net, byte_swap_I16) {
    std::int16_t valueI16 = 0xeedd;

    std::int16_t actual = jfern::byte_swap(valueI16);
    std::int16_t expected = 0xddee;

    EXPECT_EQ(actual, expected);

    valueI16 = 0x7edd;

    actual = jfern::byte_swap(valueI16);
    expected = 0xdd7e;

    EXPECT_EQ(actual, expected);

    valueI16 = -1;
    EXPECT_EQ(jfern::byte_swap(valueI16), valueI16);

    valueI16 = 0;
    EXPECT_EQ(jfern::byte_swap(valueI16), valueI16);
}

TEST(net, byte_swap_I32) {
    std::int32_t valueI32 = 0xeeddccbb;

    std::int32_t actual = jfern::byte_swap(valueI32);
    std::int32_t expected = 0xbbccddee;

    EXPECT_EQ(actual, expected);

    valueI32 = 0x7eddccbb;

    actual = jfern::byte_swap(valueI32);
    expected = 0xbbccdd7e;

    EXPECT_EQ(actual, expected);

    valueI32 = -1;
    EXPECT_EQ(jfern::byte_swap(valueI32), valueI32);

    valueI32 = 0;
    EXPECT_EQ(jfern::byte_swap(valueI32), valueI32);
}

TEST(net, byte_swap_I64) {
    std::int64_t valueI64 = 0xeeddccbbaa998877;

    std::int64_t actual = jfern::byte_swap(valueI64);
    std::int64_t expected = 0x778899aabbccddee;

    EXPECT_EQ(actual, expected);

    valueI64 = 0x7eddccbbaa998877;

    actual = jfern::byte_swap(valueI64);
    expected = 0x778899aabbccdd7e;

    EXPECT_EQ(actual, expected);

    valueI64 = -1;
    EXPECT_EQ(jfern::byte_swap(valueI64), valueI64);

    valueI64 = 0;
    EXPECT_EQ(jfern::byte_swap(valueI64), valueI64);
}

TEST(net, byte_swap_U8) {
    for (std::uint8_t i = std::numeric_limits<std::uint8_t>::max();
         i >= 0; i--) {
        ASSERT_EQ(jfern::byte_swap(i), i);

        if (i == 0) break;
    }
}

TEST(net, byte_swap_U16) {
    std::uint16_t valueU16 = 0xeedd;

    std::uint16_t actual = jfern::byte_swap(valueU16);
    std::uint16_t expected = 0xddee;

    EXPECT_EQ(actual, expected);

    valueU16 = 0x7edd;

    actual = jfern::byte_swap(valueU16);
    expected = 0xdd7e;

    EXPECT_EQ(actual, expected);

    valueU16 = 0xffff;
    EXPECT_EQ(jfern::byte_swap(valueU16), valueU16);

    valueU16 = 0;
    EXPECT_EQ(jfern::byte_swap(valueU16), valueU16);
}

TEST(net, byte_swap_U32) {
    std::uint32_t valueU32 = 0xeeddccbb;

    std::uint32_t actual = jfern::byte_swap(valueU32);
    std::uint32_t expected = 0xbbccddee;

    EXPECT_EQ(actual, expected);

    valueU32 = 0x7eddccbb;

    actual = jfern::byte_swap(valueU32);
    expected = 0xbbccdd7e;

    EXPECT_EQ(actual, expected);

    valueU32 = 0xffffffff;
    EXPECT_EQ(jfern::byte_swap(valueU32), valueU32);

    valueU32 = 0;
    EXPECT_EQ(jfern::byte_swap(valueU32), valueU32);
}

TEST(net, byte_swap_U64) {
    std::uint64_t valueU64 = 0xeeddccbbaa998877;

    std::uint64_t actual = jfern::byte_swap(valueU64);
    std::uint64_t expected = 0x778899aabbccddee;

    EXPECT_EQ(actual, expected);

    valueU64 = 0x7eddccbbaa998877;

    actual = jfern::byte_swap(valueU64);
    expected = 0x778899aabbccdd7e;

    EXPECT_EQ(actual, expected);

    valueU64 = -1;
    EXPECT_EQ(jfern::byte_swap(valueU64), valueU64);

    valueU64 = 0;
    EXPECT_EQ(jfern::byte_swap(valueU64), valueU64);
}

}  // namespace
