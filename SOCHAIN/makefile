INC_DIR = inc
SRC_DIR = src
OBJ_DIR = obj
OBJ_FILES = $(OBJ_DIR)/main.o $(OBJ_DIR)/memory.o $(OBJ_DIR)/process.o $(OBJ_DIR)/server.o $(OBJ_DIR)/wallet.o
FLAGS = -Wall -Wextra -I$(INC_DIR)
CC = gcc
PROGRAM_NAME = Main
OUTPUT_DIR = bin

all:
	make $(PROGRAM_NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(FLAGS) -o $@ -c $^

$(PROGRAM_NAME): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(OUTPUT_DIR)/$(PROGRAM_NAME) 

clean:
	rm $(OBJ_DIR)/*.o

fclean:
	make clean
	rm $(OUTPUT_DIR)/$(PROGRAM_NAME)
	
re:
	make fclean
	make all

.phony: all clean fclean re