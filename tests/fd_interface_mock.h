/**
 *  \file   fd_interface_mock.h
 *  \author Jason Fernandez
 *  \date   09/24/2023
 *
 *  https://github.com/jfern2011/networking
 */

#ifndef NETWORKING_TESTS_FD_INTERFACE_MOCK_H_
#define NETWORKING_TESTS_FD_INTERFACE_MOCK_H_

#include "gmock/gmock.h"

#include "networking/fd_interface.h"

namespace {
/**
 * Mock file descriptor API
 */
class fd_interface_mock : public jfern::fd_interface {
public:
    MOCK_METHOD(int,  get,          (),                        (const, noexcept, override));
    MOCK_METHOD(bool, is_blocking,  (),                        (const, noexcept, override));
    MOCK_METHOD(int,  poll,         (short events),            (noexcept, override));
    MOCK_METHOD(int,  poll,         (short events,
                                     event_handler_t handler), (override));
    MOCK_METHOD(int,  poll,         (short events,
                                     int timeout),             (noexcept, override));
    MOCK_METHOD(int,  poll,         (short events,
                                     int timeout,
                                     event_handler_t handler), (override));
    MOCK_METHOD(bool, reset,        (int fd),                  (noexcept, override));
    MOCK_METHOD(bool, set_blocking, (bool enable),             (noexcept, override));

    MOCK_METHOD(bool, valid, (), (const, noexcept));

    /**
     * gMock doesn't work well with overloaded operators. This overload wraps
     * the mock function valid()
     */
    explicit operator bool() const noexcept override {
        return valid();
    }
};

}  // anonymous namespace

#endif  // NETWORKING_TESTS_FD_INTERFACE_MOCK_H_
