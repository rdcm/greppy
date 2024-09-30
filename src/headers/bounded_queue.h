#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

template <typename T> class BoundedQueue {
  private:
    std::queue<T> queue_;
    std::mutex mutex_;
    size_t capacity_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
    bool is_completed_ = false;

  public:
    BoundedQueue(size_t capacity) : capacity_(capacity), is_completed_(false) {}

    void complete() {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            is_completed_ = true;
        }
        not_empty_.notify_all();
    }

    bool is_completed() { return is_completed_; }

    bool size() const { return queue_.size(); }

    void push(const T &item) {
        std::unique_lock<std::mutex> lock(mutex_);
        not_full_.wait(lock, [this]() { return queue_.size() < capacity_; });

        queue_.push(std::move(item));
        not_empty_.notify_one();
    }

    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this]() { return !queue_.empty() || is_completed_; });

        if (queue_.empty() && is_completed_) {
            return {};
        }

        T item = std::move(queue_.front());
        queue_.pop();
        not_full_.notify_one();
        return item;
    }
};
