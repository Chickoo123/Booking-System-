// Ticket Booking System - ticket requests processed in arrival order.
//
// The original linear array queue could never reuse slots: after MAX_SIZE
// requests it reported "full" forever, even when every request was processed.
// A ring buffer fixes that while keeping O(1) enqueue/dequeue.
#include <iostream>

#include "circular_queue.hpp"
#include "input.hpp"

int main() {
    constexpr std::size_t kCapacity = 5;
    CircularQueue<int> queue(kCapacity);

    while (true) {
        std::cout << "\nTicket Booking System\n1. Add ticket request\n2. Process next request\n"
                     "3. Display queue\n4. Exit\n";
        auto choice = read_int("Enter your choice: ", 1, 4);
        if (!choice || *choice == 4) break;

        if (*choice == 1) {
            auto id = read_int("Ticket ID: ", 1, 1000000);
            if (!id) break;
            if (queue.push(*id))
                std::cout << "Ticket request " << *id << " queued (" << queue.size() << "/"
                          << kCapacity << ").\n";
            else
                std::cout << "Queue is full. Process a request first.\n";
        } else if (*choice == 2) {
            if (auto id = queue.pop())
                std::cout << "Processing ticket request " << *id << ".\n";
            else
                std::cout << "No ticket requests to process.\n";
        } else if (queue.empty()) {
            std::cout << "No ticket requests in the queue.\n";
        } else {
            std::cout << "Queue (next first):";
            queue.for_each([](int id) { std::cout << ' ' << id; });
            std::cout << '\n';
        }
    }
    std::cout << "Exiting.\n";
    return 0;
}
