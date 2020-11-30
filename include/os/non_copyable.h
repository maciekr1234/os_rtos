#pragma once

namespace os {

template <typename T>
class non_copyable {
public:
    non_copyable(const non_copyable&) = delete;
    non_copyable& operator=(const non_copyable&) = delete;

protected:
    non_copyable()  = default;
    ~non_copyable() = default;
};

template <typename T>
class non_moveable {
public:
    non_moveable(const non_moveable&) = delete;
    non_moveable& operator=(const non_moveable&) = delete;

protected:
    non_moveable()  = default;
    ~non_moveable() = default;
};


} // namespace os
