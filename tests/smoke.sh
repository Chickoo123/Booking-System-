#!/usr/bin/env bash
# End-to-end smoke tests: drive the interactive programs with scripted input.
set -euo pipefail
cd "$(dirname "$0")/.."

expect() { grep -qF -- "$2" <<<"$1" || { echo "FAIL: expected '$2'"; echo "$1"; exit 1; }; }

# Hotel: book, reject out-of-range, duplicate and garbage input, cancel FIFO.
out=$(printf '1\n101\n7\n1\n7\nabc\n1\n12\n2\n3\n4\n' | ./build/hotel_booking)
expect "$out" "Room 7 booked"
expect "$out" "between 1 and 100"
expect "$out" "Room 7 is already booked"
expect "$out" "That is not a number"
expect "$out" "Booking cancelled for room 7"
expect "$out" "  Room 12"

# EOF must terminate cleanly instead of looping forever.
printf '1\n' | timeout 5 ./build/hotel_booking >/dev/null

# Ticket: fill, reject overflow, process one, then the freed slot is reusable.
out=$(printf '1\n1\n1\n2\n1\n3\n1\n4\n1\n5\n1\n6\n2\n1\n6\n3\n4\n' | ./build/ticket_booking)
expect "$out" "Queue is full"
expect "$out" "Processing ticket request 1"
expect "$out" "Queue (next first): 2 3 4 5 6"

echo "Smoke tests passed."
