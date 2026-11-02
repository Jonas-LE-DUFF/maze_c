CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
CPPFLAGS = -Iheaders -Itest
SANITIZE_FLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

SRC = sources
TEST_FOLDER = tests
BUILD_FOLDER = build
SCORE_FOLDER = scores
MAZES_FOLDER = mazes
HEADERS_FOLDER = headers

SCORES = $(SCORE_FOLDER)/*.score
MAZES  = $(MAZES_FOLDER)/*.cfg

OBJS = $(addprefix $(BUILD_FOLDER)/, maze.o union_find.o frontend.o position.o input.o player_actions.o score_handler.o)

FORMAT_FILES = $(SRC)/*.c headers/*.h

all : $(BUILD_FOLDER)/main.o $(OBJS) | $(MAZES_FOLDER) $(SCORE_FOLDER)
	$(CC) $(SANITIZE_FLAGS) -o build/maze $^ $(SANITIZE_FLAGS)

test : tests/test_maze tests/test_file
	./$(TEST_FOLDER)/test_maze
	./$(TEST_FOLDER)/test_file

$(MAZES_FOLDER) | $(SCORE_FOLDER): 
	mkdir $@

tests/% : build/%.o $(OBJS)
	$(CC) -o $@ $^

build/main.o : $(SRC)/main.c build/maze.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/%.o : $(SRC)/%.c headers/%.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/%.o : $(TEST_FOLDER)/%.c $(TEST_FOLDER)/test.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/test_maze.o: $(TEST_FOLDER)/test_maze.c $(TEST_FOLDER)/test.h $(TEST_FOLDER)/minunit.h
build/test_file.o: $(TEST_FOLDER)/test_file.c $(TEST_FOLDER)/test.h $(TEST_FOLDER)/minunit.h

clean : 
	rm -f $(BUILD_FOLDER)/* $(SCORES) $(MAZES)

format:
	clang-format -i $(FORMAT_FILES)

format-check:
	clang-format --dry-run -Werror $(FORMAT_FILES)