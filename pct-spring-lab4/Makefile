CC = gcc
CFLAGS = -fopenmp
LDFLAGS = -lm
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Список имен .c файлов в папке src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Список имен объектных файлов
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Имя исполняемого файла
EXEC = $(BIN_DIR)/program

all: $(EXEC)

# Компиляция .c файлов в объектные файлы
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Сборка исполняемого файла
$(EXEC): $(OBJ_FILES) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o $(EXEC)

run:
	cd bin && ./program

graph:
	cd graphs && gnuplot graph.gp
