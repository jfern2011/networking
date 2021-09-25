#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "jfern/stream_channel.h"

namespace jfern {

class TcpServer {
public:
    TcpServer();

    TcpServer(const std::string& host, std::uint16_t port);

    ~TcpServer();

    std::weak_ptr<StreamChannel> Accept();

    NetError Reset();
    NetError Reset(const std::string& host, std::uint16_t port);

    NetError Start();
    NetError Stop();

private:
    std::vector<std::shared_ptr<StreamChannel>>
        m_channels;
};

}  // namespace jfern

#endif  // TCP_SERVER_H_
