parse:
	gcc -o parse parse_file.c map_vec.c node.c

bfs: bfs.c
	gcc -o bfs bfs.c map_vec.c node.c frontier.c explored_vec.c parse_file.c

search: search.c
	gcc -o search search.c bfs.c map_vec.c node.c frontier.c explored_vec.c parse_file.c

clean:
	rm -f *.o parse bfs search

.PHONY: clean
