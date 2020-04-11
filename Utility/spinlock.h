#ifndef SPINLOCK_HPP
#define SPINLOCK_HPP

#include <atomic>

namespace utility {

class spinlock {
public:
    spinlock() = default;
    ~spinlock() = default;
    spinlock& operator=(const spinlock&) = delete;

    // On successful lock acquisition returns true, otherwise returns false
    bool try_lock() noexcept;

    void lock() noexcept;
    void unlock() noexcept;

private:
    std::atomic_bool guard_ { false };
};

} // namespace utility

#endif