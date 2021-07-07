CC=gcc
RM = rm -f

SRC = src
BIN = bin
HEADERS = headers


scan: $(SRC)/main.c $(SRC)/actions.c $(SRC)/contacts.c $(SRC)/timer.c 
	$(CC) $^ -o $(BIN)/$@ -I $(HEADERS)

clean:
	$(RM) $(BIN)/*