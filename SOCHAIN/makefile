# Grupo 35
# Membros: Francisco Lima nº 61864, Marcio Caetano nº 61799
#

.RECIPEPREFIX = >

OBJ_FILES = main.o memory.o process.o server.o wallet.o csignal.o clog.o csettings.o cstats.o ctime.o synchronization.o
OBJ_PATHS = $(addprefix $(OBJ_DIR)/,$(OBJ_FILES))
INC_DIR = inc
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
FLAGS = -Wall -Wextra -Wno-unused-parameter -I$(INC_DIR)
FLAGS_ENTREGA = -Wall -Wextra -Werror -I$(INC_DIR)
LIBS = -lm
CC = gcc
PROGRAM = $(BIN_DIR)/SOchain
DEFAULT_ARGS_FILE = args.txt
DEFAULT_ARGS = "256.0\n16\n4\n64\n512\n"
DEFAULT_SETTINGS_FILE = settings.txt
DEFAULT_LOG_FILE = log.txt
DEFAULT_STATS_FILE = stats.txt
DEFAULT_PERIOD = 20
DEFAULT_SETTINGS = "$(DEFAULT_LOG_FILE)\n$(DEFAULT_STATS_FILE)\n$(DEFAULT_PERIOD)"

all: $(PROGRAM) $(DEFAULT_ARGS_FILE) $(DEFAULT_SETTINGS_FILE) $(DEFAULT_LOG_FILE) $(DEFAULT_STATS_FILE)

$(OUTPUT_DIR):
>	mkdir -p $(OUTPUT_DIR)

$(OBJ_DIR):
>   mkdir -p $(OBJ_DIR)

$(DEFAULT_ARGS_FILE):
>	[ -f $(DEFAULT_ARGS_FILE) ] || printf $(DEFAULT_ARGS) > $(DEFAULT_ARGS_FILE)

$(DEFAULT_SETTINGS_FILE):
>	[ -f $(DEFAULT_SETTINGS_FILE) ] || printf $(DEFAULT_SETTINGS) > $(DEFAULT_SETTINGS_FILE)

$(DEFAULT_LOG_FILE):
>	[ -f $(DEFAULT_LOG_FILE) ] || touch $(DEFAULT_LOG_FILE)

$(DEFAULT_STATS_FILE):
>	[ -f $(DEFAULT_STATS_FILE) ] || touch $(DEFAULT_STATS_FILE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
>   $(CC) $(FLAGS) -o $@ -c $^

$(PROGRAM): $(OBJ_PATHS) | $(OUTPUT_DIR)
>   $(CC) $(OBJ_PATHS) $(LIBS) -o $(PROGRAM)

clean:
>   rm -f $(OBJ_DIR)/*.o
>   rm -f $(PROGRAM)

fclean: clean
> rm -f $(DEFAULT_ARGS_FILE)
> rm -f $(DEFAULT_SETTINGS)
> rm -f $(DEFAULT_LOG_FILE)
> rm -f $(DEFAULT_STATS_FILE)

re:
>	clear
>	make clean
>	make all

run: all
>	./$(PROGRAM) $(DEFAULT_ARGS_FILE) $(DEFAULT_SETTINGS_FILE)

prof:
>		./$(bin_DIR)/SOchain_profs 256.0 16 4 64 512

val: re
>	valgrind --leak-check=full -s ./$(PROGRAM) $(DEFAULT_ARGS_FILE) $(DEFAULT_SETTINGS_FILE)

.PHONY: all clean re run prof val fclean

