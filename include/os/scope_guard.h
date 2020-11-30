#pragma once

#include <os/non_copyable.h>

#include <utility>

namespace os {

template <typename Callable>
class scope_guard : non_copyable<scope_guard<Callable>> {
public:

    constexpr explicit scope_guard(Callable&& cb) noexcept : mCallback{std::move(cb)}, mReleased{false} {}

    scope_guard(scope_guard&& other) noexcept : mCallback{std::move(other.mCallback)}, mReleased{other.mReleased} {
        other.release();
    }

    ~scope_guard() noexcept {
        if (mReleased == false) {
            std::invoke(mCallback);
        }
    }

    void release() noexcept { mReleased = true; }

    scope_guard& operator=(scope_guard&&) = delete;
private:
    Callable mCallback;
    bool     mReleased;
};

template <typename Callable>
scope_guard<Callable> make_scope_guard(Callable&& cb) noexcept {
    return scope_guard<Callable>{std::move(cb)};
}

} // namespace os
