LIB = include
SRC = src
TESTS = unit_testing

CC = g++
CFLAGS = -O2 -I$(LIB)  # Optimization flag and include path for headers

OBJS = $(SRC)/file_operations.o $(SRC)/util.o $(SRC)/vamana.o $(SRC)/main.o $(SRC)/find_check.o
EXEC = main

TEST_OBJS = $(SRC)/util.o $(SRC)/vamana.o $(SRC)/find_check.o $(SRC)/file_operations.o
TESTS_EXEC = temp_tests

# Pattern rule for compiling object files
$(SRC)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Main executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

# Test executable
$(TESTS_EXEC): $(TESTS)/temp_tests.cpp $(TEST_OBJS)
	$(CC) $(CFLAGS) $(TEST_OBJS) $(TESTS)/temp_tests.cpp -o $(TESTS_EXEC)

# Run tests
tests: $(TESTS_EXEC)
	./$(TESTS_EXEC)

# Run Valgrind
val: $(TESTS_EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TESTS_EXEC)

# Clean build artifacts
clean:
	rm -f $(OBJS) $(EXEC) $(TESTS_EXEC)

# Run main executable
run: $(EXEC)
	./$(EXEC)
