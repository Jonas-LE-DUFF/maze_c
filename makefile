CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
CPPFLAGS = -Iheaders -Itest

SRC = sources
TEST_FOLDER = tests

FORMAT_FILES = $(SRC)/*.c headers/*.h

all : build/main.o build/maze.o build/union_find.o build/frontend.o build/position.o build/input.o build/player_actions.o build/score_handler.o
	$(CC) -fsanitize=address,undefined -fno-omit-frame-pointer -o build/maze $^ -fsanitize=address,undefined -fno-omit-frame-pointer

test : tests/test_maze tests/test_file
	./$(TEST_FOLDER)/test_maze
	./$(TEST_FOLDER)/test_file

tests/test_maze : build/test_maze.o build/maze.o build/union_find.o build/frontend.o build/position.o build/input.o build/player_actions.o build/score_handler.o
	$(CC) -o $@ $^

tests/test_file : build/test_file.o build/maze.o build/union_find.o build/frontend.o build/position.o build/input.o build/player_actions.o build/score_handler.o
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
	rm -f build/* ./mazes/*.cfg ./score/*.score

format:
	clang-format -i $(FORMAT_FILES)

format-check:
	clang-format --dry-run -Werror $(FORMAT_FILES)