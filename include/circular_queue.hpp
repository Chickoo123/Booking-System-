// circular_queue.hpp - fixed-capacity FIFO queue backed by a ring buffer.
//
// Tracks an explicit element count instead of comparing front/rear, which
// removes the classic "front > rear" bug once the indices wrap around.
// All operations are O(1); storage is O(capacity).
#pragma once

#include <cstddef>
#include <optional>
#include <vector>

template <typename T>
class CircularQueue {
public:
    explicit CircularQueue(std::size_t capacity) : buffer_(capacity) {}

    bool empty() const { return count_ == 0; }
    bool full() const { return count_ == buffer_.size(); }
    std::size_t size() const { return count_; }
    std::size_t capacity() const { return buffer_.size(); }

    // Returns false (and leaves the queue unchanged) when full.
    bool push(const T& value) {
        if (full()) return false;
        buffer_[(head_ + count_) % buffer_.size()] = value;
        ++count_;
        return true;
    }

    // Removes and returns the oldest element, or std::nullopt when empty.
    std::optional<T> pop() {
        if (empty()) return std::nullopt;
        T value = buffer_[head_];
        head_ = (head_ + 1) % buffer_.size();
        --count_;
        return value;
    }

    std::optional<T> front() const {
        if (empty()) return std::nullopt;
        return buffer_[head_];
    }

    // i = 0 is the oldest element. Caller must ensure i < size().
    const T& at(std::size_t i) const { return buffer_[(head_ + i) % buffer_.size()]; }

    template <typename Fn>
    void for_each(Fn fn) const {
        for (std::size_t i = 0; i < count_; ++i) fn(at(i));
    }

private:
    std::vector<T> buffer_;
    std::size_t head_ = 0;
    std::size_t count_ = 0;
};
