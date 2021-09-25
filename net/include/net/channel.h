#ifndef CHANNEL_H_
#define CHANNEL_H_

#include "jfern/net.h"

namespace jfern {

class Channel {
public:
    Channel();

    virtual ~Channel();

    template <typename T>
    NetError Read(T* data);

    template <typename T>
    NetError Write(const T& data);

protected:
    DataBuffer m_buf;
};

}  // namespace jfern

#endif  // CHANNEL_H_
