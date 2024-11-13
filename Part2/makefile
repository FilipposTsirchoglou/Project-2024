
LIB = inlcude
SRC = src

CC = g++

OBJS = $(SRC)/file_operations.o $(SRC)/util.o $(SRC)/vamana.o $(SRC)/main.o $(SRC)/find_check.o

EXEC = main

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)

run:
	./$(EXEC)