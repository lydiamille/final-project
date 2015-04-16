run: bev.o file_access.o ideas.o
	gcc -o run bev.o file_access.o ideas.o
bev.o: bev.c file_access.h ideas.h
	gcc -c bev.c
file_access.o: file_access.c file_access.h
	gcc -c file_access.c
ideas.o: ideas.c ideas.h
	gcc -c ideas.c
