# Grupo 35
# Membros: Francisco Lima nº 61864, Marcio Caetano nº -----
#

.RECIPEPREFIX = >

OBJ_FILES = main.o memory.o process.o server.o wallet.o csignal.o
OBJ_PATHS = $(addprefix $(OBJ_DIR)/,$(OBJ_FILES))
INC_DIR = inc
SRC_DIR = src
OBJ_DIR = obj
FLAGS = -Wall -Wextra -Wno-unused-parameter -I$(INC_DIR)
FLAGS_ENTREGA = -Wall -Wextra -Werror -I$(INC_DIR)
LIBS = -lm
CC = gcc
PROGRAM_NAME = SOchain
OUTPUT_DIR = bin

all: $(PROGRAM_NAME)

$(OUTPUT_DIR):
>	mkdir -p $(OUTPUT_DIR)

$(OBJ_DIR):
>   mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
>   $(CC) $(FLAGS) $(LIBS) -o $@ -c $^

$(PROGRAM_NAME): $(OBJ_PATHS) | $(OUTPUT_DIR)
>   $(CC) $(OBJ_PATHS) -o $(OUTPUT_DIR)/$(PROGRAM_NAME)

clean:
>   rm -f $(OBJ_DIR)/*.o
>   rm -f $(OUTPUT_DIR)/$(PROGRAM_NAME)

re: clean all

run: all
> 	./$(OUTPUT_DIR)/$(PROGRAM_NAME) 10 2 1 8 3

prof:
>		./$(OUTPUT_DIR)/SOchain_profs 10 2 1 8 3

.PHONY: all clean re run prof
