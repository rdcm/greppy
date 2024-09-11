#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

using namespace std;

template <typename T> class BoundedQueue {
  private:
    queue<T> queue_;
    mutex mutex_;
    size_t capacity_;
    condition_variable not_empty_;
    condition_variable not_full_;
    bool is_completed_ = false;

  public:
    BoundedQueue(size_t capacity) : capacity_(capacity), is_completed_(false) {}

    void complete() {
        {
            unique_lock<mutex> lock(mutex_);
            is_completed_ = true;
        }
        not_empty_.notify_all();
    }

    bool is_completed() { return is_completed_; }

    void push(const T &item) {
        unique_lock<mutex> lock(mutex_);
        not_full_.wait(lock, [this]() { return queue_.size() < capacity_; });

        queue_.push(item);
        not_empty_.notify_one();
    }

    optional<T> pop() {
        unique_lock<mutex> lock(mutex_);
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
