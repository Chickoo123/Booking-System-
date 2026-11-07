# Booking System — Ring-Buffer Queues in C++

[![Build & test](https://github.com/Chickoo123/Booking-System-/actions/workflows/ci.yml/badge.svg)](https://github.com/Chickoo123/Booking-System-/actions/workflows/ci.yml)
![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)

Two console booking systems built on one reusable, tested, fixed-capacity **circular queue** (`include/circular_queue.hpp`).

| Program | What it models | Data structure |
|---|---|---|
| `hotel_booking` | 100 rooms, no double-booking, cancel oldest booking first | `CircularQueue<int>` + occupancy bitmap |
| `ticket_booking` | Ticket requests served in arrival order | `CircularQueue<int>` (capacity 5) |

## Bugs fixed in this version

The first version of this repo had three classic queue bugs. Each now has a regression test.

1. **Wrap-around cancellation bug (hotel).** The empty check `front > rear` is wrong for a circular buffer: once `rear` wraps behind `front`, the queue looked empty while it still held bookings. The queue now tracks an explicit `count_`.
2. **Queue that never frees space (tickets).** The linear array queue only moved `front` forward, so after 5 requests it reported "full" forever, even with every request processed. The ring buffer reuses slots.
3. **Infinite loop on bad input.** `cin >> int` on input like `abc`, or at EOF, made the menu spin forever. `read_int()` validates the range, recovers from bad input, and exits cleanly at EOF.

The hotel program also rejects out-of-range and already-booked room numbers.

## Complexity

| Operation | Time | Notes |
|---|---|---|
| `push` / `pop` / `front` | O(1) | index arithmetic modulo capacity |
| `for_each` | O(n) | oldest → newest |
| Space | O(capacity) | allocated once |

## Build & run

```bash
make            # builds build/hotel_booking and build/ticket_booking
make test       # unit tests + scripted end-to-end smoke tests
./build/hotel_booking
```

On Windows, use MSYS2/MinGW or WSL, or compile directly:

```bash
g++ -std=c++17 -Iinclude src/hotel_booking.cpp -o hotel_booking
```

CI builds with both **g++** and **clang++** using `-Wall -Wextra -Wpedantic -Werror`, then runs every test again under **AddressSanitizer + UndefinedBehaviorSanitizer**.

## Layout

```
include/circular_queue.hpp   generic ring-buffer queue
include/input.hpp            safe integer input
src/hotel_booking.cpp        hotel program
src/ticket_booking.cpp       ticket program
tests/test_circular_queue.cpp unit tests (wrap-around, slot reuse, FIFO)
tests/smoke.sh               drives both programs with scripted stdin
```

See also: [linked-list stack & queue booking systems](https://github.com/Chickoo123/Flight-Ticket-booking-Queue-Implementation-using-Linked-List-).
