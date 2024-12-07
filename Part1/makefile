LIB = inlcude
SRC = src
TESTS = unit_testing

CC = g++

OBJS = $(SRC)/file_operations.o $(SRC)/util.o $(SRC)/vamana.o $(SRC)/main.o $(SRC)/find_check.o

EXEC = main

TEST_OBJS = $(SRC)/util.o $(SRC)/vamana.o $(SRC)/find_check.o

TESTS_EXEC = temp_tests

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

$(TESTS_EXEC): $(TESTS)/temp_tests.cpp $(TEST_OBJS)
	$(CC) $(TEST_OBJS) $(TESTS)/temp_tests.cpp -o $(TESTS_EXEC)


tests: $(TESTS_EXEC)
	./$(TESTS_EXEC)


val: $(TESTS_EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TESTS_EXEC)


clean:
	rm -f $(OBJS) $(EXEC) $(TESTS_EXEC)

run:
	./$(EXEC)