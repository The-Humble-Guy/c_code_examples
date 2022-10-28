TARGET = graph
FLAGS = -Wall -Werror -Wextra
SOURCE = \
			../src/*.c
all: clean graph

clean:
	cd ../build && \
	rm -rf *

graph:
	cd ../build && \
	gcc $(FLAGS) $(SOURCE) -lm -o $(TARGET)