// Unit tests for CircularQueue. Build & run: make test
#undef NDEBUG
#include <cassert>
#include <iostream>
#include <string>

#include "circular_queue.hpp"

static void test_basic_fifo() {
    CircularQueue<int> q(3);
    assert(q.empty() && !q.full());
    assert(q.push(1) && q.push(2) && q.push(3));
    assert(q.full() && !q.push(4));
    assert(*q.pop() == 1 && *q.pop() == 2 && *q.pop() == 3);
    assert(q.empty() && !q.pop().has_value());
}

// Regression: the original hotel code broke once rear wrapped behind front.
static void test_wraparound_keeps_order() {
    CircularQueue<int> q(3);
    q.push(1);
    q.push(2);
    q.push(3);
    q.pop();
    q.pop();    // head now at index 2
    q.push(4);  // tail wraps to index 0
    q.push(5);  // ...and index 1
    assert(q.size() == 3 && q.full());
    assert(*q.front() == 3);
    assert(q.at(0) == 3 && q.at(1) == 4 && q.at(2) == 5);
    assert(*q.pop() == 3 && *q.pop() == 4 && *q.pop() == 5);
    assert(q.empty());
}

// Regression: the original ticket queue reported "full" forever after N pushes.
static void test_slots_are_reused() {
    CircularQueue<int> q(2);
    for (int round = 0; round < 1000; ++round) {
        assert(q.push(round));
        assert(*q.pop() == round);
    }
    assert(q.empty());
}

static void test_non_trivial_type() {
    CircularQueue<std::string> q(2);
    q.push("alice");
    q.push("bob");
    std::string joined;
    q.for_each([&](const std::string& s) { joined += s + ","; });
    assert(joined == "alice,bob,");
}

int main() {
    test_basic_fifo();
    test_wraparound_keeps_order();
    test_slots_are_reused();
    test_non_trivial_type();
    std::cout << "All circular queue tests passed.\n";
    return 0;
}
