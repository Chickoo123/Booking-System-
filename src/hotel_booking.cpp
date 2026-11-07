// Hotel Room Booking System - circular queue of bookings (FIFO cancellation).
//
// Fixes over the original version:
//  * cancellation no longer breaks once the ring buffer wraps around
//  * room numbers are range-checked and cannot be double-booked
//  * non-numeric input / EOF no longer causes an infinite loop
#include <iostream>
#include <vector>

#include "circular_queue.hpp"
#include "input.hpp"

namespace {
constexpr int kRooms = 100;

class Hotel {
public:
    Hotel() : bookings_(kRooms), occupied_(kRooms + 1, false) {}

    void book(int room) {
        if (occupied_[room]) {
            std::cout << "Room " << room << " is already booked.\n";
            return;
        }
        if (!bookings_.push(room)) {
            std::cout << "Hotel is fully booked.\n";
            return;
        }
        occupied_[room] = true;
        std::cout << "Room " << room << " booked. (" << bookings_.size() << "/" << kRooms
                  << " occupied)\n";
    }

    void cancel_earliest() {
        auto room = bookings_.pop();
        if (!room) {
            std::cout << "No bookings to cancel.\n";
            return;
        }
        occupied_[*room] = false;
        std::cout << "Booking cancelled for room " << *room << ".\n";
    }

    void display() const {
        if (bookings_.empty()) {
            std::cout << "No current bookings.\n";
            return;
        }
        std::cout << "Current bookings (oldest first):\n";
        bookings_.for_each([](int room) { std::cout << "  Room " << room << '\n'; });
    }

private:
    CircularQueue<int> bookings_;
    std::vector<bool> occupied_;
};
}  // namespace

int main() {
    Hotel hotel;
    while (true) {
        std::cout << "\n1) Book a room\n2) Cancel earliest booking\n3) Display bookings\n4) Exit\n";
        auto choice = read_int("Enter your choice: ", 1, 4);
        if (!choice || *choice == 4) break;
        switch (*choice) {
            case 1:
                if (auto room = read_int("Room number (1-100): ", 1, kRooms)) hotel.book(*room);
                break;
            case 2:
                hotel.cancel_earliest();
                break;
            case 3:
                hotel.display();
                break;
        }
    }
    std::cout << "Goodbye.\n";
    return 0;
}
