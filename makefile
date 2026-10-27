CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
CPPFLAGS = -Iheaders -Itest

SRC = sources
TEST_FOLDER = tests

FORMAT_FILES = $(SRC)/*.c headers/*.h

all : build/main.o build/maze.o build/union_find.o build/frontend.o build/position.o build/input.o build/player_actions.o build/score_handler.o
	$(CC) -o build/maze $^

test : tests/test_maze tests/test_file
	./$(TEST_FOLDER)/test_maze
	./$(TEST_FOLDER)/test_file

tests/test_maze : build/test_maze.o build/maze.o build/union_find.o build/frontend.o build/position.o build/input.o build/player_actions.o build/score_handler.o
	$(CC) -o $@ $^

tests/test_file : build/test_file.o build/maze.o build/union_find.o build/frontend.o build/position.o build/input.o build/player_actions.o build/score_handler.o
	$(CC) -o $@ $^

build/%.o : $(SRC)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/%.o : $(TEST_FOLDER)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

build/test_maze.o: $(TEST_FOLDER)/test_maze.c $(TEST_FOLDER)/test.h $(TEST_FOLDER)/minunit.h
build/test_file.o: $(TEST_FOLDER)/test_file.c $(TEST_FOLDER)/test.h $(TEST_FOLDER)/minunit.h
build/main.o : $(SRC)/main.c build/maze.o
build/maze.o : $(SRC)/maze.c headers/maze.h
build/union_find.o : $(SRC)/union_find.c headers/union_find.h
build/frontend.o : $(SRC)/frontend.c headers/frontend.h
build/position.o : $(SRC)/position.c headers/position.h
build/input.o : $(SRC)/input.c headers/input.h
build/player_actions.o : $(SRC)/player_actions.c headers/player_actions.h
build/score_handler.o : $(SRC)/score_handler.c headers/score_handler.h

clean : 
	rm -f build/* ./mazes/*.cfg ./score/*.score

format:
	clang-format -i $(FORMAT_FILES)

format-check:
	clang-format --dry-run -Werror $(FORMAT_FILES)