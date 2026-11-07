CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror -Iinclude
BIN := build

.PHONY: all test clean

all: $(BIN)/hotel_booking $(BIN)/ticket_booking

$(BIN)/%: src/%.cpp include/*.hpp | $(BIN)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BIN)/test_circular_queue: tests/test_circular_queue.cpp include/circular_queue.hpp | $(BIN)
	$(CXX) $(CXXFLAGS) $< -o $@

test: all $(BIN)/test_circular_queue
	./$(BIN)/test_circular_queue
	bash tests/smoke.sh

$(BIN):
	mkdir -p $(BIN)

clean:
	rm -rf $(BIN)
