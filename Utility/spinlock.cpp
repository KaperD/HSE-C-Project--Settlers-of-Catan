#include <iostream>
#include <chrono>
#include <thread>

#include "spinlock.h"

namespace utility {

namespace {

void yield(size_t k) noexcept {
    if (k < 8) { 
        // pass
    } else if(k < 128) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

} // namespace

bool spinlock::try_lock() noexcept {
    return !(guard_.exchange(true));
}

void spinlock::lock() noexcept {
    for (size_t k = 0; !try_lock(); ++k) { 
        yield(k);
    }
}

void spinlock::unlock() noexcept {
    guard_.exchange(false);
}

} // namespace utility
