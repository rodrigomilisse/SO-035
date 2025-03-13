INC_DIR = inc
SRC_DIR = src
OBJ_DIR = obj
OBJ_FILES = $(OBJ_DIR)/main.o $(OBJ_DIR)/memory.o $(OBJ_DIR)/process.o $(OBJ_DIR)/server.o $(OBJ_DIR)/wallet.o
FLAGS = -Wall -Wextra -Werror -I$(INC_DIR)
CC = gcc
PROGRAM_NAME = Main
OUTPUT_DIR = bin

all: $(PROGRAM_NAME)

$(OBJ_DIR):
    mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
    $(CC) $(FLAGS) -o $@ -c $^

$(PROGRAM_NAME): $(OBJ_FILES)
    $(CC) $(OBJ_FILES) -o $(OUTPUT_DIR)/$(PROGRAM_NAME)

clean:
    rm -f $(OBJ_DIR)/*.o

fclean: clean
    rm -f $(OUTPUT_DIR)/$(PROGRAM_NAME)

re: fclean all

.PHONY: all clean fclean re
