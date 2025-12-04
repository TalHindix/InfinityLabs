#******************************************************************************
# Exercise:    ThreadPool
# Date:        03/12/2025
# Developer:   Tal Hindi
# Reviewer:    
# Status:      
#******************************************************************************

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -g -pthread
LDFLAGS = -pthread

# Directories - relative to project root
FW_INC_DIR = framework/inc
FW_SRC_DIR = framework/src
FW_TEST_DIR = framework/test
OBJ_DIR = obj
BIN_DIR = bin

INCLUDES = -I$(FW_INC_DIR)

# Headers
HEADERS = $(FW_INC_DIR)/thread_pool.hpp \
          $(FW_INC_DIR)/waitablequeue.hpp \
          $(FW_INC_DIR)/pq.hpp

# Target
TARGET = $(BIN_DIR)/thread_pool_test

.PHONY: all clean run debug release valgrind tsan help

all: directories $(TARGET)
	@echo "Build successful!"

directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(TARGET): $(OBJ_DIR)/thread_pool.o $(OBJ_DIR)/thread_pool_test.o
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/thread_pool.o: $(FW_SRC_DIR)/thread_pool.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/thread_pool_test.o: $(FW_TEST_DIR)/thread_pool_test.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run: all
	./$(TARGET)

debug: CXXFLAGS += -DDEBUG -O0
debug: clean all

release: CXXFLAGS += -DNDEBUG -O2
release: clean all

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

tsan: CXXFLAGS += -fsanitize=thread
tsan: LDFLAGS += -fsanitize=thread
tsan: clean all
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

help:
	@echo "Targets:"
	@echo "  all       - Build"
	@echo "  run       - Build and run"
	@echo "  clean     - Remove build files"
	@echo "  debug     - Debug build"
	@echo "  release   - Release build"
	@echo "  valgrind  - Run with Valgrind"
	@echo "  tsan      - Run with ThreadSanitizer"
