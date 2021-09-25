#ifndef STREAM_CHANNEL_H_
#define STREAM_CHANNEL_H_

#include <cstddef>
#include <cstdint>
#include <string>

#include "jfern/net.h"

namespace jfern {

class StreamChannel final : public Channel {
public:
    StreamChannel(const std::string& host, std::uint16_t);

    ~StreamChannel();

    NetError Close() override;

    NetError Read(std::uint8_t* data, std::size_t* nbytes) override;

    NetError Write(const std::uint8_t* data, std::size_t nbytes) override;

    template <typename T>
    StreamChannel& operator<<(const T& data);

    template <typename T>
    StreamChannel& operator>>(const T& data);

private:
    NetError Flush() override;
};

}  // namespace jfern

#endif  // STREAM_CHANNEL_H_
