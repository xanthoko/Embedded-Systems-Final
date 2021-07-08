CC=gcc
RUN = scan test1
RM = rm -f

# DIRECTORIES
SRC = src
BIN = bin
TESTS = tests
HEADERS = headers

all: $(RUN)

scan: $(SRC)/main.c $(SRC)/actions.c $(SRC)/contacts.c $(SRC)/timer.c 
	$(CC) $^ -o $(BIN)/$@ -I $(HEADERS)

test1: $(TESTS)/test_convert_to_close.c $(SRC)/actions.c $(SRC)/contacts.c $(SRC)/timer.c
	$(CC) $^ -o $(BIN)/$@ -I $(HEADERS)

test2: $(TESTS)/test_delete_close.c $(SRC)/actions.c $(SRC)/contacts.c $(SRC)/timer.c
	$(CC) $^ -o $(BIN)/$@ -I $(HEADERS)

test3: $(TESTS)/test_delete_non_close.c $(SRC)/actions.c $(SRC)/contacts.c $(SRC)/timer.c
	$(CC) $^ -o $(BIN)/$@ -I $(HEADERS)

clean:
	$(RM) $(BIN)/*