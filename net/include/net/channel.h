/**
 *  \file   channel.h
 *  \author Jason Fernandez
 *  \date   09/26/2021
 *
 *  Copyright 2021 Jason Fernandez
 *
 *  https://github.com/jfern2011/io_tools
 */

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include "jfern/data_buffer.h"
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
