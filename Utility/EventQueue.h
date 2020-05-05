#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>

#include "game.grpc.pb.h"

namespace utility {

    class EventQueue final {
    public:
        EventQueue() : hasElement_(false) {}

        void push(::game::Event event) {
            std::unique_lock<std::mutex> locker(lockqueue_);
            events_.push(std::move(event));
            hasElement_.store(true);
            queuecheck_.notify_all();
        }

        ::game::Event front() {
            std::unique_lock<std::mutex> locker(lockqueue_);
            while (!hasElement_.load()) {
                queuecheck_.wait(locker);
            }
            if (!events_.empty()) {
                ::game::Event event = events_.front();
                events_.pop();
                if (events_.empty()) {
                    hasElement_.store(false);
                }
                return event;
            } else {
                std::cerr << "front()" << std::endl;
                throw (std::logic_error("front()"));
            }
        }

        void clear() {
            std::unique_lock<std::mutex> locker(lockqueue_);
            while (!events_.empty()) {
                events_.pop();
            }
            hasElement_.store(false);
        }

        bool empty() {
            return !hasElement_.load();
        }


    private:
        std::queue<::game::Event> events_;
        std::mutex lockqueue_;
        std::condition_variable queuecheck_;
        std::atomic_bool hasElement_;
    };

} // namespace utility

#endif