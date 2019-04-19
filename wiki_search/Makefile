SRCS := $(wildcard *.c)

parse:
	gcc -o parse parse_file.c map_vec.c node.c

bfs: bfs.c
	gcc -o bfs bfs.c map_vec.c node.c frontier.c explored_vec.c parse_file.c

search: $(SRCS)
	gcc -g -o search $(SRCS)

clean:
	rm -f *.o parse bfs search

.PHONY: clean
