
CC = g++

OBJS = functions.o temp_tests.o 

EXEC = temp_tests

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)

run:
	./$(EXEC)